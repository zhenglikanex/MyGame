#pragma once

#include <string_view>

#include "Framework/Game/Component/UniqueId.hpp"
#include "Framework/Game/Component/GAS/GameplayEvent.hpp"
#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"
#include "Framework/Game/Utility/GameplayEventUtility.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct AnimEventUtility
{
	static void SendAnimEvent(Registry& registry, entt::entity e, std::string_view event)
	{
		if (event == "StartInputTask")
		{
			GameplayEventUtility::SendGameplayEvent(registry, registry.get<Local>(e), GameplayTag("GA.StartInputTask"),e);
		}
		else if (event == "OverInputTask")
		{
			GameplayEventUtility::SendGameplayEvent(registry, registry.get<Local>(e), GameplayTag("GA.OverInputTask"),e);
		}
	}
};