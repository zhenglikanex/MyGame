#pragma once

#include "Framework/Game/InputService.hpp"

class UnityInputService : public InputService
{
public:
	void InputHandler() const override;
private:
};