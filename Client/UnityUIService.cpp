#include "UnityUIService.hpp"

#include "Client/UnityBridge.hpp"

UnityUIService::UnityUIService()
{
}

UnityUIService::~UnityUIService()
{

}

void UnityUIService::RefreshUI(uint32_t hp, uint32_t max_hp, uint32_t mp, uint32_t max_mp) const
{
	UnityBridge::Get().CallUnity<void>("RefreshUI", hp, max_hp, mp, max_mp);
}
