#pragma once

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayHitTestTask.hpp"

#include "Framework/Game/Utility/PhysicsUtility.hpp"
#include "Framework/Game/Utility/RollbackUtility.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"


struct GameplayHitTestTaskSystem : System
{
public:
	std::vector<entt::entity> hits;

	GameplayHitTestTaskSystem(Registry& _registry) : System(_registry)
	{
	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		// todo:考虑是否要命中的判断放到一起处理
		// 放到一起后可以按照要回滚的帧数排序来提高效率?
		// 而且在查询阶段cache的效率会更高?
		// 带来的时同步和预测回滚、复制的开销？
		auto& game_state = registry.ctx<GameState>();

		auto view = registry.view<GameplayAbilitySpec, GameplayAbilityScript, GameplayHitTestTask>();
		for (auto e : view)
		{
			auto& [spec, script, task] = registry.get<GameplayAbilitySpec, GameplayAbilityScript, GameplayHitTestTask>(e);
			if (registry.valid(spec.source))
			{
				bool rollback = false;

				if (rollback_utility::CheckRollback(registry, 0))
				{
					rollback = true;
					rollback_utility::RollbackPhysicActor(registry, 0);
				}

				for (auto& gameplay_hit_test_spec : task.value)
				{
					hits.clear();

					switch (gameplay_hit_test_spec.type)
					{
					case GameplayHitType::kRay:
						break;

					case GameplayHitType::kOverlap:
					{
						auto& geometry = gameplay_hit_test_spec.collider.geometry;
						auto transform = physics_utility::Transform(gameplay_hit_test_spec.transform) * physics_utility::Transform(gameplay_hit_test_spec.collider.transform);
						if (geometry.type() == GeometryType::kSphere)
						{
							physics_utility::CheckSphereOverlap(registry, PxSphereGeometry(geometry.sphere().r), transform, hits);
						}
						else if (gameplay_hit_test_spec.collider.geometry.type() == GeometryType::kBox)
						{
							physics_utility::CheckBoxOverlap(registry, PxBoxGeometry(geometry.box().e.x, geometry.box().e.y, geometry.box().e.z), transform, hits);
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

					if (gameplay_hit_test_spec.on_hit != "")
					{
						script.value[gameplay_hit_test_spec.on_hit](script.value,hits);
					}
				}

				if (rollback)
				{
					rollback_utility::RollbackPhysicActor(registry,game_state.run_frame);
				}
			}

			task.value.clear();
		}
	}

	void Finalize() override
	{

	}
};