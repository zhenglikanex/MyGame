#pragma once

#include <functional>

#include "Framework/Game/InputService.hpp"

class ServerInputService : public InputService
{
public:
	void InputHandler() const override;

	void set_input_handler(const std::function<void()>& hanlder)
	{
		input_handler_ = hanlder;
	}
private:
	std::function<void()> input_handler_;
};