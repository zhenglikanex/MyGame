#include "ClientNetworkService.hpp"

#ifdef _WIN32

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#define TIME_BEGIN_PERIOD(ms) ::timeBeginPeriod(ms)
#define TIME_END_PERIOD(ms) ::timeEndPeriod(ms)

#else

#define TIME_BEGIN_PERIOD(ms) 
#define TIME_END_PERIOD(ms)

#endif // Windows


using namespace std::chrono;

ClientNetworkService::ClientNetworkService()
	: running_(false)
	, socket_(io_context_,asio::ip::udp::endpoint(asio::ip::udp::v4(),0))
	, connect_timer_(io_context_)
	, kcp_update_timer_(io_context_)
	, type_(ConnectType::kTypeDisconnect)
	, connect_time_(0)
	, conv_(0)
	, kcp_(nullptr)
	, cur_read_(0)
	, cur_write_(0)
{

}

ClientNetworkService::~ClientNetworkService()
{
	Stop();
}

void ClientNetworkService::Run()
{
	if(!running_)
	{
		running_ = true;
		thread_ = std::thread([this]
		{
			UdpReceive();
			TIME_BEGIN_PERIOD(1);
			io_context_.run();
			TIME_END_PERIOD(1);
		});
	}
}

void ClientNetworkService::Stop()
{
	if (running_)
	{
		running_;
		io_context_.stop();

		if (thread_.joinable())
		{
			thread_.join();
		}
	}
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
		KcpMessage message;
		message.set_type(KcpMessage::KcpMessageType::kTypeDisconnect);

		std::vector<uint8_t> data(message.GetByteSize());
		message.Serialize(data.data());

		Send(data.data(), data.size());

		Disconnected();
	}
}

void ClientNetworkService::Send(uint8_t* data, uint32_t len)
{
	if (type_ == ConnectType::kTypeConnected)
	{
		ikcp_send(kcp_, (char*)data, len);
	}
	else
	{
		//todo 输出日志;	
	}
}

bool ClientNetworkService::IsEmpty() const
{
	return cur_read_ == cur_write_;
}

const NetMessage& ClientNetworkService::PopMessage()
{
	if (cur_read_ < cur_write_)
	{
		auto& message = messages_[cur_read_++];
		if (cur_read_ >= messages_.size())
		{
			cur_read_ = 0;
		}
		return message;
	}
}

int ClientNetworkService::UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user)
{
	auto service = (ClientNetworkService*)user;
	service->UdpSend((uint8_t*)buf, len);
	return 0;
}

void ClientNetworkService::Connecting(uint32_t timeout)
{
	auto& msg = MakeReqKcpConnectMsg();
	socket_.send_to(asio::buffer(msg.data(),msg.size()) , server_endpoint_);
	// 定时请求连接udp,直到建立起kcp连接
	connect_timer_.expires_after(milliseconds(100));
	connect_timer_.async_wait([this, timeout](std::error_code ec)
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

void ClientNetworkService::Connected(kcp_conv_t conv)
{
	connect_timer_.cancel();
	type_ = ConnectType::kTypeConnected;
	connect_handler_(ConnectStatus::kTypeConnected);

	conv_ = conv;

	kcp_ = ikcp_create(conv_, this);
	kcp_->output = &ClientNetworkService::UdpOutput;

	// 启动快速模式
	// 第二个参数 nodelay-启用以后若干常规加速将启动
	// 第三个参数 interval为内部处理时钟，默认设置为 10ms
	// 第四个参数 resend为快速重传指标，设置为2
	// 第五个参数 为是否禁用常规流控，这里禁止
	//ikcp_nodelay(kcp_, 1, 10, 2, 1);
	ikcp_nodelay(kcp_, 1, 5, 1, 1); // 设置成1次ACK跨越直接重传, 这样反应速度会更快. 内部时钟5毫秒.

	KcpUpdate();
}

void ClientNetworkService::Disconnected()
{
	type_ == ConnectType::kTypeDisconnect;
	ikcp_release(kcp_);
	kcp_ = nullptr;
	kcp_update_timer_.cancel();
}

void ClientNetworkService::UdpSend(uint8_t* data, uint32_t len)
{
	socket_.send_to(asio::buffer(data, len), server_endpoint_);
}

void ClientNetworkService::UdpReceive()
{
	socket_.async_receive_from(
		asio::buffer(data_.data(), kMaxMsgSize), server_endpoint_,
		[this](std::error_code ec, std::size_t bytes_recvd)
	{
		if (!ec && bytes_recvd > 0)
		{
			if (type_ == ConnectType::kTypeConnecting && IsRspKcpConnectMsg((char*)data_.data(), bytes_recvd))
			{
				Connected(GetKcpConv((char*)data_.data()));
			}
			else
			{
				KcpReceive(data_.data(),bytes_recvd);
			}
		}
		UdpReceive();
	});
}

void ClientNetworkService::KcpReceive(uint8_t* data, uint32_t len)
{
	auto conv = ikcp_getconv(kcp_);
	
	if (conv == conv_)
	{
		// 减去读取的conv信息
		auto ret =  ikcp_recv(kcp_,(char*)data, len - sizeof(kcp_conv_t));
		if (ret)
		{
			messages_[cur_write_].Parse(data_.data(), len);
			++cur_write_;
			if (cur_write_ >= messages_.size())
			{
				cur_write_ = 0;
			}

			//todo: ringbuffer大小不够,客户端卡了,可以考虑直接断线
			if (cur_write_ == cur_read_)
			{
				
			}
		}
	}
	else
	{
		// todo error
	}
}

void ClientNetworkService::KcpUpdate()
{
	if (kcp_ && type_ == ConnectType::kTypeConnected)
	{
		auto cur_clock = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		ikcp_update(kcp_,cur_clock);
	}

	kcp_update_timer_.expires_after(milliseconds(5));
	kcp_update_timer_.async_wait([this](std::error_code ec) 
		{
			if (!ec)
			{
				KcpUpdate();
			}
		});
}
