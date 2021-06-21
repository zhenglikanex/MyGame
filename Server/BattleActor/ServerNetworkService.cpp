#include "ServerNetworkService.hpp"

#include <cassert>

ServerNetworkService::ServerNetworkService(const std::function<void(std::string_view, std::vector<uint8_t>)>& handler)
{

}

ServerNetworkService::~ServerNetworkService()
{

}

void ServerNetworkService::Request(std::string_view name, std::vector<uint8_t>&& data, const std::function<void(const std::vector<uint8_t>& data)> & /*= nullptr*/)
{
	assert(false && "not support request");
}

void ServerNetworkService::Send(std::string_view name, std::vector<uint8_t>&& data)
{
	if (send_handler_)
	{
		send_handler_(name, std::move(data));
	}
}