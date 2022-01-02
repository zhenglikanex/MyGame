#pragma once

#include "Framework/Game/RandomEngine.hpp"
#include "Framework/Game/GameHelper.hpp"
#include "Framework/Game/Locator.hpp"

#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/Component/LocalId.hpp"
#include "Framework/Game/Component/UniqueId.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/PlayerManager.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"
#include "Framework/Game/Component/GAS/TryGameplayAbility.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/System.hpp"

struct TryGameplayAbilitySystem : public System
{
	TryGameplayAbilitySystem(entt::registry& _registry) : System(_registry) { }
	~TryGameplayAbilitySystem() {}

	bool Initialize() override
	{
		return true;
	}

	bool CheckCoolDown(const GameplayAbility& ga)
	{
		return true;
	}

	bool CheckCost(const GameplayAbility& ga)
	{
		return true;
	}

	// 检查是否会被其他技能阻塞
	bool CheckOtherBlock(const GameplayAbility& ga,const std::vector<entt::entity>& entities)
	{
		for (auto e : entities)
		{
			auto& spec = registry.get<GameplayAbilitySpec>(e);
			
		}
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto& game_helper = registry.ctx<Locator>().Ref<GameHelper>();
		auto& ga_manager = registry.ctx<GameplayAbilityManager>();

		if (auto try_ga = registry.try_ctx<TryGameplayAbility>())
		{
			for (auto& ga_list : try_ga->value)
			{
				for (auto& info : ga_list.second)
				{
					if (!registry.valid(info.source))
					{
						continue;
					}

					if (!(registry.has<UniqueId>(info.source)) || !(registry.has<GameplayState>(info.source)))
					{
						continue;
					}

					auto& ga = ga_manager.LoadGameplayAbility(info.ability);
					
					auto source_tags = registry.try_get<GameplayTags>(info.source);
					//auto target_tags = registry.try_get<GameplayTags>(info.target);

					if (!CheckCoolDown(ga))
					{
						continue;
					}

					if (!CheckCost(ga))
					{
						continue;
					}

					// todo:其他检查
					auto e = registry.create();
					if (game_helper.IsAuthority())
					{
						registry.emplace<UniqueId>(e, e);
					}
					registry.emplace<GameplayAbilitySpec>(e, info.ability, 1, registry.get<UniqueId>(info.source),UniqueId(entt::entity(entt::null)));
					if (auto player = registry.try_get<Player>(info.source))
					{
						auto& player_manager = registry.ctx<PlayerManager>();
						registry.emplace<LocalId>(e,player_manager.players[player->id].local_id_generator.value++);
						registry.emplace<Player>(e, *player);
					}
				}

				ga_list.second.clear();
			}
		}
	}
	

	void Finalize() override
	{

	}
};
