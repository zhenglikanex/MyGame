#pragma once

#include <functional>

#include "Framework/Game/Service/NetworkService.hpp"

using NetRecvHandler = std::function<std::optional<std::vector<uint8_t>>(const std::string&)>;
using NetSendHandler = std::function<void(uint32_t id, std::string_view, std::vector<uint8_t>)>;

class ServerNetworkService : public NetworkService
{
public:
	ServerNetworkService(const NetRecvHandler& recv_handler,
		const NetSendHandler& send_handler);

	std::optional<std::vector<uint8_t>> Recv(const std::string& name) const override;
	void Send(std::string_view name, std::vector<uint8_t>&& data) const override;
	void Send(uint32_t id, std::string_view name, std::vector<uint8_t>&& data) const;
private:
	NetRecvHandler recv_handler_;
	NetSendHandler send_handler_;
};