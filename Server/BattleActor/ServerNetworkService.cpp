#include "ServerNetworkService.hpp"

#include <cassert>

ServerNetworkService::ServerNetworkService(
	const std::function<std::unique_ptr<NetMessage>()>& recv_handler, 
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
		auto message = recv_handler_();
		if (message)
		{
			// todo:暂时这么写一下，还没找到合适的结构
			return std::make_unique<std::tuple<std::string, std::vector<uint8_t>>>(std::make_tuple(message->name(), message->data()));
		}
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