#pragma once

#include "Framework/Game/Service/InputService.hpp"

class UnityInputService : public InputService
{
public:
	Input GetInput() const override;
private:
};