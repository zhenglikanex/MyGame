#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

struct GameplayStateSystem : public System
{
	GameplayStateSystem(Registry& _registry)
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
			
			auto& tag_count = registry.get<GameplayTagCountContainer>(e);
			gameplay_state.tag_counts[run_frame % gameplay_state.tag_counts.size()] = tag_count;
		}
	}

	void Finalize() override
	{

	}
};