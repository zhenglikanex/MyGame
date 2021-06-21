#pragma once

#include "Framework/Game/NetworkService.hpp"

class ServerNetworkService : public NetworkService
{
public:
	ServerNetworkService(const std::function<void(std::string_view, std::vector<uint8_t>)>& handler);
	~ServerNetworkService();

	void Request(std::string_view name, std::vector<uint8_t>&& data,
		const std::function<void(const std::vector<uint8_t>& data)> & = nullptr) override;

	void Send(std::string_view name, std::vector<uint8_t>&& data) override;
private:
	std::function<void(std::string_view, std::vector<uint8_t>)> send_handler_;
};