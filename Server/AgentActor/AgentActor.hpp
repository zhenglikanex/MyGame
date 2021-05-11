#pragma once
#include "Actor.h"
#include "ActorMessage.h"

#include "Framework/Network/NetMessage.hpp"

using namespace actor_net;

class AgentActor : public Actor
{
public:
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
	void ClientConnect(const std::string& name, const Func& func)
	{
		client_handlers_.emplace(name, std::bind(func, this,std::placeholders::_1));
	}

private:
	void Send(const std::any& data);

	//actor call
	void JoinBattle(const std::any& data);
private:
	//actor request
private:
	// client request
	void Ping(const NetMessage& request);
	void JoinMatch(const NetMessage& request);
	void LeaveMatch(const NetMessage& request);
	void BattledLoaded(const NetMessage& request);
private:

	ActorId gate_;
	kcp_conv_t conv_;
	bool join_matching_;
	ActorId battle_;
	ClientHandlerMap client_handlers_;
};