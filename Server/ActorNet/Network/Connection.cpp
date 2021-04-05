#include "Connection.hpp"
#include "NetworkActor.hpp"


Connection::Connection(uint32_t owner_id, asio::ip::tcp::socket&& socket)
	: owner_id_(owner_id)
	, socket_(std::move(socket))
{

}

Connection::~Connection()
{

}

void Connection::Receive()
{
	ReadHead();
}

void Connection::Send(Buffer&& buffer)
{
	auto buffer_ptr = std::make_shared<Buffer>(std::move(buffer));
	asio::async_write(socket_, asio::buffer(buffer_ptr->data(),buffer_ptr->size()),
		[this, buffer_ptr](const asio::error_code& ec, std::size_t /*length*/) //保持buffer的生命周期到send结束
		{
		});
}

void Connection::Close()
{
	socket_.close();
}

void Connection::ReadHead()
{
	socket_.async_read_some(asio::buffer(data_, kHeadSize),
		[this](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				uint16_t size;
				memcpy(&size, data_.data(), kHeadSize);

				ReadBody(size);
			}
			else 
			{
				// todo: close
			}
		});
}

void Connection::ReadBody(uint16_t size)
{
	socket_.async_read_some(asio::buffer(data_, size),
		[this,size](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				Buffer data(size);
				std::copy_n(data.data(), data_.data(),size);

				ReadHead();
			}
			else
			{
				if (close_handler_)
				{
					close_handler_();
				}
				// todo: close
			}
		});
}
