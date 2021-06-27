#pragma once

#include "PxPhysicsAPI.h"

using namespace physx;

#ifndef USE_PVD 
#define USE_PVD 1
#endif

struct PhysicsWorld
{
	static PxDefaultAllocator		s_allocator;
	static PxDefaultErrorCallback	s_error_callback;

	PxFoundation* foundation;
	PxPhysics* physics;
	PxDefaultCpuDispatcher* dispatcher;
	PxScene* scene;
	PxMaterial* material;

#if USE_PVD
	PxPvd* pvd;
#endif

	PhysicsWorld() 
		: foundation(nullptr)
		, physics(nullptr)
		, dispatcher(nullptr)
		, scene(nullptr)
		, material(nullptr)

#if USE_PVD
		, pvd(nullptr)
#endif
	{
		
	}
};

