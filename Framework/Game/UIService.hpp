#pragma once

#include "Framework/Game/Service.hpp"

#include <cstdint>

class UIService : public Service
{
public:
	virtual ~UIService() = 0 {}

	virtual void RefreshUI(uint32_t hp,uint32_t max_hp,uint32_t mp,uint32_t max_mp) const = 0;
};