#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/EntityByLocalMap.hpp"
#include "Framework/Game/Component/PhysicsWorld.hpp"
#include "Framework/Game/Component/PhysicsActor.hpp"

#include "Framework/Game/Utility/PhysicsUtility.hpp"

struct PhysicsSystem : public System,public PxSimulationEventCallback
{
	entt::observer kinematic_updater{registry,entt::collector.group<Transform, PhysicsKinematicTag, PhysicsActor>().update<Transform>().where<PhysicsActor, PhysicsKinematicTag>()};

	PhysicsSystem(entt::registry& _registry) 
		: System(_registry)
	{ 

	}

	~PhysicsSystem() {}

	bool Initialize() override
	{
		registry.set<PhysicsWorld>(PhysicsUtility::CreatePhysicsWorld(this,registry));
		
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		UpdateKinematicActor();

		auto& world = registry.ctx<PhysicsWorld>();
		world.scene->simulate(static_cast<float>(dt));
		world.scene->fetchResults(true);

		kinematic_updater.clear();
	}

	void Finalize() override
	{
		PhysicsUtility::DestoryPhysicsWorld(registry.ctx<PhysicsWorld>());
		registry.unset<PhysicsWorld>();
	}

	void UpdateKinematicActor()
	{
		auto view = registry.view<Transform, PhysicsKinematicTag, PhysicsActor>();
		for (auto e : kinematic_updater)
		{
			auto& transform = view.get<Transform>(e);
			auto& physics_actor = view.get<PhysicsActor>(e);
			PhysicsUtility::UpdateActorPose(physics_actor.actor, transform);
		}
	}

	// 连接碰撞，其中必须有一个actor是刚体
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
	{
		//for (PxU32 i = 0; i < nbPairs; i++)
		//{
		//	const PxContactPair& cp = pairs[i];

		//	// 进入碰撞
		//	if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		//	{
		//		for (uint32_t i = 0; i < 2; ++i)
		//		{
		//			auto actor = reinterpret_cast<PhysicsActor*>(pairHeader.actors[i]->userData);
		//		}
		//		//if ((pairHeader.actors[0] == ) || (pairHeader.actors[1] == mSubmarineActor))
		//		{
		//			//PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];
		//			//Seamine* mine = reinterpret_cast<Seamine*>(otherActor->userData);
		//			// insert only once
		//			//if (std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
		//				//mMinesToExplode.push_back(mine);

		//			break;
		//		}
		//	}
		//}
	}

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override
	{
		//auto& map = registry.ctx<EntityByLocalMap>();

		//for (PxU32 i = 0; i < count; ++i)
		//{
		//	auto trigger = (entt::entity)reinterpret_cast<uint32_t>(pairs[i].triggerActor->userData);
		//	auto other = (entt::entity)reinterpret_cast<uint32_t>(pairs[i].otherActor->userData);
		//	
		//	INFO("onTrigger trigger {} other {}", trigger, other);

		//	if (registry.valid(trigger) && registry.valid(other))
		//	{
		//		std::array<std::tuple<entt::entity, entt::entity>, 2> infos = { std::make_tuple(trigger,other),std::make_tuple(other,trigger) };
		//		for (auto info : infos)
		//		{
		//			auto source = std::get<0>(info);
		//			auto target = std::get<1>(info);

		//			if (auto reciver = registry.try_get<CollisionReceiver>(source))
		//			{
		//				auto it = map.value.find(reciver->value);
		//				if (it != map.value.end())
		//				{
		//					entt::entity e = it->second;
		//					if (registry.all_of<GameplayAbilityScript>(e))
		//					{
		//						auto& script = registry.get<GameplayAbilityScript>(e);
		//						script.value["OnTrigger"](script.value,target);
		//					}
		//				}
		//			}
		//		}
		//	}
		//	else
		//	{
		//		INFO("--------------------vaild tigger e1 {}   e2 {}", trigger, other);
		//	}

		//	/*if (registry.valid(tigger_actor->self))
		//	{
		//		auto& contact_list = registry.get<ContactList>(tigger_actor->self);
		//		contact_list.value.emplace_back(other_actor->self);
		//	}*/
		//}
	}

	void onConstraintBreak(PxConstraintInfo*, PxU32) override
	{

	}

	void onWake(PxActor**, PxU32) override
	{

	}

	void onSleep(PxActor**, PxU32) override
	{

	}

	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) 
	{

	}
};