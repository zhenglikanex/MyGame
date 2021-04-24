#include "ClientNetworkService.hpp"

#include <chrono>

using namespace std::chrono;

ClientNetworkService::ClientNetworkService()
	: alloc_session_(0)
{
	network_.set_connect_handler([this](ClientNetwork::ConnectStatus code)
		{
			
		});

	network_.Run();
}

bool ClientNetworkService::Connect(const std::string& ip, uint16_t port, uint32_t timeout)
{	
	network_.Connect(ip, port, timeout);
	// 等待异步连接完成
	while (network_.type() == ClientNetwork::ConnectType::kTypeConnecting)
	{
		std::this_thread::sleep_for(milliseconds(10));
	}

	return network_.IsConnected();
}

void ClientNetworkService::Disconnect()
{
	network_.Disconnect();
	alloc_session_ = 0;
	rpc_handlers_.clear();
}

bool ClientNetworkService::IsConnected() const
{
	return network_.IsConnected();
}

void ClientNetworkService::Request(std::string_view name, std::vector<uint8_t>&& data, 
	const std::function<void(const std::vector<uint8_t>& data)>& callback/*= nullptr*/)
{
	// RPC
	++alloc_session_;
	NetMessage message;
	message.set_name(name);
	message.set_session(alloc_session_);
	message.set_data(std::move(data));
	
	rpc_handlers_.emplace(message.session(),callback);

	std::vector<uint8_t> buffer(message.GetByteSize());
	message.Serialize(buffer.data());

	network_.Send(std::move(data));
}

void ClientNetworkService::Send(std::string_view name, std::vector<uint8_t>&& data)
{
	NetMessage message;
	message.set_name(name);
	message.set_session(0);
	message.set_data(std::move(data));
	
	std::vector<uint8_t> buffer(message.GetByteSize());
	message.Serialize(buffer.data());

	network_.Send(std::move(data));
}

void ClientNetworkService::Update()
{
	while (auto buffer = network_.PopBuffer())
	{
		NetMessage message;
		message.Parse(buffer->data(), buffer->size());
		if (message.session() != 0) // RPC
		{
			auto iter = rpc_handlers_.find(message.session());
			if (iter != rpc_handlers_.end())
			{
				auto& func = iter->second;
				func(message.data());
			}
		}
		else
		{
			// push
			if (message_handler_)
			{
				message_handler_(message);
			}
		}
	}
}