#include "Network.h"

namespace network
{
	Network::Network(uint16_t port)
		:acceptor_(io_context_,asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port)), session_id_(0)
	{
		
	}

	void Network::Run()
	{
		// 异步监听
		AsyncAccept();
		io_context_.run();
	}

	void Network::Stop()
	{
	}

	void Network::AsyncAccept()
	{
		acceptor_.async_accept(std::bind(&Network::AcceptHandler, this, std::placeholders::_1, std::placeholders::_2)); //异步accept,socket建立连接后调用AcceptHandle
	}

	void Network::AcceptHandler(const asio::error_code& ce,asio::ip::tcp::socket socket)
	{
		// 分配seesion的唯一id
		uint32_t id = ++session_id_;

		// 创建socket会话
		SessionPtr session_ptr = std::make_shared<Session>(id, io_context_, std::move(socket));
		session_ptr->set_connect_handler(connect_handler_);
		session_ptr->set_receive_handler(receive_handler_);
		session_ptr->set_close_handler(close_handler_);

		// 开启会话监听
		session_ptr->Start();

		AsyncAccept();
	}

}