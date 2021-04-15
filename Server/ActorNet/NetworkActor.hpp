#ifndef NETWORK_ACTOR_H_
#define NETWORK_ACTOR_H_

#include <memory>

#include "Config.hpp"
#include "ActorMessage.h"

#include "asio.hpp"

namespace actor_net
{
	static const uint16_t kMaxMsgSize = (1 << 16);
	static const uint8_t kHeadSize = 2;

	class Connection;
	class ActorNet;

	using AcceptHandler = std::function<void(const std::shared_ptr<Connection>&)>;
	using ReceiveHandler = std::function<void(const std::shared_ptr<Connection>&, Buffer&&)>;
	using SendCallback = std::function<void(const std::shared_ptr<Connection>&)>;
	using CloseHandler = std::function<void(const std::shared_ptr<Connection>&)>;

	class Connection : std::enable_shared_from_this<Connection>
	{
	public:
		Connection(uint32_t owner_id, asio::ip::tcp::socket&& socket);
		~Connection();

		void Receive();
		void Send(Buffer&& buffer);
		void Close();

		void set_receive_handler(const ReceiveHandler& handler) { receive_handler_ = handler; }
		void set_close_handler(const CloseHandler& handler) { close_handler_ = handler; }

		void set_id(uint16_t id) { id_ = id; }

		uint32_t id() const { return id_; }
		uint32_t owner_id() const { return owner_id_; }
	private:
		void ReadHead();
		void ReadBody(uint16_t size);

		uint16_t id_;
		uint32_t owner_id_;
		asio::ip::tcp::socket socket_;
		std::array<uint8_t, kMaxMsgSize> data_;
		ReceiveHandler receive_handler_;
		CloseHandler close_handler_;
	};

	class TcpServer
	{
	public:
		TcpServer(uint32_t owner_id,asio::io_context& io_context, uint16_t port);
		void Start();

		void set_accept_handler(const AcceptHandler& handler) { accept_handler_ = handler; }
	private:
		uint32_t owner_id_;
		asio::ip::tcp::acceptor acceptor_;
		AcceptHandler accept_handler_;
	};

	class UdpServer
	{
	public:
		using UdpReceiveHanlder = std::function<void(asio::ip::udp::endpoint, Buffer&&)>;

		UdpServer(uint32_t owner_id, asio::io_context& io_context, uint16_t port);
		void Start();
		void Send(const asio::ip::udp::endpoint& udp_remote_endpoint,Buffer&& buffer);
		void set_receive_handler(const UdpReceiveHanlder handler) { receive_handler_ = handler; }
	private:
		uint32_t owner_id_;
		asio::ip::udp::socket socket_;
		asio::ip::udp::endpoint udp_remote_endpoint_;
		std::array<uint8_t, kMaxMsgSize> data_;
		UdpReceiveHanlder receive_handler_;
	};

	class NetworkActor
	{
	public:
		NetworkActor(std::shared_ptr<ActorNet> actor_net);
		NetworkActor(const NetworkActor&) = delete;
		NetworkActor(NetworkActor&&) = delete;
		NetworkActor& operator=(const NetworkActor&) = delete;
		NetworkActor& operator=(NetworkActor&&) = delete;

		void Post(ActorMessage&& actor_msg);
		void Run();
		void Stop();
	private:
		void CreateTcpServer(uint32_t src, uint16_t port);
		void CloseTcpServer(uint32_t src);
		void TcpAccept(const std::shared_ptr<Connection>& connection);
		void TcpSend(uint16_t connection_id, Buffer&& data);
		void TcpClose(uint16_t connection_id);

		void CreateUdpServer(uint32_t src, uint16_t port);
		void CloseUdpServer(uint32_t src);
		void UdpSend(uint32_t src,asio::ip::udp::endpoint udp_remote_endpoint, Buffer&& data);		
	private:
		static const uint16_t kMaxConnectionNum = 2 ^ 16;

		std::shared_ptr<ActorNet> actor_net_;
		asio::io_context io_context_;
		asio::io_context::work work_;
		std::unordered_map<uint32_t,std::unique_ptr<TcpServer>> tcp_servers_;
		std::unordered_map<uint32_t,std::unique_ptr<UdpServer>> udp_servers_;
		std::array<std::shared_ptr<Connection>, kMaxConnectionNum> connections_;
		std::atomic<uint32_t> alloc_id_;
	};

}
#endif