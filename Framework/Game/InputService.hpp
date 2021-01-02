#pragma once

#include <memory>
#include <string>
#include "Framework/Game/Service.hpp"

class IViewImpl;

class InputService : public Service
{
public:
	virtual ~InputService() = 0 {}

	virtual void InputHandler() const = 0;
private:
};