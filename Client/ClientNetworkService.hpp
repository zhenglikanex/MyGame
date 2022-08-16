#pragma once

#include <functional>

#include "Framework/Game/NetworkService.hpp"

#include "Framework/Proto/NetMessage.hpp"

class ClientNetworkService : public NetworkService
{
public:
	ClientNetworkService(
		const std::function<std::optional<NetMessage>(const std::string&)>& recv_handler,
		const std::function<void(std::string_view, std::vector<uint8_t>)>& send_handler);

	std::optional<std::vector<uint8_t>> Recv(const std::string& name) const override;
	void Send(std::string_view name, std::vector<uint8_t>&& data) const override;
private:
	std::function<std::optional<NetMessage>(const std::string&)> recv_handler_;
	std::function<void(std::string_view, std::vector<uint8_t>)> send_handler_;
};