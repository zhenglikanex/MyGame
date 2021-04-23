#include "ClientNetworkService.hpp"

ClientNetworkService::ClientNetworkService()
	: alloc_session_(0)
{

}


void ClientNetworkService::Request(std::string_view name, std::vector<uint8_t>&& data, 
	const std::function<void(std::vector<uint8_t>&& data)>& callback/*= nullptr*/)
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
		if (message.session() != 0) // rpc
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