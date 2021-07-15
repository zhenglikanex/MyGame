#pragma once

#include <algorithm>

#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/Character.hpp"
#include "Framework/Game/Component/PhysicsWorld.hpp"

#include "Framework/Game/System.hpp"

struct RigidbodyProjectionHit
{
	PxRigidBody* rigidbody;
	vec3 hit_point;
	vec3 effective_hit_normal;
	vec3 hit_velocity;
	bool stable_on_hit;
};

inline quat ForwardRotation(const vec3& forward)
{
	if (forward.x >= fixed16(0))
	{
		auto cos = glm::clamp(glm::dot(forward, vec3(0.0, 0.0, 1.0)), fixed16(-1), fixed16(1));
		return quat(vec3(0, fpm::acos(cos), 0));
	}
	else
	{
		auto cos = glm::clamp(glm::dot(forward, vec3(0.0, 0.0, 1.0)), fixed16(-1), fixed16(1));
		return quat(vec3(0, -fpm::acos(cos), 0));
	}
}

struct KinematicCharacterMovementSystem : public System
{
	static const int kMaxHitsBudget = 16;
	static const int kMaxCollisionBudget = 16;
	static const int kMaxGroundingSweepIterations = 2;
	static const int kMaxSteppingSweepIterations = 3;
	static const int kMaxRigidbodyOverlapsCount = 16;
	const float kCollisionOffset = 0.01f;
	const float kGroundProbeReboundDistance = 0.02f;
	const float kMinimumGroundProbingDistance = 0.005f;
	const float kGroundProbingBackstepDistance = 0.1f;
	const float kSweepProbingBackstepDistance = 0.002f;
	const float kSecondaryProbesVertical = 0.02f;
	const float kSecondaryProbesHorizontal = 0.001f;
	const float kMinVelocityMagnitude = 0.01f;
	const float kSteppingForwardDistance = 0.03f;
	const float kMinDistanceForLedge = 0.05f;
	const float kCorrelationForVerticalObstruction = 0.01f;
	const float kExtraSteppingForwardDistance = 0.01f;
	const float kExtraStepHeightPadding = 0.01f;

	PxRaycastHit internal_character_hits[kMaxHitsBudget];
	entt::entity internal_probed_colliders[kMaxCollisionBudget];
	std::vector<PxRigidBody*> rigidbodies_pushed_this_move;
	PxRigidBody* last_attached_rigidbody;
	bool solve_movement_collisions = true;
	bool solve_grounding = true;
	bool move_poistion_dirty = false;
	vec3 move_position_target = vec3(0);
	bool move_rotation_dirty = false;
	quat move_rotation_target = glm::identity<quat>();
	bool last_solved_overlap_normal_dirty = false;
	vec3 last_solved_overlap_normal = vec3(0, 0, 1);
	int rigidbody_projection_hit_count = 0;
	bool is_moving_from_attached_rigidbody = false;
	bool must_unground = false;
	float must_unground_time_counter = 0.f;
	vec3 cached_world_up = vec3(0, 1, 0);
	vec3 cached_world_forward = vec3(0, 0, 1);
	vec3 cached_world_right = vec3(1, 0, 0);
	vec3 cached_zero_vector = vec3(0);
	

	KinematicCharacterMovementSystem(entt::registry& _registry) : System(_registry) { }
	~KinematicCharacterMovementSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		
	}

	void UpdatePhase1(float dt)
	{
		auto view = registry.view<Local,Transform,CharacterMovement, Character,Collider>();
	}

	void UpdatePhase2(float dt)
	{

	}

	void Finalize() override
	{
		
	}
};
