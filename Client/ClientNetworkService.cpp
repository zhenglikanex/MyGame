#include "ClientNetworkService.hpp"

ClientNetworkService::ClientNetworkService(
	const std::function<std::unique_ptr<NetMessage>(const std::string&)>& recv_handler,
	const std::function<void(std::string_view, std::vector<uint8_t>)>& send_handler)
	: recv_handler_(recv_handler)
	, send_handler_(send_handler)
{

}

ClientNetworkService::~ClientNetworkService()
{

}

std::unique_ptr<std::vector<uint8_t>> ClientNetworkService::Recv(const std::string& name) const
{
	if (recv_handler_)
	{
		auto message = recv_handler_(name);
		if (message)
		{
			return std::make_unique<std::vector<uint8_t>>(message->MoveData());
		}
	}

	return nullptr;
}

void ClientNetworkService::Send(std::string_view name, std::vector<uint8_t>&& data) const
{
	if (send_handler_)
	{
		send_handler_(name, std::move(data));
	}
}

