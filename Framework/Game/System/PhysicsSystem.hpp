#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/PhysicsWorld.hpp"
#include "Framework/Game/Component/PhysicsActor.hpp"
#include "Framework/Game/Component/Contact.hpp"
#include "Framework/Game/Component/Remote.hpp"
#include "Framework/Game/Component/Skill.hpp"

#include "Framework/Game/Utility/PhysicsUtility.hpp"

struct PhysicsSystem : public ObserverSystem,public PxSimulationEventCallback
{
	entt::observer* static_updater;

	PhysicsSystem(entt::registry& _registry) 
		: ObserverSystem(_registry)
		, static_updater(nullptr)
	{ 

	}

	~PhysicsSystem() {}

	bool Initialize() override
	{
		registry.set<PhysicsWorld>(PhysicsUtility::CreatePhysicsWorld(this));

		static_updater = CreateObserver(entt::collector.group<Transform, PhysicsStaticTag, PhysicsActor>().update<Transform>().where<PhysicsActor, PhysicsStaticTag>());
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		ClearContactList();
		UpdateStaticActor();

		auto& world = registry.ctx<PhysicsWorld>();
		world.scene->simulate(static_cast<float>(dt));
		world.scene->fetchResults(true);
	}

	void Finalize() override
	{
		PhysicsUtility::DestoryPhysicsWorld(registry.ctx<PhysicsWorld>());
		registry.unset<PhysicsWorld>();
	}

	void ClearContactList()
	{
		auto view = registry.view<ContactList>();
		for (auto e : view)
		{
			auto& contact_list = view.get<ContactList>(e);
			contact_list.value.clear();
		}
	}

	void UpdateStaticActor()
	{
		auto view = registry.view<Transform, PhysicsStaticTag, PhysicsActor>();
		for (auto e : *static_updater)
		{
			auto& [transform, physics_actor] = view.get<Transform, PhysicsActor>(e);

			assert(physics_actor.actor != nullptr);

			auto rigid = (PxRigidStatic*)(physics_actor.actor);

			assert(rigid != nullptr);

			rigid->setGlobalPose(PhysicsUtility::Transform(transform));
		}
	}

	// 连接碰撞，其中必须有一个actor是刚体
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
	{
		for (PxU32 i = 0; i < nbPairs; i++)
		{
			const PxContactPair& cp = pairs[i];

			// 进入碰撞
			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				for (uint32_t i = 0; i < 2; ++i)
				{
					auto actor = reinterpret_cast<PhysicsActor*>(pairHeader.actors[i]->userData);
				}
				//if ((pairHeader.actors[0] == ) || (pairHeader.actors[1] == mSubmarineActor))
				{
					//PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];
					//Seamine* mine = reinterpret_cast<Seamine*>(otherActor->userData);
					// insert only once
					//if (std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
						//mMinesToExplode.push_back(mine);

					break;
				}
			}
		}
	}

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override
	{
		for (PxU32 i = 0; i < count; ++i)
		{
			auto trigger = (entt::entity)reinterpret_cast<uint32_t>(pairs[i].triggerActor->userData);
			auto other = (entt::entity)reinterpret_cast<uint32_t>(pairs[i].otherActor->userData);
			
			if (registry.valid(trigger) && registry.valid(other))
			{
				auto& tigger_contacts = registry.get<ContactList>(trigger);
				tigger_contacts.value.emplace_back(other);

				auto& other_contacts = registry.get<ContactList>(other);
				other_contacts.value.emplace_back(trigger);

				INFO("REMOTE {}  {}", registry.has<Remote>(trigger), registry.has<Remote>(other));
				INFO("sKILL {}  {}", registry.has<Skill>(trigger), registry.has<Skill>(other));

				INFO("tigger e1 {}   e2 {}", trigger, other);
			}
			else
			{
				INFO("--------------------vaild tigger e1 {}   e2 {}", trigger, other);
			}

			/*if (registry.valid(tigger_actor->self))
			{
				auto& contact_list = registry.get<ContactList>(tigger_actor->self);
				contact_list.value.emplace_back(other_actor->self);
			}*/
			
		}
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

	void Connect() override
	{

	}

	void Disconnect() override
	{

	}
};