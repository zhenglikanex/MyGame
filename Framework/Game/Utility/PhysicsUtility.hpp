#pragma once

#include "Framework/Game/Component/PhysicsWorld.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp" 

#include "3rdparty/include/entt/entt.hpp"

struct PhysicsUtility
{
	static PhysicsWorld CreatePhysicsWorld()
	{
		PhysicsWorld world;
		world.foundation = PxCreateFoundation(PX_FOUNDATION_VERSION,PhysicsWorld::s_allocator,PhysicsWorld::s_error_callback);
		
#if USE_PVD
		world.pvd = PxCreatePvd(*world.foundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		world.pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
		world.physics = PxCreatePhysics(PX_PHYSICS_VERSION, *world.foundation, PxTolerancesScale(), true, world.pvd);
#else
		world.physics = PxCreatePhysics(PX_PHYSICS_VERSION, *world.foundation, PxTolerancesScale(), true);
#endif

		PxSceneDesc scene_desc(world.physics->getTolerancesScale());
		scene_desc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		world.dispatcher = PxDefaultCpuDispatcherCreate(2);
		scene_desc.cpuDispatcher = world.dispatcher;
		scene_desc.filterShader = PxDefaultSimulationFilterShader;
		world.scene = world.physics->createScene(scene_desc);
		
#if USE_PVD
		PxPvdSceneClient* pvdClient = world.scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
#endif
		
		world.material = world.physics->createMaterial(0.5, 0.5f, 0.6f);

		return world;
	}

	static void DestoryPhysicsWorld(PhysicsWorld& world)
	{
		world.scene->release();
		world.scene = nullptr;

		world.dispatcher->release();
		world.dispatcher = nullptr;

		world.physics->release();
		world.physics = nullptr;

#if USE_PVD
		PxPvdTransport* transport = world.pvd->getTransport();
		world.pvd->release();
		world.pvd = nullptr;

		transport->release();
#endif
		world.foundation->release();
		world.foundation = nullptr;

	}

	//
	static PxRigidDynamic* CreateDynamic(entt::registry& registry,const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
	{
		auto& world = registry.ctx<PhysicsWorld>();
		PxRigidDynamic* dynamic = PxCreateDynamic(*world.physics, t, geometry, *world.material, 10.0f);
		//dynamic->setAngularDamping(0.5f);
		dynamic->setLinearVelocity(velocity);
		return dynamic;
	}

	static PxRigidStatic* CreateStatic(entt::registry& registry, const PxTransform& t, const PxGeometry& geometry)
	{
		auto& world = registry.ctx<PhysicsWorld>();
		PxRigidStatic* rigid_static = PxCreateStatic(*world.physics, t, geometry,*world.material);
		return rigid_static;
	}

	static PxRigidDynamic* CreateKinematic(entt::registry& registry, const PxTransform& t, const PxGeometry& geometry)
	{
		auto& world = registry.ctx<PhysicsWorld>();
		PxRigidDynamic* dynamic = PxCreateDynamic(*world.physics, t, geometry, *world.material, 10.0f);
		//dynamic->setAngularDamping(0.5f);
		return dynamic;
	}
};