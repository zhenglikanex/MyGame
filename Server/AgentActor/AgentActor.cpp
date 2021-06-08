#include "AgentActor.hpp"

#include <iostream>

#include "Framework/Proto/NetMessage.hpp"

#include "Framework/Proto/Battle.pb.h"

using namespace std::chrono;

AgentActor::AgentActor(ActorId id)
	: Actor(id)
	, gate_(kNull)
	, conv_(0)
	, join_matching_(false)
	, battle_(kNull)
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
	ActorConnect("send", &AgentActor::Send, this);
	ActorConnect("client", &AgentActor::ClientReceive,this);
	ActorConnect("join_battle", &AgentActor::JoinBattle, this);

	ClientConnect("ping", &AgentActor::Ping);
	ClientConnect("join_match", &AgentActor::JoinMatch);
	ClientConnect("battle_loaded", &AgentActor::BattledLoaded);
	ClientConnect("input_command", &AgentActor::InputCommand);

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

	//std::cout << "AgentStart" << gate << " " << conv << std::endl;
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

void AgentActor::Send(const std::any& data)
{
	auto& [name,buffer] = std::any_cast<std::tuple<std::string,std::vector<uint8_t>>>(data);

	NetMessage push;
	push.set_name(name);
	push.set_data(std::move(buffer));
	
	Call(gate_, "send", std::make_tuple(conv_, push));
}

void AgentActor::JoinBattle(const std::any& data)
{
	battle_ = std::any_cast<ActorId>(data);
}

void AgentActor::Ping(const NetMessage& request)
{
	std::any data = std::make_tuple(conv_, request);
	
	Proto::Ping ping;
	ping.ParseFromArray(request.data().data(), request.data().size());

	Call(gate_, "send", std::move(data));
}

void AgentActor::JoinMatch(const NetMessage& request)
{
	if (!join_matching_)
	{
		std::any data = id();
		Request("MatchActor", "join_match", std::move(data), [this,request](ActorMessage&& message)
			{
				join_matching_ = true;

				uint8_t status = 0;
				std::vector<uint8_t> buffer(1);
				std::copy_n(buffer.data(), 1, &status);

				NetMessage response;
				response.set_session(request.session());
				response.set_data(std::move(buffer));
				std::any data = std::make_tuple(conv_, std::move(response));
				Call(gate_, "send", std::move(data));
			});
	}
	else
	{
		// 偷懒不封装接口了,也不用protobuf了麻烦
		uint8_t status = 0;
		std::vector<uint8_t> buffer(1);
		std::copy_n(buffer.data(), 1, &status);

		NetMessage response;
		response.set_session(request.session());
		response.set_data(std::move(buffer));
		std::any data = std::make_tuple(conv_, std::move(response));
		Call(gate_, "send", std::move(data));
	}
}

void AgentActor::LeaveMatch(const NetMessage& request)
{
	if (join_matching_)
	{
		std::any data = id();
		Request("MatchActor", "leave_match", std::move(data), [this,request](ActorMessage&& message)
			{
				join_matching_ = false;

				uint8_t status = 0;
				std::vector<uint8_t> buffer(1);
				std::copy_n(buffer.data(), 1, &status);

				NetMessage response;
				response.set_session(request.session());
				response.set_data(std::move(buffer));
				std::any data = std::make_tuple(conv_, std::move(response));
				Call(gate_, "send", std::move(data));
			});
	}
	else
	{
		// 偷懒不封装接口了,也不用protobuf了麻烦
		uint8_t status = 0;
		std::vector<uint8_t> buffer(1);
		std::copy_n(buffer.data(), 1, &status);

		NetMessage response;
		response.set_session(request.session());
		response.set_data(std::move(buffer));
		std::any data = std::make_tuple(conv_, std::move(response));
		Call(gate_, "send", std::move(data));
	}
}

void AgentActor::BattledLoaded(const NetMessage& request)
{
	if (battle_ != kNull)
	{
		Call(battle_, "battle_loaded", std::any(id()));
	}

	NetMessage response;
	response.set_session(request.session());
	Call(gate_,"send", std::move(response));
}

void AgentActor::InputCommand(const NetMessage& request)
{
	if (battle_ != kNull)
	{
		std::cout << "input command" << battle_ << std::endl;
		Proto::GameCommand command;
		command.ParseFromArray(request.data().data(), request.data().size());
		
		Call(battle_, "input_command", std::make_tuple(id(),std::move(command)));
	}
}

ACTOR_IMPLEMENT(AgentActor)

