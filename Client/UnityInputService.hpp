#pragma once

#include <functional>

#include "Framework/Game/InputService.hpp"

class UnityInputService : public InputService
{
public:
	UnityInputService(const std::function<void>& input_handler);

	Input GetInput() const override;
private:
	std::function<void> input_handler_;
};