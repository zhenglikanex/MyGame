#include "Network.h"

namespace network
{
	Network::Network(uint16_t port)
		:acceptor_(io_context_,asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port)), session_id_(0)
	{
		
	}

	void Network::Run()
	{
		// �첽����
		AsyncAccept();
		io_context_.run();
	}

	void Network::Stop()
	{
		io_context_.stop();
	}

	void Network::AsyncAccept()
	{
		acceptor_.async_accept(std::bind(&Network::AcceptHandler, this, std::placeholders::_1, std::placeholders::_2)); //�첽accept,socket�������Ӻ����AcceptHandle
	}

	void Network::AcceptHandler(const asio::error_code& ce,asio::ip::tcp::socket socket)
	{
		// ����seesion��Ψһid
		auto id = session_id_.fetch_add(1);

		// ����socket�Ự
		SessionPtr session_ptr = std::make_shared<Session>(id, io_context_, std::move(socket));
		session_ptr->set_connect_handler(connect_handler_);
		session_ptr->set_receive_handler(receive_handler_);
		session_ptr->set_close_handler(close_handler_);

		// �����Ự����
		session_ptr->Start();

		AsyncAccept();
	}

}