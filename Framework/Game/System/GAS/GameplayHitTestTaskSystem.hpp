#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/GameHelper.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayHitTestTask.hpp"

#include "Framework/Game/Utility/PhysicsUtility.hpp"
#include "Framework/Game/Utility/RollbackUtility.hpp"
#include "Framework/Game/Utility/PhysicsUtility.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"


struct GameplayHitTestTaskSystem : System
{
public:
	std::vector<entt::entity> hits;

	GameplayHitTestTaskSystem(entt::registry& _registry) : System(_registry)
	{
	}

	~GameplayHitTestTaskSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		// todo:考虑是否要命中的判断放到一起处理
		// 放到一起后可以按照要回滚的帧数排序来提高效率?
		// 而且在查询阶段cache的效率会更高?
		// 带来的时同步和预测回滚、复制的开销？
		float time = static_cast<float>(dt);

		auto& game_helper = registry.ctx<Locator>().Ref<GameHelper>();
		auto& game_state = registry.ctx<GameState>();
		auto& map = registry.ctx<EntityByUniqueIdMap>();

		auto view = registry.view<GameplayAbilitySpec, GameplayAbilityScript, GameplayHitTestTask>();
		for (auto e : view)
		{
			auto& [spec, script, task] = registry.get<GameplayAbilitySpec, GameplayAbilityScript, GameplayHitTestTask>(e);
			auto it = map.value.find(spec.source);
			if (it != map.value.end())
			{
				entt::entity source = it->second;
				auto& command = registry.get<Command>(source);

				bool rollback = game_helper.IsAuthority() && RollbackUtility::CheckRollback(registry, command.verified_frame);
				if (rollback)
				{
					RollbackUtility::RollbackPhysicActor(registry, command.verified_frame);
				}

				for (auto& spec : task.value)
				{
					hits.clear();

					switch (spec.type)
					{
					case GameplayHitType::kRay:
						break;
					case GameplayHitType::kOverlap:
					{
						auto& geometry = spec.collider_info.geometry;
						auto transform = PhysicsUtility::Transform(spec.transform) * PhysicsUtility::Transform(spec.collider_info.transform);
						if (geometry.type() == GeometryType::kSphere)
						{
							PhysicsUtility::CheckSphereOverlap(registry, PxSphereGeometry(geometry.sphere().r), transform, hits);
						}
						else if (spec.collider_info.geometry.type() == GeometryType::kBox)
						{
							PhysicsUtility::CheckBoxOverlap(registry, PxBoxGeometry(geometry.box().e.x, geometry.box().e.y, geometry.box().e.z), transform, hits);
						}
						else
						{
							assert(false && "not support!");
						}
					}
						break;
					case GameplayHitType::kSweep:
						break;
					default:
						break;

					}

					for (auto hit : hits)
					{
						if (spec.on_hit != "")
						{
							script.value[spec.on_hit](script.value,hits);
						}
					}
				}

				if (rollback)
				{
					RollbackUtility::RollbackPhysicActor(registry,game_state.run_frame);
				}
				
				
			}

			task.value.clear();
		}
	}

	void Finalize() override
	{

	}
};