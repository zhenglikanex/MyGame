#pragma once

#include <memory>
#include <string>

#include "Framework/Game/Service.hpp"

enum class InputKeyStatus
{
	kDown,
	kUp
};

struct Input
{
	float x_axis;		// “°∏À ‰»Î
	float y_axis;		// “°∏À ‰»Î

	std::unordered_map<std::string, InputKeyStatus> abilities;
};

class InputService : public Service
{
public:
	virtual ~InputService() = 0 {}

	virtual Input GetInput() const = 0;
};