#include "ServerInputService.hpp"

void ServerInputService::InputHandler() const
{
	if (input_handler_)
	{
		input_handler_();
	}
}