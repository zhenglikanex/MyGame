#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/PhysicsWorld.hpp"
#include "Framework/Game/Component/PhysicsActor.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Contact.hpp"

#include "Framework/Game/Utility/PhysicsUtility.hpp"

struct CreatePhysicsActorSystem : public System
{
	CreatePhysicsActorSystem(entt::registry& _registry) : System(_registry) { }
	~CreatePhysicsActorSystem() {}

	static void RemovePhysicActor(entt::registry& registry,entt::entity e)
	{
		auto& physics_world = registry.ctx<PhysicsWorld>();
		auto& physics_actor = registry.get<PhysicsActor>(e);
		if (physics_actor.actor)
		{
			INFO("removeActor!!!!!!!!!!!!!!!!!!!")
			physics_world.scene->removeActor(*physics_actor.actor,false);
		}
	}
	
	bool Initialize() override
	{
		registry.on_destroy<PhysicsActor>().connect<&CreatePhysicsActorSystem::RemovePhysicActor>();
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto view = registry.view<Transform,ColliderInfo>(entt::exclude<PhysicsActor>);
		for (auto e : view)
		{
			auto& [transform,collider_info] = view.get<Transform,ColliderInfo>(e);

			PxRigidActor* actor = nullptr;
			PxTransform px_transform(PhysicsUtility::Transform(transform));
			PxTransform offset(PhysicsUtility::Transform(collider_info.transform));

			auto& geom = collider_info.geometry;
			if (geom.type() == GeometryType::kBox)
			{
				actor = PhysicsUtility::CreateKinematic(registry, px_transform, PxBoxGeometry(geom.box().e.x, geom.box().e.y, geom.box().e.z), collider_info.trigger, offset);
			}
			else if (geom.type() == GeometryType::kCapsule)
			{
				actor = PhysicsUtility::CreateKinematic(registry, px_transform, PxCapsuleGeometry(geom.capsule().r, geom.capsule().h), collider_info.trigger, offset);
			}
			else if (geom.type() == GeometryType::kSphere)
			{
				actor = PhysicsUtility::CreateKinematic(registry, px_transform, PxSphereGeometry(geom.sphere().r), collider_info.trigger, offset);
			}
			
			if (actor)
			{
				registry.emplace<PhysicsStaticTag>(e);
				registry.emplace<PhysicsActor>(e, e, actor);
				actor->userData = (void*)e;

				registry.emplace<ContactList>(e);
			}

		}
	}

	void Finalize() override
	{
		registry.on_destroy<PhysicsActor>().disconnect<&CreatePhysicsActorSystem::RemovePhysicActor>();
	}
};