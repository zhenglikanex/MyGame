#pragma once

#include "IActor.h"
#include "ActorNet.h"
#include "ActorMessage.h"
#include "Network.h"
using namespace actor_net;

class MasterActor : public IActor
{
public:
	MasterActor();
	virtual ~MasterActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void OnReceive(const ActorMessagePtr& actor_msg) override;
private:
	network::NetworkPtr network_ptr_;			// 用于分布式节点连接
};