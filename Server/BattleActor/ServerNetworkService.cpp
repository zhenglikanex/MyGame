#include "ServerNetworkService.hpp"

#include <cassert>

ServerNetworkService::ServerNetworkService(
	const std::function<std::unique_ptr<std::tuple<std::string, std::vector<uint8_t>>>()>& recv_handler,
	const std::function<void(std::string_view, std::vector<uint8_t>)>& send_handler)
	: recv_handler_(recv_handler)
	, send_handler_(send_handler)
{

}

ServerNetworkService::~ServerNetworkService()
{

}

std::unique_ptr<std::tuple<std::string, std::vector<uint8_t>>> ServerNetworkService::Recv() const
{
	if (recv_handler_)
	{
		return recv_handler_();
	}

	return nullptr; 
}

void ServerNetworkService::Send(std::string_view name, std::vector<uint8_t>&& data) const
{
	if (send_handler_)
	{
		send_handler_(name, std::move(data));
	}
}