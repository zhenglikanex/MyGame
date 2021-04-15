#include "ClientNetworkService.hpp"

using namespace std::chrono;

ClientNetworkService::ClientNetworkService()
	: socket_(io_context_,asio::ip::udp::endpoint(asio::ip::udp::v4(),0))
	, timer_(io_context_)
	, type_(ConnectType::kTypeDisconnect)
	, connect_time_(0)
	, conv_(0)
	, cur_read_(0)
	, cur_write_(0)
{

}

ClientNetworkService::~ClientNetworkService()
{

}

void ClientNetworkService::Run()
{
	
}

void ClientNetworkService::Stop()
{

}

void ClientNetworkService::Connect(const std::string& ip, uint16_t port,uint32_t timeout)
{
	if (type_ != ConnectType::kTypeDisconnect)
	{
		connect_time_ = duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();

		server_endpoint_ = asio::ip::udp::endpoint(asio::ip::address::from_string(ip), port);
		type_ = ConnectType::kTypeConnecting;
		Connecting(timeout);
	}
}

void ClientNetworkService::Disconnect()
{
	if (type_ == ConnectType::kTypeConnected)
	{
		socket_.send_to(MakeKcpDisconnectMsg(), server_endpoint_);
		type_ == ConnectType::kTypeDisconnect;
	}
}

void ClientNetworkService::Send(uint8_t* data, uint32_t len)
{
	if (type_ == ConnectType::kTypeConnected)
	{
		socket_.send_to(asio::buffer(data, len),server_endpoint_);
	}
	else {
		//todo 输出信息
	}
}

bool ClientNetworkService::IsEmpty() const
{
	return cur_read_ == cur_write_;
}

const NetMessage& ClientNetworkService::PopMessage()
{
	if (cur_write_ < cur_write_)
	{
		return messages_[cur_write_++];
	}
}

void ClientNetworkService::Connecting(uint32_t timeout)
{
	socket_.send_to(MakeReqKcpConnectMsg(), server_endpoint_);
	// 定时请求连接udp,直到建立起kcp连接
	timer_.expires_after(milliseconds(100));
	timer_.async_wait([this, timeout](std::error_code ec)
	{
		if (!ec)
		{
			if (type_ == ConnectType::kTypeConnecting)
			{
				auto now = duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();
				if (now - connect_time_ > timeout)
				{
					type_ = ConnectType::kTypeDisconnect;
					connect_handler_(ConnectStatus::kTypeTimeout);
					return;
				}

				Connecting(timeout);
			}
		}
	});
}

void ClientNetworkService::Receive()
{
	socket_.async_receive_from(
		asio::buffer(data_, kMaxMsgSize), server_endpoint_,
		[this](std::error_code ec, std::size_t bytes_recvd)
	{
		if (!ec && bytes_recvd > 0)
		{
			if (type_ == ConnectType::kTypeConnecting && IsRspKcpConnectMsg((char*)data_.data(), bytes_recvd))
			{
				connect_handler_(ConnectStatus::kTypeConnected);
			}
			else
			{
				messages_[cur_write_].Parse(data_.data(), bytes_recvd);
				++cur_write_;
			}
		}
		Receive();
	});
}