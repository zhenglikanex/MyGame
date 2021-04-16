#pragma once

#include <cstdint>
#include <functional>
#include <chrono>
#include <atomic>

#include "Framework/Game/NetworkService.hpp"
#include "Framework/Network/NetMessage.hpp"

#include "Client/Kcp/ikcp.h"

#include "3rdparty/include/asio.hpp"

class ClientNetworkService : public NetworkService
{
public:
	enum class ConnectType
	{
		kTypeDisconnect,
		kTypeConnecting,
		kTypeConnected,
	};

	enum class ConnectStatus
	{
		kTypeConnected,
		kTypeDisconnect,
		kTypeTimeout
	};

	ClientNetworkService();
	~ClientNetworkService();

	void Run();
	void Stop();

	void Connect(const std::string& ip, uint16_t port,uint32_t timeout);
	void Disconnect();

	void Send(uint8_t* data, uint32_t len) override;

	bool IsEmpty() const;
	const NetMessage& PopMessage();

	void set_connect_handler(const std::function<void(ConnectStatus status)>& connect_handler) 
	{ 
		connect_handler_ = connect_handler;
	}
private:
	static const uint16_t kMaxMsgSize = (1 << 16) - 1;

	static int UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user);

	void Connecting(uint32_t timeout);
	void Connected(kcp_conv_t conv);
	void Disconnected();

	void SendConnectedMsg();

	void UdpSend(uint8_t* data, uint32_t len);
	void UdpReceive();

	void KcpReceive(uint8_t* data,uint32_t len);
	void KcpUpdate();

	bool running_;
	std::thread thread_;

	asio::io_context io_context_;
	asio::ip::udp::socket socket_;
	asio::ip::udp::endpoint server_endpoint_;
	asio::steady_timer connect_timer_;
	asio::steady_timer kcp_update_timer_;
	std::array<uint8_t, kMaxMsgSize> data_;

	ConnectType type_;
	uint32_t connect_time_;
	kcp_conv_t conv_;
	ikcpcb* kcp_;
	
	std::function<void(ConnectStatus status)> connect_handler_;

	std::atomic<uint32_t> cur_read_;
	std::atomic<uint32_t> cur_write_;
	std::array<NetMessage, 150> messages_;
};
