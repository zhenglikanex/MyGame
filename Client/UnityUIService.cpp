#include "UnityUIService.hpp"

#include "Client/UnityBridge.hpp"

void UnityUIService::RefreshUI(uint32_t hp, uint32_t max_hp, uint32_t mp, uint32_t max_mp) const
{
	UnityBridge::Get().CallUnity<void>("RefreshUI", hp, max_hp, mp, max_mp);
}

void UnityUIService::RefreshAbilityCooldown(std::string_view json_info) const
{
	UnityBridge::Get().CallUnity<void>("RefreshAbilityCooldown", json_info.data());
}

void UnityUIService::HpFlyingText(float x, float y, float z, int32_t value) const
{
	UnityBridge::Get().CallUnity<void>("HpFlyingText",x,y,z,value);
}
