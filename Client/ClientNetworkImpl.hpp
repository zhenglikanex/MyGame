#pragma once

#include <cstdint>
#include <functional>
#include <chrono>
#include <atomic>
#include <cstdint>

#include "Framework/Proto/Kcp/ikcp.h"
#include "Framework/Proto/NetMessage.hpp"

#include "3rdparty/include/asio.hpp"

class ClientNetworkImpl
{
public:
	enum ConnectType : uint8_t
	{
		kTypeDisconnect,
		kTypeConnecting,
		kTypeConnected,
	};

	enum class ConnectErrorCode : uint8_t
	{
		kTypeNotError,
		kTypeServerDisconnect,
		kTypeServerTimeout,
	};

	ClientNetworkImpl();
	~ClientNetworkImpl();

	void Run();
	void Stop();
	bool IsRunning() const { return running_; }

	bool Connect(const std::string& ip, uint16_t port, uint32_t timeout);
	void Disconnect();
	bool IsConnected() const;

	void Send(std::vector<uint8_t>&& data);
	void Send(uint8_t* data, uint32_t len);

	bool IsEmpty() const;
	std::unique_ptr<std::vector<uint8_t>> PopBuffer();

	void set_connect_handler(const std::function<void(ConnectErrorCode status)>& connect_handler)
	{
		connect_handler_ = connect_handler;
	}

	uint8_t type() const { return type_; }
private:
	static const uint16_t kMaxMsgSize = 0xFFFF;

	static int UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user);

	void Connecting(uint32_t timeout);
	void Connected(kcp_conv_t conv);
	void Timeout();

	void SendConnectedMsg();

	void UdpSend(uint8_t* data, uint32_t len);
	void UdpReceive();

	void KcpReceive(uint8_t* data, uint32_t len);
	void KcpUpdate();

	std::atomic_bool running_;
	std::thread thread_;
	std::mutex mutex_;

	asio::io_context io_context_;
	asio::ip::udp::socket socket_;
	asio::ip::udp::endpoint server_endpoint_;
	asio::steady_timer connect_timer_;
	asio::steady_timer kcp_update_timer_;
	std::array<uint8_t, kMaxMsgSize> data_;

	std::atomic<uint8_t> type_;
	uint32_t connect_time_;
	kcp_conv_t conv_;
	ikcpcb* kcp_;
	uint32_t cur_clock_;

	std::function<void(ConnectErrorCode status)> connect_handler_;

	std::atomic<uint32_t> cur_read_;
	std::atomic<uint32_t> cur_write_;
	std::array<std::vector<uint8_t>, 3000> buffers_;
};
