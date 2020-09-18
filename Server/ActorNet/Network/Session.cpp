#include "Session.h"
#include <iostream>

namespace network
{
	Session::Session(uint32_t id, io_context& io_context, tcp::socket socket)
		:id_(id)
		, socket_(std::move(socket))
	{

	}

	void Session::Start()
	{
		if (connect_handler_) {
			connect_handler_(shared_from_this());
		}

		ReceiveHeader();
	}

	void Session::Send(const Message& message, const SendCallback& callback)
	{
		auto self(shared_from_this());
		
		// 序列化message
		int size = Message::kHeaderSize + message.size();
		Message::Encode(send_data_.data(), message);

		// 发送size字节后返回
		boost::asio::async_write(socket_, boost::asio::buffer(send_data_, size),
			[this, self, callback](const boost::system::error_code& ec, std::size_t /*length*/)
		{
			if (callback)
			{
				callback();
			}
		});
	}

	void Session::ReceiveHeader()
	{
		auto self(shared_from_this());

		//读取Message::kHeaderSize个字节后返回
		boost::asio::async_read(socket_,boost::asio::buffer(recv_data_,Message::kHeaderSize),
			[this, self](const boost::system::error_code& ec, std::size_t /*length*/)
		{
			// 捕获session指针,使其在处理回调期间不会被销毁
			if (!ec) {
				Message message;
				Message::Decode(message_,(uint8_t*)&recv_data_);

				ReceiveBody();
			}
			else {
				if (close_handler_) {
					close_handler_(self);
				}
			}
		});
	}

	void Session::ReceiveBody()
	{
		auto self(shared_from_this());

		//读取body_size个字节后返回
		boost::asio::async_read(socket_, boost::asio::buffer(recv_data_,message_.size()),
			// 这里不应该引用捕获message,message会被释放
			[this, self](const boost::system::error_code& ec, std::size_t /*length*/)	
		{
			// 捕获session指针,使其在处理回调期间不会被销毁
			if (!ec) {
				auto body_size = message_.size();
				message_.set_size(0);
				message_.Write(recv_data_.data(), body_size);
				if (receive_handler_) {
					receive_handler_(self, message_);
				}

				message_.Clear();

				// 继续接受消息
				ReceiveHeader();
			}
			else {
				if (close_handler_) {
					close_handler_(self);
				}
			}
		});
	}

}