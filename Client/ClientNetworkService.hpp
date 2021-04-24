#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "ClientNetwork.hpp"

#include "Framework/Game/NetworkService.hpp"

class ClientNetworkService : public NetworkService
{
public:
	ClientNetworkService();

	bool Connect(const std::string& ip, uint16_t port, uint32_t timeout);
	void Disconnect();

	bool IsConnected() const;

	void Request(std::string_view name,std::vector<uint8_t>&& data,
		const std::function<void(const std::vector<uint8_t>& data)>& callback = nullptr) override;

	void Send(std::string_view name, std::vector<uint8_t>&& data) override;

	void Update();

	void set_messge_handler(const std::function<void(const NetMessage&)>& handler)
	{
		message_handler_ = handler;
	}
private:
	ClientNetwork network_;
	uint16_t alloc_session_;
	std::unordered_map<uint16_t, std::function<void(const std::vector<uint8_t>&)>> rpc_handlers_;
	std::function<void(const NetMessage&)> message_handler_;
};