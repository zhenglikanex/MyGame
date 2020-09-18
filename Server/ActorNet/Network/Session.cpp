#include "Session.h"
#include <iostream>

namespace network
{
	Session::Session(uint32_t id,asio::io_context& io_context,asio::ip::tcp::socket socket)
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
		
		// ���л�message
		int size = Message::kHeaderSize + message.size();
		Message::Encode(send_data_.data(), message);

		// ����size�ֽں󷵻�
		asio::async_write(socket_, asio::buffer(send_data_, size),
			[this, self, callback](const asio::error_code& ec, std::size_t /*length*/)
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

		//��ȡMessage::kHeaderSize���ֽں󷵻�
		asio::async_read(socket_,asio::buffer(recv_data_,Message::kHeaderSize),
			[this, self](const asio::error_code& ec, std::size_t /*length*/)
		{
			// ����sessionָ��,ʹ���ڴ���ص��ڼ䲻�ᱻ����
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

		//��ȡbody_size���ֽں󷵻�
		asio::async_read(socket_, asio::buffer(recv_data_,message_.size()),
			// ���ﲻӦ�����ò���message,message�ᱻ�ͷ�
			[this, self](const asio::error_code& ec, std::size_t /*length*/)	
		{
			// ����sessionָ��,ʹ���ڴ���ص��ڼ䲻�ᱻ����
			if (!ec) {
				auto body_size = message_.size();
				message_.set_size(0);
				message_.Write(recv_data_.data(), body_size);
				if (receive_handler_) {
					receive_handler_(self, message_);
				}

				message_.Clear();

				// ����������Ϣ
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