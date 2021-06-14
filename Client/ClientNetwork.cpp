#include "ClientNetwork.hpp"

#ifdef _WIN32

#include <windows.h>
#include <mmsystem.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

#define TIME_BEGIN_PERIOD(ms) ::timeBeginPeriod(ms)
#define TIME_END_PERIOD(ms) ::timeEndPeriod(ms)

#else

#define TIME_BEGIN_PERIOD(ms) 
#define TIME_END_PERIOD(ms)

#endif // Windows


using namespace std::chrono;

ClientNetwork::ClientNetwork()
	: running_(false)
	, socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0))
	, connect_timer_(io_context_)
	, kcp_update_timer_(io_context_)
	, data_(std::array<uint8_t, kMaxMsgSize>{0})
	, type_(ConnectType::kTypeDisconnect)
	, connect_time_(0)
	, conv_(0)
	, kcp_(nullptr)
	, cur_clock_(0)
	, cur_read_(0)
	, cur_write_(0)
{

}

ClientNetwork::~ClientNetwork()
{
	if (IsRunning())
	{
		Stop();
	}
}

void ClientNetwork::Run()
{
	thread_ = std::thread([this]
	{
		running_ = true;
		UdpReceive();
		TIME_BEGIN_PERIOD(1);
		io_context_.run();
		TIME_END_PERIOD(1);
		running_ = false;
	});
}

void ClientNetwork::Stop()
{
	socket_.close();
	connect_timer_.cancel();
	kcp_update_timer_.cancel();

	if (thread_.joinable())
	{
		thread_.join();
	}

	Disconnect();
}

bool ClientNetwork::Connect(const std::string& ip, uint16_t port, uint32_t timeout)
{
	connect_time_ = duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();

	server_endpoint_ = asio::ip::udp::endpoint(asio::ip::address::from_string(ip), port);
	type_ = ConnectType::kTypeConnecting;
	Connecting(timeout);

	while (type_ == kTypeConnecting)
	{
		std::this_thread::sleep_for(milliseconds(50));
	}

	return IsConnected();
}

void ClientNetwork::Disconnect()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (IsConnected())
	{
		type_ = ConnectType::kTypeDisconnect;
		connect_time_ = 0;
		conv_ = 0;

		if (kcp_)
		{
			ikcp_release(kcp_);
			kcp_ = nullptr;
		}

		cur_clock_ = 0;
		cur_read_ = 0;
		cur_write_ = 0;
	}
}

bool ClientNetwork::IsConnected() const
{
	return type_ == ConnectType::kTypeConnected;
}

void ClientNetwork::Send(std::vector<uint8_t>&& data)
{
	auto buffer = std::make_shared<std::vector<uint8_t>>(std::move(data));
	io_context_.post([this, buffer]()
		{
			if (IsConnected())
			{
				auto ret = ikcp_send(kcp_, (char*)buffer->data(), buffer->size());
			}
			else
			{
				//todo 输出日志;	
			}
		});
}

void ClientNetwork::Send(uint8_t* data, uint32_t len)
{
	auto buffer = std::make_shared<std::vector<uint8_t>>(len);
	std::copy_n(data, len, buffer->data());
	io_context_.post([this, buffer]()
		{
			if (IsConnected())
			{
				auto ret = ikcp_send(kcp_, (char*)buffer->data(), buffer->size());
				ikcp_flush(kcp_);
			}
			else
			{
				//todo 输出日志;	
			}
		});
}

bool ClientNetwork::IsEmpty() const
{
	return cur_read_ == cur_write_;
}

std::unique_ptr<std::vector<uint8_t>> ClientNetwork::PopBuffer()
{
	if (IsConnected())
	{
		if (!IsEmpty())
		{
			auto buffer = std::make_unique<std::vector<uint8_t>>(std::move(buffers_[cur_read_]));
			auto next = cur_read_ + 1;
			cur_read_ = next % buffers_.size();
			return buffer;
		}
	}

	return nullptr;
}

int ClientNetwork::UdpOutput(const char* buf, int len, ikcpcb* kcp, void* user)
{
	auto service = (ClientNetwork*)user;
	service->UdpSend((uint8_t*)buf, len);
	return 0;
}

void ClientNetwork::Connecting(uint32_t timeout)
{
	auto& msg = MakeReqKcpConnectMsg();
	socket_.send_to(asio::buffer(msg.data(), msg.size()), server_endpoint_);
	// 定时请求连接udp,直到建立起kcp连接
	connect_timer_.expires_after(milliseconds(100));
	connect_timer_.async_wait([this, timeout](std::error_code ec)
		{
			auto str = ec.message();
			if (!ec)
			{
				if (type_ == ConnectType::kTypeConnecting)
				{
					auto now = duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();
					if (now - connect_time_ >= timeout)
					{
						Timeout();
						return;
					}

					Connecting(timeout);
				}
			}
		});
}

void ClientNetwork::Connected(kcp_conv_t conv)
{
	connect_timer_.cancel();

	conv_ = conv;

	kcp_ = ikcp_create(conv_, this);
	kcp_->output = &ClientNetwork::UdpOutput;

	// 启动快速模式
	// 第二个参数 nodelay-启用以后若干常规加速将启动
	// 第三个参数 interval为内部处理时钟，默认设置为 10ms
	// 第四个参数 resend为快速重传指标，设置为2
	// 第五个参数 为是否禁用常规流控，这里禁止
	//ikcp_nodelay(kcp_, 1, 10, 2, 1);
	ikcp_nodelay(kcp_, 1, 10, 1, 1); // 设置成1次ACK跨越直接重传, 这样反应速度会更快. 内部时钟5毫秒.
	ikcp_wndsize(kcp_, 1024, 1024);
	kcp_->interval = 1;
	kcp_->rx_minrto = 5;

	type_ = ConnectType::kTypeConnected;

	SendConnectedMsg();

	kcp_update_timer_.expires_at(steady_clock::now());
	KcpUpdate();
}

void ClientNetwork::Timeout()
{
	type_ = ConnectType::kTypeDisconnect;
	connect_timer_.cancel();
}

void ClientNetwork::SendConnectedMsg()
{
	// 发送连接上的消息
	auto msg = std::make_shared<std::string>(MakeKcpConnectedMsg());
	ikcp_send(kcp_, (char*)msg->data(), msg->size());
}

void ClientNetwork::UdpSend(uint8_t* data, uint32_t len)
{
	auto buffer_ptr = std::make_shared<std::string>((char*)data, len);
	socket_.async_send_to(
		asio::buffer(buffer_ptr->data(), buffer_ptr->size()), server_endpoint_,
		[buffer_ptr](asio::error_code, std::size_t) // 保持buffer生命周期到发送结束
		{

		}
	);
}

void ClientNetwork::UdpReceive()
{
	socket_.async_receive_from(
		asio::buffer(data_.data(), kMaxMsgSize), server_endpoint_,
		[this](std::error_code ec, std::size_t bytes_recvd)
		{
			auto str = ec.message();
			if (!ec)
			{
				if (bytes_recvd > 0)
				{
					if (type_ == ConnectType::kTypeConnecting && IsRspKcpConnectMsg((char*)data_.data(), bytes_recvd))
					{
						Connected(GetKcpConv((char*)data_.data()));
					}
					else if (type_ == ConnectType::kTypeConnected)
					{
						if (IsKcpDisconnectMsg((char*)data_.data(), bytes_recvd))
						{
							if (connect_handler_)
							{
								connect_handler_(ConnectErrorCode::kTypeServerDisconnect);
							}
							Disconnect();
							return;
						}

						KcpReceive(data_.data(), bytes_recvd);
					}
				}
			}
			if (ec.value() != 995)
			{
				UdpReceive();
			}
		});
}

void ClientNetwork::KcpReceive(uint8_t* data, uint32_t len)
{
	auto conv = ikcp_getconv(kcp_);
	if (conv == conv_)
	{
		ikcp_input(kcp_, (char*)data, len);
		while (true)
		{
			auto recv_bytes = ikcp_recv(kcp_, (char*)data, len);
			if (recv_bytes > 0)
			{
				// 客户端卡了，直接掉线
				auto next = cur_write_ + 1;
				if (next % buffers_.size() == cur_read_)
				{
					if (connect_handler_)
					{
						connect_handler_(ConnectErrorCode::kTypeServerTimeout);
					}

					Disconnect();
					return;
				}

				buffers_[cur_write_].resize(recv_bytes);
				std::copy_n(data, recv_bytes, buffers_[cur_write_].data());
				cur_write_ = next % buffers_.size();
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		// todo error
	}
}

void ClientNetwork::KcpUpdate()
{
	if (kcp_ && IsConnected())
	{
		cur_clock_ = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		ikcp_update(kcp_, cur_clock_);

		kcp_update_timer_.expires_at(kcp_update_timer_.expires_at() + milliseconds(1));
		kcp_update_timer_.async_wait([this](std::error_code ec)
			{
				if (!ec)
				{
					KcpUpdate();
				}
			});
	}
}