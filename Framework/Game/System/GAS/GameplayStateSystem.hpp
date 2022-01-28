#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

struct GameplayStateSystem : public System
{
	GameplayStateSystem(entt::registry& _registry)
		: System(_registry)
	{

	}

	~GameplayStateSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto run_frame = registry.ctx<GameState>().run_frame;
		auto view = registry.view<GameplayState,GameplayAttributeSet>();
		for (auto e : view)
		{
			auto& [gameplay_state,attribute_set] = view.get<GameplayState,GameplayAttributeSet>(e);
			gameplay_state.attribute_sets[run_frame % gameplay_state.attribute_sets.size()] = attribute_set;
			
			auto tags = registry.try_get<GameplayTags>(e);
			if (tags)
			{
				gameplay_state.tags[run_frame % gameplay_state.tags.size()] = *tags;
			}
		}
	}

	void Finalize() override
	{

	}
};