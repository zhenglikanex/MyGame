#include "NetworkComponent.hpp"

#include "Actor.h"
#include "ActorNet.h"

namespace actor_net
{
	NetworkComponent::NetworkComponent(std::weak_ptr<Actor> weak_actor)
		: weak_actor_(weak_actor)
	{

	}

	void NetworkComponent::CreateTcpServer(uint16_t port)
	{
		auto actor = weak_actor_.lock();
		if (actor)
		{
			actor->actor_net()->CreateTcpServer(actor->id(), port);
		}
	}

	void NetworkComponent::CloseTcpServer()
	{
		auto actor = weak_actor_.lock();
		if (actor)
		{
			actor->actor_net()->CloseTcpServer(actor->id());
		}
	}

	void NetworkComponent::TcpSend(uint16_t connection_id, Buffer&& data)
	{
		auto actor = weak_actor_.lock();
		if (actor)
		{
			actor->actor_net()->TcpSend(connection_id, std::move(data));
		}
	}

	void NetworkComponent::TcpClose(uint16_t connection_id)
	{
		auto actor = weak_actor_.lock();
		if (actor)
		{
			actor->actor_net()->TcpClose(connection_id);
		}
	}

	void NetworkComponent::CreateUdpServer(uint16_t port)
	{
		auto actor = weak_actor_.lock();
		if (actor)
		{
			actor->actor_net()->CreateUdpServer(actor->id(), port);
		}
	}

	void NetworkComponent::CloseUdpServer()
	{
		auto actor = weak_actor_.lock();
		if (actor)
		{
			actor->actor_net()->CloseUdpServer(actor->id());
		}
	}

	void NetworkComponent::UdpSend(const asio::ip::udp::endpoint& udp_remote_endpoint, Buffer&& data)
	{
		auto actor = weak_actor_.lock();
		if (actor)
		{
			actor->actor_net()->UdpSend(actor->id(), udp_remote_endpoint, std::move(data));
		}
	}
}