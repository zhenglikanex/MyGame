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
	using HandlerMap = std::unordered_map<std::string,std::function<void(asio::ip::udp::endpoint,NetMessage&&)>>;

	KcpGateActor(ActorId id);
	virtual ~KcpGateActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void Receive(ActorMessage&& actor_msg) override;
	void NetworkReceive(ActorMessage&& actor_msg);

	template<class Func>
	void Connect(const std::string& name, const Func& func)
	{
		handlers_.emplace(name, std::bind(func, this, std::placeholders::_1, std::placeholders::_2));
	}

	void AddConnection(kcp_conv_t conv,const asio::ip::udp::endpoint& endpoint);
	void ForceDisconnection(kcp_conv_t conv);

	void UdpSend(const asio::ip::udp::endpoint& endpoint, Buffer&& buffer);
	void KcpSend(kcp_conv_t conv, Buffer&& buffer);

	void KcpReciveHandler(const std::shared_ptr<KcpConnection>& connection,Buffer&& buffer);
	void KcpSendHandler(const std::shared_ptr<KcpConnection>& connection, Buffer&& buffer);
	
	void ConnectHandler(const asio::ip::udp::endpoint& endpoint);
	void ConnectSuccessHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv, KcpMessage&& message);
	void DisconnectHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv, KcpMessage&& message);
private:
	std::shared_ptr<NetworkComponent> network_component_;
	HandlerMap handlers_;
	std::unordered_map<kcp_conv_t, std::shared_ptr<KcpConnection>> connections_;
	std::unordered_map<kcp_conv_t, ActorId> agents_;
	std::vector<kcp_conv_t> timeout_connections_;
	kcp_conv_t alloc_conv_;
	uint32_t cur_clock_;
};