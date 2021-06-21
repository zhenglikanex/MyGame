#pragma once

#include <functional>

#include "Framework/Game/InputService.hpp"

class ServerInputService : public InputService
{
public:
	ServerInputService(const std::function<void()>& handler);
	~ServerInputService();

	void InputHandler() const override;
private:
	std::function<void()> input_handler_;
};