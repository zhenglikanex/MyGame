#include "KcpGateActor.hpp"

#include "Framework/Proto/NetMessage.hpp"

#include "Server/KcpGateActor/Kcp/ikcp.h"

KcpGateActor::KcpGateActor(ActorId id)
	: Actor(id)
	, alloc_conv_(1000)
	, cur_clock_(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
	, kcp_timer_(0)
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

	// update all kcp connection
	kcp_timer_ = AddTimer(1, -1, [this]() {
		uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		cur_clock_ = now;

		for (auto& entry : connections_)
		{
			entry.second->Update(cur_clock_);

			if (entry.second->IsTimeout(cur_clock_))
			{
				timeout_connections_.emplace_back(entry.second->conv());
			}
		}

		for (auto conv : timeout_connections_)
		{
			ForceDisconnection(conv);
		}
		timeout_connections_.clear();
		});

	ActorConnect("send", &KcpGateActor::Send, this);

	std::cout << "KcpInit:" << id() << std::endl;

	return true;
}

void KcpGateActor::Stop()
{
	CancelTimer(kcp_timer_);
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
				iter->second->Receive(std::move(buffer),cur_clock_);
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
	auto connection = std::make_shared<KcpConnection>(conv, endpoint,cur_clock_);
	connection->set_receive_handler(std::bind(&KcpGateActor::KcpReciveHandler, this, std::placeholders::_1, std::placeholders::_2));
	connection->set_send_handler(std::bind(&KcpGateActor::KcpSendHandler, this, std::placeholders::_1, std::placeholders::_2));
	connections_.emplace(conv, std::move(connection));
}

void KcpGateActor::ForceDisconnection(kcp_conv_t conv)
{
	std::cout << "ForceDisconnection" << std::endl;
	connections_.erase(conv);
	auto iter = agents_.find(conv);
	if (iter != agents_.end())
	{
		KillActor(iter->second);
		agents_.erase(iter);
	}
}

void KcpGateActor::UdpSend(const asio::ip::udp::endpoint& endpoint, Buffer&& buffer)
{
	network_component_->UdpSend(endpoint, std::move(buffer));
}

void KcpGateActor::KcpSend(kcp_conv_t conv, Buffer&& buffer)
{
	auto iter = connections_.find(conv);
	if (iter != connections_.end())
	{
		iter->second->Send(std::move(buffer));
	}
}

void KcpGateActor::KcpReciveHandler(const KcpConnection& connection, Buffer&& buffer)
{
	if (IsKcpConnectedMsg((char*)buffer.data(),buffer.size()))
	{
		ConnectSuccessHandler(connection.endpoint(), connection.conv());
	}
	else
	{
		auto iter = agents_.find(connection.conv());
		if (iter != agents_.end())
		{
			NetMessage message;
			message.Parse(buffer.data(), buffer.size());
			Call(iter->second, "client",std::move(message));
		}
		else
		{
			std::cout << "not agent" << std::endl;
		}
	}
}

void KcpGateActor::KcpSendHandler(const KcpConnection& connection, Buffer&& buffer)
{
	network_component_->UdpSend(connection.endpoint(), std::move(buffer));
}

void KcpGateActor::ConnectHandler(const asio::ip::udp::endpoint& endpoint)
{
	std::cout << "connect" << endpoint.address().to_string() << std::endl;

	kcp_conv_t conv = alloc_conv_++;
	AddConnection(conv, endpoint);

	std::string msg = MakeRspKcpConnectMsg(conv);
	Buffer buffer(msg.size());
	std::copy_n((uint8_t*)msg.data(), buffer.size(), buffer.data());
	network_component_->UdpSend(endpoint, std::move(buffer));
}

void KcpGateActor::ConnectSuccessHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv)
{
	std::cout << "ConnectSuccessHandler" << endpoint.address().to_string() << conv << std::endl;
	auto agent = StartActor("AgentActor.dll");
	if (agent != kNull)
	{
		agents_.emplace(conv, agent);
		auto config = std::make_tuple(this->id(), conv);
		Call(agent, "start", config);
	}
}

void KcpGateActor::DisconnectHandler(const asio::ip::udp::endpoint& endpoint, kcp_conv_t conv)
{
	ForceDisconnection(conv);
}

void KcpGateActor::Send(const std::any& data)
{
	auto& [conv, message] = std::any_cast<std::tuple<uint32_t, NetMessage>>(data);
	Buffer buffer(message.GetByteSize());
	message.Serialize(buffer.data());
	KcpSend(conv, std::move(buffer));
}

ACTOR_IMPLEMENT(KcpGateActor)

