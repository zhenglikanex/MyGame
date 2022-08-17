#include "ClientNetworkService.hpp"

ClientNetworkService::ClientNetworkService(
	std::function<std::optional<NetMessage>(const std::string&)> recv_handler,
	std::function<void(std::string_view, std::vector<uint8_t>)> send_handler)
	: recv_handler_(std::move(recv_handler))
	, send_handler_(std::move(send_handler))
{

}

std::optional<std::vector<uint8_t>> ClientNetworkService::Recv(const std::string& name) const
{
	if (recv_handler_)
	{
		if (auto message = recv_handler_(name))
		{
			return message->MoveData();
		}
	}

	return {};
}

void ClientNetworkService::Send(std::string_view name, std::vector<uint8_t>&& data) const
{
	if (send_handler_)
	{
		send_handler_(name, std::move(data));
	}
}

