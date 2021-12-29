#pragma once

#include <cstdint>

class UIService
{
public:
	virtual ~UIService() = 0 {}

	virtual void RefreshUI(uint32_t hp,uint32_t max_hp,uint32_t mp,uint32_t max_mp) const;
};