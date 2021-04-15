#include "AgentActor.hpp"

#include <iostream>

AgentActor::AgentActor(ActorId id)
	: Actor(id)
{

}

AgentActor::~AgentActor()
{

}

bool AgentActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!Actor::Init(actor_net))
	{
		return false;
	}
	
	ActorConnect("start", &Start);
	ActorConnect("client", &ClientReceive);

	return true;
}

void AgentActor::Stop()
{

}

void AgentActor::Receive(ActorMessage&& actor_msg)
{
	if (actor_msg.type() == ActorMessage::MessageType::kTypeActor)
	{
		auto iter = actor_handlers_.find(actor_msg.name());
		if (iter != actor_handlers_.end())
		{
			iter->second(actor_msg.data());
		}
	}
	else if(actor_msg.type() == ActorMessage::MessageType::kTypeRequest)
	{
		
	}
}

void AgentActor::Start(const std::any& data)
{
	auto&[gate, conv] = std::any_cast<std::tuple<ActorId, kcp_conv_t>>(data);
	gate_ = gate;
	conv_ = conv;
}

void AgentActor::ClientReceive(const std::any& data)
{
	auto& message = std::any_cast<const NetMessage&>(data);
	auto iter = client_handlers_.find(message.name());
	if (iter != client_handlers_.end())
	{
		iter->second(message);
	}
	else
	{
		std::cout << "未处理的客户端消息:" << message.name() << std::endl;
	}
}

ACTOR_IMPLEMENT(AgentActor)