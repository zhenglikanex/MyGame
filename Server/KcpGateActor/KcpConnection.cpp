#include "KcpConnection.hpp"
#include <iostream>

KcpConnection::KcpConnection(kcp_conv_t conv, const asio::ip::udp::endpoint& endpoint,uint32_t cur_clock)
	: conv_(conv)
	, endpoint_(endpoint)
	, last_recv_clock_(cur_clock)
	, data_({ 0 })
{
	kcp_ = ikcp_create(conv, (void*)this);
	kcp_->output = &KcpConnection::UdpOutput;

	// 启动快速模式
	// 第二个参数 nodelay-启用以后若干常规加速将启动
	// 第三个参数 interval为内部处理时钟，默认设置为 10ms
	// 第四个参数 resend为快速重传指标，设置为2
	// 第五个参数 为是否禁用常规流控，这里禁止
	//ikcp_nodelay(kcp_, 1, 10, 2, 1);
	ikcp_nodelay(kcp_, 1, 5, 1, 1); // 设置成1次ACK跨越直接重传, 这样反应速度会更快. 内部时钟5毫秒.
}

KcpConnection::~KcpConnection()
{
	ikcp_release(kcp_);
	kcp_ = nullptr;
}

void KcpConnection::Receive(Buffer&& buffer,uint32_t cur_clock)
{
	last_recv_clock_ = cur_clock;

	char* data = (char*)buffer.data() + sizeof(kcp_conv_t);	// 跳过已经已经读取的conv字节
	size_t size = buffer.size() - sizeof(kcp_conv_t);

	ikcp_input(kcp_, data, size);
	
	{
		auto kcp_recvd_bytes = ikcp_recv(kcp_, data, size);
		if (kcp_recvd_bytes <= 0)
		{
			// todo:
			std::cout << "kcp_recvd_bytes<=0: " << kcp_recvd_bytes << std::endl;
		}
		else
		{
			Buffer recv_buffer(kcp_recvd_bytes);
			std::copy_n((uint8_t*)data_.data(), kcp_recvd_bytes, recv_buffer.data());
			receive_handler_(shared_from_this(), std::move(recv_buffer));
		}
	}
}

void KcpConnection::Send(Buffer&& buffer)
{
	int send_ret = ikcp_send(kcp_,(char*)buffer.data(), buffer.size());
	if (send_ret < 0)
	{
		std::cout << "send_ret<0: " << send_ret << std::endl;
	}
}

void KcpConnection::Update(uint32_t cur_clock)
{
	ikcp_update(kcp_, cur_clock);
}

bool KcpConnection::IsTimeout(uint32_t cur_clock)
{
	return cur_clock - last_recv_clock_ > kTimeoutTime;
}

int KcpConnection::UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user)
{
	auto* connection = (KcpConnection*)user;
	if (connection->send_handler_)
	{
		Buffer buffer(len);
		std::copy_n((const uint8_t*)buf, len, buffer.data());
		connection->send_handler_(connection->shared_from_this(),std::move(buffer));
	}

	return 1;
}
