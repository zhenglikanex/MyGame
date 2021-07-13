#pragma once

#include "Framework/Game/Math.hpp"

#include "Framework/Game/Component/PhysicsWorld.hpp"
#include "Framework/Game/Component/Transform.hpp"

#include "3rdparty/include/entt/entt.hpp"

inline PxFilterFlags SubmarineFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	// trigger the contact callback for pairs (A,B) where 
	// the filtermask of A contains the ID of B and vice versa.
	//pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;
	}

	return PxFilterFlag::eDEFAULT;
}

struct PhysicsUtility
{
	enum class FilterGroup : PxU32
	{
		kActor	= (1 << 0),
		kWeapon = (1 << 1),

		kAll	= ~0
	};

	static PxU32 GetFilterMask(FilterGroup group)
	{
		PxU32 filter_mask = (PxU32)FilterGroup::kAll;

		switch (group)
		{
		case PhysicsUtility::FilterGroup::kActor:
			filter_mask = (PxU32)FilterGroup::kWeapon;

			break;
		case PhysicsUtility::FilterGroup::kWeapon:
			filter_mask = (PxU32)FilterGroup::kActor;

			break;
		default:
			break;
		}

		return filter_mask;
	}

	static PhysicsWorld CreatePhysicsWorld(PxSimulationEventCallback* callback)
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
		scene_desc.filterShader = SubmarineFilterShader;
		// 启用trigger之间的碰撞检测,保持跟unity一致，注意这个在physx3.5中被移除
		scene_desc.flags |= PxSceneFlag::eDEPRECATED_TRIGGER_TRIGGER_REPORTS;
		scene_desc.simulationEventCallback = callback;
		
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
	static PxRigidDynamic* CreateDynamic(
		entt::registry& registry,
		const PxTransform& t,
		const PxGeometry& geometry,
		bool trigger = false,
		const PxTransform& shape_offset = PxTransform(PxIdentity),
		FilterGroup group = FilterGroup::kAll,
		const PxVec3& velocity = PxVec3(0))
	{
		auto& world = registry.ctx<PhysicsWorld>();
		PxRigidDynamic* dynamic = PxCreateDynamic(*world.physics, t, geometry, *world.material, 10.0f,shape_offset);
		//dynamic->setAngularDamping(0.5f);
		dynamic->setLinearVelocity(velocity);

		PxShape* shape;
		dynamic->getShapes(&shape, 1);

		if (trigger)
		{
			//shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			//shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		SetupFiltering(shape, (PxU32)group, GetFilterMask(group));

		world.scene->addActor(*dynamic);
		return dynamic;
	}

	static PxRigidStatic* CreateStatic(
		entt::registry& registry, 
		const PxTransform& t,
		const PxGeometry& geometry,
		bool trigger = false, 
		const PxTransform& shape_offset = PxTransform(PxIdentity),
		FilterGroup group = FilterGroup::kAll)
	{
		auto& world = registry.ctx<PhysicsWorld>();
		PxRigidStatic* rigid_static = PxCreateStatic(*world.physics, t, geometry,*world.material,shape_offset);

		PxShape* shape;
		rigid_static->getShapes(&shape, 1);

		if (trigger)
		{
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		SetupFiltering(shape, (PxU32)group, GetFilterMask(group));

		world.scene->addActor(*rigid_static);
		
		return rigid_static;
	}

	static PxRigidDynamic* CreateKinematic(
		entt::registry& registry,
		const PxTransform& t,
		const PxGeometry& geometry,
		bool trigger = false,
		const PxTransform& shape_offset = PxTransform(PxIdentity),
		FilterGroup group = FilterGroup::kAll)
	{
		auto& world = registry.ctx<PhysicsWorld>();
		PxRigidDynamic* kinematic = PxCreateKinematic(*world.physics, t, geometry, *world.material, 10.0f, shape_offset);

		PxShape* shape;
		kinematic->getShapes(&shape, 1);

		if (trigger)
		{	
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		SetupFiltering(shape, (PxU32)group, GetFilterMask(group));

		world.scene->addActor(*kinematic);
		return kinematic;
	}

	static void SetupFiltering(PxShape* shape, PxU32 filter_group,PxU32 filter_mask)
	{
		PxFilterData filter_data;
		filter_data.word0 = filter_group;	// word0 = own ID
		filter_data.word1 = filter_mask;	// word1 = ID mask to filter pairs that trigger a contact callback;

		shape->setSimulationFilterData(filter_data);
	}

	static PxTransform Transform(const Transform& transform)
	{
		PxVec3 pos(Vec3(transform.position));
		PxQuat quat(Quat(transform.rotation));
		return PxTransform(Vec3(transform.position), Quat(transform.rotation));
	}

	static PxTransform Transform(mat4& mat)
	{
		return PxTransform(PxMat44(glm::value_ptr(mat)));
	}

	static PxVec2 Vec2(const vec2& v)
	{
		return PxVec2(v.x, v.y);
	}

	static PxVec3 Vec3(const vec3& v)
	{
		return PxVec3(v.x, v.y, v.z);
	}

	static PxVec4 Vec4(const vec4& v)
	{
		return PxVec4(v.x, v.y, v.z, v.z);
	}

	static PxQuat Quat(const quat& q)
	{
		return PxQuat(q.x, q.y, q.z, q.w);
	}
};