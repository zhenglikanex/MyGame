#pragma once

#include <iostream>
#include <ctime>
#include <iomanip>
#include <chrono>

#include "Actor.h"
#include "ActorMessage.h"
#include "NetworkComponent.hpp"
#include "KcpConnection.hpp"

#include "Framework/Network/NetMessage.hpp"

using namespace actor_net;
using namespace std::chrono;

class KcpGateActor : public Actor
{
public:
	KcpGateActor(ActorId id);
	virtual ~KcpGateActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void Receive(ActorMessage&& actor_msg) override;
	void NetworkReceive(ActorMessage&& actor_msg);

	void AddConnection(kcp_conv_t conv,const asio::ip::udp::endpoint& endpoint);
	void ForceDisconnection(kcp_conv_t conv);

	void UdpSend(const asio::ip::udp::endpoint& endpoint, Buffer&& buffer);
	void KcpSend(kcp_conv_t conv, Buffer&& buffer);

	void KcpReciveHandler(const KcpConnection& connection,Buffer&& buffer);
	void KcpSendHandler(const KcpConnection& connection, Buffer&& buffer);
	
	void ConnectHandler(const asio::ip::udp::endpoint& endpoint);
	void ConnectSuccessHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv, KcpMessage&& message);
	void DisconnectHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv, KcpMessage&& message);
private:
	std::shared_ptr<NetworkComponent> network_component_;
	std::unordered_map<kcp_conv_t, std::shared_ptr<KcpConnection>> connections_;
	std::unordered_map<kcp_conv_t, ActorId> agents_;
	std::vector<kcp_conv_t> timeout_connections_;
	kcp_conv_t alloc_conv_;
	uint32_t cur_clock_;
};