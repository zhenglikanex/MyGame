#pragma once

#include <functional>
#include <memory>

#include "Config.hpp"

#include "Framework/Network/NetMessage.hpp"

#include "3rdparty/include/asio.hpp"

class KcpConnection;

using namespace actor_net;

using KcpReceiveHandler = std::function<void(const std::shared_ptr<KcpConnection>&, Buffer&&)>;
using KcpSendHanlder = std::function<void(const std::shared_ptr<KcpConnection>&, Buffer&&)>;

class KcpConnection : std::enable_shared_from_this<KcpConnection>
{
public:
	static const uint32_t kTimeoutTime = 5000;	// 5sec
public:
	KcpConnection(kcp_conv_t conv,const asio::ip::udp::endpoint& endpoint,uint32_t cur_time);
	~KcpConnection();
	
	void Receive(Buffer&& buffer,uint32_t cur_time);
	void Send(Buffer&& buffer);

	void Update(uint32_t cur_clock);

	bool IsTimeout(uint32_t cur_clock);

	void set_receive_handler(const KcpReceiveHandler& handler) { receive_handler_ = handler; }
	void set_send_handler(const KcpSendHanlder& handler) { send_handler_ = handler; }

	kcp_conv_t conv() const { return conv_; }
	const asio::ip::udp::endpoint endpoint() const { return endpoint_; }
	const ikcpcb* kcp() const { return kcp_; }
private:
	static int UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user);

	kcp_conv_t conv_;
	asio::ip::udp::endpoint endpoint_;
	ikcpcb* kcp_;
	uint32_t last_recv_clock_;
	std::array<char, 2 ^ 16> data_;

	KcpReceiveHandler receive_handler_;
	KcpSendHanlder send_handler_;
};

