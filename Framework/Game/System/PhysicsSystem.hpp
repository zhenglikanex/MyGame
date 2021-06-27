#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/PhysicsWorld.hpp"

struct PhysicsSystem : public System ,public PxSimulationEventCallback
{
	PhysicsSystem(entt::registry& _registry) : System(_registry) { }
	~PhysicsSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto& world = registry.ctx<PhysicsWorld>();
		world.scene->simulate(static_cast<float>(dt));
		world.scene->fetchResults(true);

		
	}

	void Finalize() override
	{

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