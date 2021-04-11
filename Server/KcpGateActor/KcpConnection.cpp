#include "KcpConnection.hpp"
#include <iostream>

KcpConnection::KcpConnection(kcp_conv_t conv, const asio::ip::udp::endpoint& endpoint)
	: conv_(conv)
	, endpoint_(endpoint)
{
	kcp_ = ikcp_create(conv, (void*)this);
	kcp_->output = &KcpConnection::UdpOutput;

	// ��������ģʽ
	// �ڶ������� nodelay-�����Ժ����ɳ�����ٽ�����
	// ���������� intervalΪ�ڲ�����ʱ�ӣ�Ĭ������Ϊ 10ms
	// ���ĸ����� resendΪ�����ش�ָ�꣬����Ϊ2
	// ��������� Ϊ�Ƿ���ó������أ������ֹ
	//ikcp_nodelay(kcp_, 1, 10, 2, 1);
	ikcp_nodelay(kcp_, 1, 5, 1, 1); // ���ó�1��ACK��Խֱ���ش�, ������Ӧ�ٶȻ����. �ڲ�ʱ��5����.
}

KcpConnection::~KcpConnection()
{
	ikcp_release(kcp_);
	kcp_ = nullptr;
}

void KcpConnection::Receive(Buffer&& buffer)
{
	char* data = (char*)buffer.data() + sizeof(kcp_conv_t);	// �����Ѿ��Ѿ���ȡ��conv�ֽ�
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

int KcpConnection::UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user)
{
	auto* connection = (KcpConnection*)user;
	if (connection->send_handler_)
	{
		Buffer buffer(len);
		std::copy_n((const uint8_t*)buf, len, buffer.data());
		connection->send_handler_(connection->shared_from_this(),std::move(buffer));
	}
}
