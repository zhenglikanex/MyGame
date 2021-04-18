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

	ActorConnect("start", &AgentActor::Start,this);
	ActorConnect("client", &AgentActor::ClientReceive,this);

	ClientConnect("ping", &AgentActor::Ping);

	return true;
}

void AgentActor::Stop()
{

}

void AgentActor::Receive(ActorMessage&& actor_msg)
{

}

void AgentActor::Start(const std::any& data)
{
	auto&[gate, conv] = std::any_cast<std::tuple<ActorId, kcp_conv_t>>(data);
	gate_ = gate;
	conv_ = conv;

	std::cout << "AgentStart" << gate << " " << conv << std::endl;
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

void AgentActor::Ping(const NetMessage& request)
{
	std::any data = std::make_tuple(conv_, request);
	Call(gate_, "send", std::move(data));
}

ACTOR_IMPLEMENT(AgentActor)

