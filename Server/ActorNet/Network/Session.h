#pragma once

#include <memory>
#include <array>
#include <functional>

#include "Message.h"

#include "asio.hpp"

namespace network
{
	class Session;

	typedef std::shared_ptr<Session> SessionPtr;

	// ��������ĳ���ͻ���֮��ġ��Ự��
	// �������д�¼�
	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		typedef std::function<void(const SessionPtr&)> ConnectHandler;
		typedef std::function<void(const SessionPtr&,const Message&)> ReceiveHandler;
		typedef std::function<void()> SendCallback;
		typedef std::function<void(const SessionPtr&)> CloseHandler;

		typedef uint32_t session_id;

		Session(const Session&) = delete;
		Session& operator=(const Session&) = delete;

		Session(session_id id,asio::io_context& io_context,asio::ip::tcp::socket socket);

		uint32_t id() { return id_; }
		asio::ip::tcp::socket& socket() { return socket_; }

		void Start();
		void Send(const Message& message, const SendCallback& callback = nullptr);

		void set_connect_handler(const ConnectHandler& handler) { connect_handler_ = handler; }
		void set_receive_handler(const ReceiveHandler& handler) { receive_handler_ = handler; }
		void set_close_handler(const CloseHandler& handler) { close_handler_ = handler; }

	private:
		void ReceiveHeader();		// ��ȡ��Ϣͷ
		void ReceiveBody();		// ��ȡ��Ϣ��

		session_id id_;				// Ψһid
		asio::ip::tcp::socket socket_;
		std::array<uint8_t,Message::kHeaderSize + Message::kBodySize> recv_data_;
		std::array<uint8_t, Message::kHeaderSize + Message::kBodySize> send_data_;

		Message message_;

		ConnectHandler connect_handler_;
		ReceiveHandler receive_handler_;
		CloseHandler close_handler_;

	};
}