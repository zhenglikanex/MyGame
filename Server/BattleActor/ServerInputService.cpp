#include "ServerInputService.hpp"

ServerInputService::ServerInputService(const std::function<void()>& handler)
	:input_handler_(handler)
{
}

ServerInputService::~ServerInputService()
{

}

void ServerInputService::InputHandler() const
{
	if (input_handler_)
	{
		input_handler_();
	}
}