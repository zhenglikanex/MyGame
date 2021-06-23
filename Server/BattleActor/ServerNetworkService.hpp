#pragma once

#include "Framework/Game/NetworkService.hpp"

#include "Framework/Proto/NetMessage.hpp"

class ServerNetworkService : public NetworkService
{
public:
	ServerNetworkService(const std::function<std::unique_ptr<NetMessage>()>& recv_handler,
		const std::function<void(std::string_view, std::vector<uint8_t>)>& send_handler);
	~ServerNetworkService();

	std::unique_ptr<std::tuple<std::string, std::vector<uint8_t>>> Recv() const override;
	void Send(std::string_view name, std::vector<uint8_t>&& data) const override;
private:
	std::function<std::unique_ptr<NetMessage>()> recv_handler_;
	std::function<void(std::string_view, std::vector<uint8_t>)> send_handler_;
};