#ifndef NETWORK_COMPONENT_H_
#define NETWORK_COMPONENT_H_

#include "Config.hpp"
#include "ActorMessage.h"
#include "Actor.h"

#include "asio.hpp"

namespace actor_net
{
	class ActorNet;
	class Actor;

	class NetworkComponent
	{
	public:
		NetworkComponent(std::weak_ptr<Actor> weak_actor);

		void CreateTcpServer(uint16_t port);
		void CloseTcpServer();
		void TcpSend(uint16_t connection_id, Buffer&& data);
		void TcpClose(uint16_t connection_id);

		void CreateUdpServer(uint16_t port);
		void CloseUdpServer();
		void UdpSend(const asio::ip::udp::endpoint& udp_remote_endpoint, Buffer&& data);
	private:
		std::weak_ptr<Actor> weak_actor_;
	};
}

#endif