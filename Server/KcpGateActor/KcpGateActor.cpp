#include "KcpGateActor.hpp"

#include <iostream>

#include "Framework/Network/NetMessage.hpp"

#include "3rdparty/include/kcp/ikcp.h"

KcpGateActor::KcpGateActor(ActorId id)
	: Actor(id)
	, alloc_conv_(1000)
{

}

KcpGateActor::~KcpGateActor()
{

}

bool KcpGateActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!Actor::Init(actor_net))
	{
		return false;
	}

	network_component_ = std::make_shared<NetworkComponent>(shared_from_this());
	network_component_->CreateUdpServer(9523);

	return true;
}

void KcpGateActor::Stop()
{
	network_component_ = nullptr;
}

void KcpGateActor::Receive(ActorMessage&& actor_msg)
{
	if (actor_msg.type() == ActorMessage::MessageType::kTypeNetwork)
	{
		NetworkReceive(std::move(actor_msg));
	}
}

void KcpGateActor::NetworkReceive(ActorMessage&& actor_msg)
{
	if (actor_msg.name() == "udp_receive")
	{
		auto& [endpoint,buffer] = std::any_cast<std::tuple<asio::ip::udp::endpoint,Buffer>>(actor_msg.data());
		if (IsReqKcpConnectMsg((char*)buffer.data(), buffer.size()))
		{
			ConnectHandler(endpoint);
		}
		else
		{
			kcp_conv_t conv = ikcp_getconv(buffer.data());
			auto iter = connections_.find(conv);
			if (iter != connections_.end())
			{
				iter->second->Receive(std::move(buffer));
			}
			else 
			{
				std::cout << "error conv" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "未处理的消息" << actor_msg.name() << std::endl;
	}
}

void KcpGateActor::AddConnection(kcp_conv_t conv, const asio::ip::udp::endpoint& endpoint)
{
	auto connection = std::make_shared<KcpConnection>(conv, endpoint);
	connection->set_receive_handler(std::bind(&KcpGateActor::KcpRecive, this, std::placeholders::_1, std::placeholders::_2));
	connection->set_send_handler(std::bind(&KcpGateActor::KcpSend, this, std::placeholders::_1, std::placeholders::_2));
	connections_.emplace(conv, std::move(connection));
}

void KcpGateActor::ForceDisconnection(kcp_conv_t conv)
{
	
}

void KcpGateActor::KcpRecive(const std::shared_ptr<KcpConnection>& connection, Buffer&& buffer)
{
	KcpMessage kcp_message;
	kcp_message.Parse(buffer.data(), buffer.size());

	if (kcp_message.type() == KcpMessage::KcpMessageType::kTypeConnect)
	{
		ConnectSuccessHandler(connection->endpoint(), connection->conv(), std::move(kcp_message));
	}
	else if (kcp_message.type() == KcpMessage::KcpMessageType::kTypeDisconnect)
	{
		DisconnectHandler(connection->endpoint(), connection->conv(), std::move(kcp_message));
	}
	else
	{
		RecvHandler(connection->endpoint(), connection->conv(), std::move(kcp_message));
	}
}

void KcpGateActor::KcpSend(const std::shared_ptr<KcpConnection>& connection, Buffer&& buffer)
{
	network_component_->UdpSend(connection->endpoint(), std::move(buffer));
}

void KcpGateActor::ConnectHandler(const asio::ip::udp::endpoint& endpoint)
{
	kcp_conv_t conv = alloc_conv_++;
	AddConnection(conv, endpoint);

	std::string msg = MakeRspKcpConnectMsg(conv);
	Buffer buffer(msg.size());
	std::copy_n((uint8_t*)msg.data(), buffer.size(), buffer.data());
	network_component_->UdpSend(endpoint, std::move(buffer));
}

void KcpGateActor::ConnectSuccessHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv, KcpMessage&& message)
{
	NewActor("Agent", [this, endpoint, conv](ActorId agent)
		{
			agents_.emplace(conv, agent);

			auto config = std::make_tuple(this->id(), conv);
			Call(agent, "start", config);
		});
}

void KcpGateActor::DisconnectHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv, KcpMessage&& message)
{
	ForceDisconnection(conv);
}

void KcpGateActor::RecvHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv, KcpMessage&& message)
{
	auto iter = agents_.find(conv);
	if (iter != agents_.end())
	{
		Call(iter->second, "client", message.MoveNetMessage());
	}
}

ACTOR_IMPLEMENT(KcpGateActor)
