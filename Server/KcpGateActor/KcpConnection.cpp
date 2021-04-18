#include "KcpConnection.hpp"
#include <iostream>

KcpConnection::KcpConnection(kcp_conv_t conv, const asio::ip::udp::endpoint& endpoint,uint32_t cur_clock)
	: conv_(conv)
	, endpoint_(endpoint)
	, last_recv_clock_(cur_clock)
{
	kcp_ = ikcp_create(conv, (void*)this);
	kcp_->output = &KcpConnection::UdpOutput;

	// ��������ģʽ
	// �ڶ������� nodelay-�����Ժ����ɳ�����ٽ�����
	// ���������� intervalΪ�ڲ�����ʱ�ӣ�Ĭ������Ϊ 10ms
	// ���ĸ����� resendΪ�����ش�ָ�꣬����Ϊ2
	// ��������� Ϊ�Ƿ���ó������أ������ֹ
	//ikcp_nodelay(kcp_, 1, 10, 2, 1);
	ikcp_nodelay(kcp_, 1, 1, 1, 1); // ���ó�1��ACK��Խֱ���ش�, ������Ӧ�ٶȻ����. �ڲ�ʱ��5����.
	kcp_->interval = 1;
	kcp_->rx_minrto = 5;
}

KcpConnection::~KcpConnection()
{
	ikcp_release(kcp_);
	kcp_ = nullptr;
}

void KcpConnection::Receive(Buffer&& buffer,uint32_t cur_clock)
{
	last_recv_clock_ = cur_clock;

	char* data = (char*)buffer.data();
	size_t size = buffer.size();

	ikcp_input(kcp_, data, size);
	
	{
		while (true)
		{
			auto kcp_recvd_bytes = ikcp_recv(kcp_, data, size);
			if (kcp_recvd_bytes <= 0)
			{
				// todo:
				break;
			}
			else
			{
				Buffer recv_buffer(kcp_recvd_bytes);
				std::copy_n((uint8_t*)data, kcp_recvd_bytes, recv_buffer.data());
				receive_handler_(*this, std::move(recv_buffer));
			}
		}
	}
}

void KcpConnection::Send(Buffer&& buffer)
{
	int send_ret = ikcp_send(kcp_, (char*)buffer.data(), buffer.size());
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
		connection->send_handler_(*connection,std::move(buffer));
	}

	return 1;
}
