#pragma once

#include <thread>

#include "Session.h"

namespace network
{
	// ����ͨ����
	// �����ͻ�����������async_accept������ĳ���ͻ��˽���socket���Ӻ�Ϊ������һ��session
	class Network
	{
	public:
		Network(uint16_t port);

		void Run();
		void Stop();

		asio::io_context& io_context() { return io_context_; }

		uint16_t port() const { return port_; }

		void set_connect_handler(const Session::ConnectHandler& handler) { connect_handler_ = handler; }
		void set_receive_handler(const Session::ReceiveHandler& handler) { receive_handler_ = handler; }
		void set_close_handler(const Session::CloseHandler& handler) { close_handler_ = handler; }

	private:
		// �����첽����
		void AsyncAccept();
		void AcceptHandler(const asio::error_code& ce, asio::ip::tcp::socket socket);

		asio::io_context io_context_;
		asio::ip::tcp::acceptor acceptor_;
		uint16_t port_;

		// session handler
		Session::ConnectHandler connect_handler_;
		Session::ReceiveHandler receive_handler_;
		Session::CloseHandler close_handler_;

		std::atomic<uint32_t> session_id_;
	};

	typedef std::shared_ptr<Network> NetworkPtr;
}