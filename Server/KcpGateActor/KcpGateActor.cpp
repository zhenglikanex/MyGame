#include "KcpGateActor.hpp"

#include <iostream>

KcpGateActor::KcpGateActor(ActorId id)
	: Actor(id)
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
		std::string str((char*)buffer.data(), buffer.size());
		std::cout << str << std::endl;
	}
	else
	{
		std::cout << "未处理的消息" << actor_msg.name() << std::endl;
	}
}

ACTOR_IMPLEMENT(KcpGateActor)

