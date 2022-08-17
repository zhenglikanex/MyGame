#pragma once

#include <string_view>

#include "Framework/Game/Component/Remote.hpp"
#include "Framework/Game/Component/GAS/GameplayEvent.hpp"
#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"
#include "Framework/Game/Utility/GameplayEventUtility.hpp"

#include "3rdparty/include/entt/entt.hpp"

namespace anim_event_utility
{
	static void SendAnimEvent(Registry& registry, entt::entity e, std::string_view event)
	{
		if (event == "StartInputTask")
		{
			//todo
			//gameplay_event_utility::SendGameplayEvent(registry, e, GameplayTag("GA.StartInputTask"),e);
		}
		else if (event == "OverInputTask")
		{
			//todo
			//gameplay_event_utility::SendGameplayEvent(registry, e, GameplayTag("GA.OverInputTask"),e);
		}
	}
};