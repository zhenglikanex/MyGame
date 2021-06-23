#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "ClientNetworkImpl.hpp"

class ClientNetwork
{
public:
	ClientNetwork();
	~ClientNetwork();

	bool Connect(const std::string& ip, uint16_t port, uint32_t timeout);
	void Disconnect();

	bool IsConnected() const;

	void Request(std::string_view name,std::vector<uint8_t>&& data,
		const std::function<void(const std::vector<uint8_t>& data)>& callback = nullptr);

	void Send(std::string_view name, std::vector<uint8_t>&& data);

	void Update();

	void set_messge_handler(const std::function<void(NetMessage&&)>& handler)
	{
		message_handler_ = handler;
	}

	ClientNetworkImpl::ConnectErrorCode GetLastError() const
	{
		return error_code_;
	}
private:
	ClientNetworkImpl network_;
	ClientNetworkImpl::ConnectErrorCode error_code_;
	uint16_t alloc_session_;
	std::unordered_map<uint16_t, std::function<void(const std::vector<uint8_t>&)>> rpc_handlers_;
	std::function<void(NetMessage&&)> message_handler_;
};