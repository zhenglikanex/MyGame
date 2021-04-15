#pragma once
#include "Actor.h"
#include "ActorMessage.h"

#include "Framework/Network/NetMessage.hpp"

using namespace actor_net;

class AgentActor : public Actor
{
public:
	using ActorHandlerMap = std::unordered_map<std::string, std::function<void(const std::any&)>>;
	using ClientHandlerMap = std::unordered_map<std::string, std::function<void(const NetMessage&)>>;

	AgentActor(ActorId id);
	virtual ~AgentActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void Receive(ActorMessage&& actor_msg) override;

	void Start(const std::any& data);
	void ClientReceive(const std::any& data);
private:
	template<class Func>
	void ActorConnect(const std::string& name, const Func& func)
	{
		actor_handlers_.emplace(name, std::bind(func, this, std::placeholders::_1));
	}

	template<class Func>
	void ClientConnect(const std::string& name, const Func& func)
	{
		client_handlers_.emplace(name, std::bind(func, this,std::placeholders::_1));
	}

	ActorId gate_;
	kcp_conv_t conv_;
	ActorHandlerMap actor_handlers_;
	ClientHandlerMap client_handlers_;
};