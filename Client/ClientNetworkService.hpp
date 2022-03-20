#pragma once

#include "Framework/Game/NetworkService.hpp"

#include "Framework/Proto/NetMessage.hpp"

class ClientNetworkService : public NetworkService
{
public:
	ClientNetworkService(
		const std::function<std::unique_ptr<NetMessage>(const std::string&)>& recv_handler,
		const std::function<void(std::string_view, std::vector<uint8_t>)>& send_handler);
	~ClientNetworkService();

	std::unique_ptr<std::vector<uint8_t>> Recv(const std::string& name) const override;
	void Send(std::string_view name, std::vector<uint8_t>&& data) const override;
private:
	std::function<std::unique_ptr<NetMessage>(const std::string&)> recv_handler_;
	std::function<void(std::string_view, std::vector<uint8_t>)> send_handler_;
};