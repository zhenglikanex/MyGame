#include "ServerNetworkService.hpp"

#include <cassert>

ServerNetworkService::ServerNetworkService(
	const NetRecvHandler& recv_handler,
	const NetSendHandler& send_handler)
	: recv_handler_(recv_handler)
	, send_handler_(send_handler)
{

}

ServerNetworkService::~ServerNetworkService()
{

}

std::unique_ptr<std::vector<uint8_t>> ServerNetworkService::Recv(const std::string & name) const
{
	if (recv_handler_)
	{
		return recv_handler_(name);
	}

	return nullptr;
}

void ServerNetworkService::Send(std::string_view name, std::vector<uint8_t>&& data) const
{

}

void ServerNetworkService::Send(uint32_t id, std::string_view name, std::vector<uint8_t>&& data) const
{
	if (send_handler_)
	{
		send_handler_(id, name, std::move(data));
	}
}
