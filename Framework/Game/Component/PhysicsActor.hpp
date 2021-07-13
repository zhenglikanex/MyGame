#pragma once

#include "PxPhysicsAPI.h"

enum class PhysicsActorType
{
	kNull,
	kStatic,
	kDynamic,
	kKinematic
};

struct PhysicsStaticTag {};
struct PhysicsDynamicTag {};
struct PhysicsKinematicTag {};

struct PhysicsActor
{
	entt::entity self;
	PxRigidActor* actor;

	PhysicsActor() 
		: self(entt::null)
		, actor(nullptr)
	{

	}

	PhysicsActor(entt::entity _self, PxRigidActor* _actor)
		: self(_self)
		, actor(_actor)
	{

	}
};

