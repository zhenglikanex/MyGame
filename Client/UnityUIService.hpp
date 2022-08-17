#pragma once

#include <cstdint>

#include "Framework/Game/UIService.hpp"

class UnityUIService : public UIService
{
public:
	void RefreshUI(uint32_t hp, uint32_t max_hp, uint32_t mp, uint32_t max_mp) const override;
	void RefreshAbilityCooldown(std::string_view json_info) const override;
	void HpFlyingText(float x,float y,float z, int32_t value) const override;
};