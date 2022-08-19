#pragma once

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

#include "PxPhysicsAPI.h"

using namespace physx;

struct MovementForward
{
	vec3 value;

	MovementForward() = default;

	MovementForward(const vec3& _value)
		: value(_value)
	{

	}
};

NON_MEMEBER_BAR(MovementForward, obj.value);

struct Movement
{
	vec3 velocity;
	vec3 angular_velocity;

	Movement() = default;

	Movement(const vec3& _velocity, const vec3& _angular_velocity)
		: velocity(_velocity)
		, angular_velocity(_angular_velocity)
	{

	}
};

NON_MEMEBER_BAR(Movement,obj.velocity, obj.angular_velocity);

class CharacterTransientGroundingReport;
class CharacterGroundingReport;

class CharacterGroundingReport
{
public:
	bool found_any_ground;
	bool is_stable_on_ground;
	bool snpping_prevented;
	vec3 ground_normal;
	vec3 inner_ground_normal;
	vec3 outer_ground_normal;

	PxRigidActor* ground_collider;
	vec3 ground_point;

	CharacterGroundingReport() = default;
	CharacterGroundingReport(CharacterTransientGroundingReport* transient_grounding_report);
};

NON_MEMEBER_BAR(CharacterGroundingReport,obj.found_any_ground,obj.is_stable_on_ground,obj.snpping_prevented,obj.inner_ground_normal,obj.outer_ground_normal,obj.ground_point);

class CharacterTransientGroundingReport
{
public:
	bool found_any_ground;
	bool is_stable_on_ground;
	bool snpping_prevented;
	vec3 ground_normal;
	vec3 inner_ground_normal;
	vec3 outer_ground_normal;

	CharacterTransientGroundingReport() = default;
	CharacterTransientGroundingReport(CharacterGroundingReport* grounding_report);
};

inline CharacterGroundingReport::CharacterGroundingReport(CharacterTransientGroundingReport* transient_grounding_report)
	: found_any_ground(transient_grounding_report->found_any_ground)
	, is_stable_on_ground(transient_grounding_report->is_stable_on_ground)
	, snpping_prevented(transient_grounding_report->snpping_prevented)
	, ground_normal(transient_grounding_report->ground_normal)
	, inner_ground_normal(transient_grounding_report->inner_ground_normal)
	, outer_ground_normal(transient_grounding_report->outer_ground_normal)
	, ground_collider(nullptr)
	, ground_point(vec3(0))
{

}

inline CharacterTransientGroundingReport::CharacterTransientGroundingReport(CharacterGroundingReport* grounding_report)
	: found_any_ground(grounding_report->found_any_ground)
	, is_stable_on_ground(grounding_report->is_stable_on_ground)
	, snpping_prevented(grounding_report->snpping_prevented)
	, ground_normal(grounding_report->ground_normal)
	, inner_ground_normal(grounding_report->inner_ground_normal)
	, outer_ground_normal(grounding_report->outer_ground_normal)
{

}

//包含在运动阶段击中的刚体信息，方便后续处理
struct RigidbodyProjectionHit
{
	PxRigidDynamic* rigidbody;
	vec3 hit_point;
	vec3 effective_hit_normal;
	vec3 hit_velocity;
	bool stable_on_hit;

	RigidbodyProjectionHit() 
		: rigidbody(nullptr)
		, stable_on_hit(false)
	{

	}
};

struct CharacterMovement
{	
	vec3 base_velocity;
	vec3 attached_rigidbody_velocity;

	// 最后一个扫略碰撞是否找到的接地点
	bool last_movement_iteration_found_any_ground;

	// 接地状态
	CharacterGroundingReport grounding_status;
	CharacterGroundingReport last_grounding_status;

	//连接的rigidbody
	PxRigidDynamic* attached_rigidbody;
	PxRigidDynamic* last_attached_rigidbody;

	std::array<RigidbodyProjectionHit, 16> internal_rigidbody_projection_hits;
	int rigidbody_projection_hit_count;

	CharacterMovement()
		: base_velocity(vec3(0))
		, attached_rigidbody_velocity(vec3(0))
		, last_movement_iteration_found_any_ground(false)
		, attached_rigidbody(nullptr)
		, last_attached_rigidbody(nullptr)
		, rigidbody_projection_hit_count(0)
	{

	}
};

NON_MEMEBER_BAR(CharacterMovement,obj.base_velocity, obj.attached_rigidbody_velocity, obj.grounding_status, obj.last_grounding_status);

struct IgnoreGravity
{
	bool b;
	IgnoreGravity() = default;
};

EMPTY_NON_MEMEBER_BAR(IgnoreGravity);

struct MovePositionTarget
{
	vec3 value;
};

struct MoveRotationTarget
{
	quat value;
};