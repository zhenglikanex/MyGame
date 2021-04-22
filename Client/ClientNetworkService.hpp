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

	void Request(std::string_view name,std::vector<uint8_t>&& data,
		const std::function<void(std::vector<uint8_t>&& data)> & = nullptr) override;

	void Send(std::string_view name, std::vector<uint8_t>&& data,
		const std::function<void(std::vector<uint8_t>&& data)> & = nullptr) override;

	void Update();

	void set_messge_handler(const std::function<void(const NetMessage&)>& handler)
	{
		message_handler_ = handler;
	}
private:
	uint16_t GenSession();

	ClientNetwork network_;
	std::atomic<uint16_t> alloc_session_;
	std::unordered_map<uint16_t, std::function<void(const std::vector<uint8_t>&)>> rpc_handlers_;
	std::function<void(const NetMessage&)> message_handler_;
};