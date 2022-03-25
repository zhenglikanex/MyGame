#pragma once

#include "Framework/Game/NetworkService.hpp"

#include "Framework/Proto/NetMessage.hpp"

using NetRecvHandler = std::function<std::unique_ptr<std::vector<uint8_t>>(const std::string&)>;
using NetSendHandler = std::function<void(uint32_t id, std::string_view, std::vector<uint8_t>)>;

class ServerNetworkService : public NetworkService
{
public:
	ServerNetworkService(const NetRecvHandler& recv_handler,
		const NetSendHandler& send_handler);
	~ServerNetworkService();

	std::unique_ptr<std::vector<uint8_t>> Recv(const std::string& name) const override;
	void Send(std::string_view name, std::vector<uint8_t>&& data) const override;
	void Send(uint32_t id, std::string_view name, std::vector<uint8_t>&& data) const;
private:
	NetRecvHandler recv_handler_;
	NetSendHandler send_handler_;
};