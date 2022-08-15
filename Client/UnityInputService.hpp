#pragma once

#include <functional>

#include "Framework/Game/InputService.hpp"

class UnityInputService : public InputService
{
public:
	Input GetInput() const override;
private:
};