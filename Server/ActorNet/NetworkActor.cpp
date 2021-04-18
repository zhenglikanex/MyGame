#include "NetworkActor.hpp"

#include "ActorNet.h"

namespace actor_net
{

	Connection::Connection(uint32_t owner_id, asio::ip::tcp::socket&& socket)
		: id_(0)
		, owner_id_(owner_id)
		, socket_(std::move(socket))
		, data_(std::array<uint8_t, kMaxMsgSize>{0})
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
		auto self = shared_from_this();

		// ��װ��tcp��ͷ
		auto send_buffer = std::make_shared<Buffer>(buffer.size() + sizeof(uint16_t));
		uint16_t msg_size = buffer.size();
		std::copy_n((uint8_t*)&msg_size, sizeof(uint16_t), send_buffer->data());
		std::copy_n(buffer.data(), msg_size, send_buffer->data());

		asio::async_write(socket_, asio::buffer(send_buffer->data(), send_buffer->size()),
			[send_buffer, self](const asio::error_code& ec, std::size_t /*length*/) //����send_buffer���������ڵ�send����
			{
			});
	}

	void Connection::Close()
	{
		socket_.close();
	}

	void Connection::ReadHead()
	{
		auto self = shared_from_this();
		socket_.async_read_some(asio::buffer(data_, kHeadSize),
			[this, self](std::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					uint16_t size;
					memcpy(&size, data_.data(), length);

					ReadBody(size);
				}
				else
				{
					if (close_handler_)
					{
						close_handler_(self);
					}
				}
			});
	}

	void Connection::ReadBody(uint16_t size)
	{
		auto self = shared_from_this();
		socket_.async_read_some(asio::buffer(data_, size),
			[this, self](std::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					Buffer data(length);
					std::copy_n(data_.data(), length, data.data());

					if (receive_handler_)
					{
						receive_handler_(self, std::move(data));
					}

					Receive();
				}
				else
				{
					if (close_handler_)
					{
						close_handler_(self);
					}
				}
			});
	}


	TcpServer::TcpServer(uint32_t owner_id, asio::io_context& io_context, uint16_t port)
		: owner_id_(owner_id)
		, acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
	{

	}

	void TcpServer::Start()
	{
		acceptor_.async_accept([this](asio::error_code ce, asio::ip::tcp::socket socket)
			{
				if (!ce)
				{
					auto connection = std::make_shared<Connection>(owner_id_, std::move(socket));
					connection->Receive();
					if (accept_handler_)
					{
						accept_handler_(std::move(connection));
					}
				}

				Start();
			});
	}

	UdpServer::UdpServer(uint32_t owner_id, asio::io_context& io_context, uint16_t port)
		: owner_id_(owner_id)
		, socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
		, data_(std::array<uint8_t, kMaxMsgSize>{0})
	{

	}

	void UdpServer::Start()
	{
		socket_.async_receive_from(
			asio::buffer(data_, kMaxMsgSize), udp_remote_endpoint_,
			[this](std::error_code ec, std::size_t bytes_recvd)
			{
				if (!ec && bytes_recvd > 0)
				{
					Buffer data(bytes_recvd);
					std::copy_n(data_.data(),bytes_recvd, data.data());

					if (receive_handler_)
					{
						receive_handler_(udp_remote_endpoint_, std::move(data));
					}
				}

				Start();
			});
	}

	void UdpServer::Send(const asio::ip::udp::endpoint& udp_remote_endpoint, Buffer&& buffer)
	{
		auto buffer_ptr = std::make_shared<Buffer>(std::move(buffer));
		socket_.async_send_to(
			asio::buffer(buffer_ptr->data(), buffer_ptr->size()), udp_remote_endpoint,
			[buffer_ptr](asio::error_code, std::size_t) // ����buffer�������ڵ����ͽ���
			{

			}
			);
	}

	NetworkActor::NetworkActor(std::shared_ptr<ActorNet> actor_net)
		: actor_net_(actor_net)
		, io_context_(asio::io_context())
		, work_(io_context_) // ��ֹio_contentû���¼����˳�
	{

	}

	void NetworkActor::Post(ActorMessage&& actor_msg)
	{
		auto actor_msg_ptr = std::make_shared<ActorMessage>(std::move(actor_msg));
		io_context_.post([this, actor_msg_ptr]()
			{
				auto& actor_msg = *actor_msg_ptr;
				auto src = actor_msg.src_id();
				if (actor_msg.name() == "tcp")
				{
					uint16_t port = std::any_cast<uint16_t>(actor_msg.data());
					CreateTcpServer(src, port);
				}
				else if (actor_msg.name() == "close_tcp_server")
				{
					uint32_t src = std::any_cast<uint32_t>(actor_msg.data());
					CloseTcpServer(src);
				}
				else if (actor_msg.name() == "tcp_send")
				{
					auto& [id, send_data] = std::any_cast<std::tuple<uint16_t, Buffer>>(actor_msg.data());
					TcpSend(id, std::move(send_data));
				}
				else if (actor_msg.name() == "tcp_close")
				{
					auto id = std::any_cast<uint16_t>(actor_msg.data());
					TcpClose(id);
				}
				else if (actor_msg.name() == "udp")
				{
					auto port = std::any_cast<uint16_t>(actor_msg.data());
					CreateUdpServer(src, port);
				}
				else if (actor_msg.name() == "close_udp_server")
				{
					CloseUdpServer(src);
				}
				else if (actor_msg.name() == "udp_send")
				{
					auto& [udp_remote_endpoint,send_data] = std::any_cast<std::tuple<asio::ip::udp::endpoint, Buffer>>(actor_msg.data());
					UdpSend(src,udp_remote_endpoint, std::move(send_data));
				}
			});
	}

	void NetworkActor::CreateTcpServer(uint32_t src, uint16_t port)
	{
		if (tcp_servers_.find(src) != tcp_servers_.end())
		{
			// todo log;
			return;
		}

		tcp_servers_.emplace(src,std::make_unique<TcpServer>(src, io_context_, port));
		tcp_servers_[src]->set_accept_handler(std::bind(&NetworkActor::TcpAccept, this, std::placeholders::_1));
	}

	void NetworkActor::CloseTcpServer(uint32_t src)
	{
		if (tcp_servers_.find(src) == tcp_servers_.end())
		{
			// todo log;
			return;
		}

		tcp_servers_.erase(src);
	}

	void NetworkActor::TcpAccept(const std::shared_ptr<Connection>& connection)
	{
		for (uint16_t i = 0; i < kMaxConnectionNum; ++i)
		{
			auto id = ++alloc_id_ % kMaxConnectionNum;
			if (!connections_[id])
			{
				connection->set_id(id);

				connection->set_receive_handler([this](const std::shared_ptr<Connection>& connection, Buffer&& data)
					{
						std::tuple<uint16_t, Buffer> tuple = std::make_tuple(connection->id(), std::move(data));
						actor_net_->SendActorMessage(0, connection->owner_id(), 0, ActorMessage::MessageType::kTypeNetwork, "tcp_receive", std::move(tuple));
					});

				connection->set_close_handler([this](const std::shared_ptr<Connection>& connection)
					{
						actor_net_->SendActorMessage(0, connection->owner_id(),0, ActorMessage::MessageType::kTypeNetwork, "tcp_close", connection->id());
					});

				actor_net_->SendActorMessage(0, connection->owner_id(), 0, ActorMessage::MessageType::kTypeNetwork, "tcp_connect", connection->id());
				connections_[id] = connection;
				return;
			}
		}

		connection->Close();
	}

	void NetworkActor::TcpSend(uint16_t connection_id, Buffer&& data)
	{
		auto& connection = connections_[connection_id];
		if (connection)
		{
			connection->Send(std::move(data));
		}
	}

	void NetworkActor::TcpClose(uint16_t connection_id)
	{
		auto& connection = connections_[connection_id];
		if (connection)
		{
			connection->Close();
		}
	}

	void NetworkActor::CreateUdpServer(uint32_t src, uint16_t port)
	{
		if (udp_servers_.find(src) != udp_servers_.end())
		{
			// todo log;
			return;
		}

		udp_servers_.emplace(src, std::make_unique<UdpServer>(src, io_context_, port));
		udp_servers_[src]->set_receive_handler([this,src](asio::ip::udp::endpoint udp_remove_point,Buffer&& data)
			{
				actor_net_->SendActorMessage(0, src, 0, ActorMessage::MessageType::kTypeNetwork, "udp_receive",std::make_tuple(std::move(udp_remove_point),std::move(data)));
			});
		udp_servers_[src]->Start();
	}

	void NetworkActor::CloseUdpServer(uint32_t src)
	{
		if (udp_servers_.find(src) == udp_servers_.end())
		{
			return;
		}

		udp_servers_.erase(src);
	}

	void NetworkActor::UdpSend(uint32_t src, asio::ip::udp::endpoint udp_remote_endpoint, Buffer&& data)
	{
		if (udp_servers_.find(src) == udp_servers_.end())
		{
			return;
		}

		udp_servers_[src]->Send(udp_remote_endpoint,std::move(data));
	}

	void NetworkActor::Run()
	{
		io_context_.run();
	}

	void NetworkActor::Stop()
	{
		io_context_.stop();
	}
}