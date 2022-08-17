#pragma once

#include "Framework/Game/Service/Service.hpp"

#include <cstdint>
#include <string_view>

class UIService : public Service
{
public:
	virtual void RefreshUI(uint32_t hp,uint32_t max_hp,uint32_t mp,uint32_t max_mp) const = 0;
	virtual void RefreshAbilityCooldown(std::string_view json_info) const = 0;
	virtual void HpFlyingText(float x, float y, float z, int32_t value) const = 0;
};