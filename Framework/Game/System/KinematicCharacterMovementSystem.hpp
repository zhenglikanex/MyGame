#pragma once

#include <algorithm>

#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/Character.hpp"
#include "Framework/Game/Component/PhysicsWorld.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/PhysicsActor.hpp"
#include "Framework/Game/Component/PhysicsWorld.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/PhysicsUtility.hpp"

//包含碰撞稳定性(是否可以站立)评估的所有信息
struct HitStabilityReport
{
	bool is_stable = false;
	vec3 inner_normal = vec3(0);
	vec3 outer_noraml = vec3(0);
	
	bool vaild_step_detected = false;
	PxRigidActor* stepped_collider = nullptr;
};

struct OverlapResult
{
	vec3 normal;
};

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

	std::vector<PxRigidDynamic*> rigidbodies_pushed_this_move;
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

	// 指定角色的移动算法可以检测到的碰撞的 LayerMask。 默认情况下，这使用刚体层的碰撞矩阵
	uint32_t collidable_layers = -1;

	explicit KinematicCharacterMovementSystem(Registry& _registry) : System(_registry) { }

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		INFO("kcc update");
		UpdatePhase1(dt);
		//
		UpdatePhase2(dt);
	}

	void UpdatePhase1(float dt)
	{
		auto view = registry.view<Local,Transform,CharacterMovement,Character,Collider,PhysicsActor>();
		for (auto e : view)
		{
			INFO("UpdatePhase1{}",registry.get<Player>(e).id);
			overlaps_count_ = 0;

			auto&[transform, movement, character,collider, physics_actor] = 
				view.get<Transform, CharacterMovement, Character,Collider, PhysicsActor>(e);
			
			e_ = e;
			character_ = &character;
			character_movement_ = &movement;

			INFO("base_velocity x{} y{} z{}", character_movement_->base_velocity.x, character_movement_->base_velocity.y, character_movement_->base_velocity.z);

			// 获取physx相关的数据
			PxShape* shape;
			physics_actor.actor->getShapes(&shape, 1);

			assert(shape->getGeometryType() == PxGeometryType::eCAPSULE && "not capsule!");
			shape_transform_ = shape->getLocalPose();
			shape->getCapsuleGeometry(px_caspule_);

			px_transform_ = physics_actor.actor->getGlobalPose();
			rigid_ = physics_actor.actor;


			// 清除推动的物体
			rigidbodies_pushed_this_move.clear();

			//更新前处理
			// todo
			BeforeCharacterUpdate(dt);

			// 重置模拟数据
			transient_position_ = transform.position;
			SetTransientRotation(transform.rotation);
			rigidbody_projection_hit_count = 0;
			overlaps_count_ = 0;
			last_solved_overlap_normal_dirty = false;

			// 处理直接移动角色位置的逻辑
			auto move_position_target = registry.try_get<MovePositionTarget>(e);
			if (move_position_target)
			{
				// 是否求解直接移动的碰撞
				if (solve_movement_collisions)
				{
					vec3 tmp_velocity = GetVelocityFromMovement(move_position_target->value - transient_position_, dt);
					if (InternalCharacterMove(tmp_velocity, dt))
					{
						if (character_->interactive_rigid_body_handling)
						{
							ProcessVelocityForRigidbodyHits(tmp_velocity, dt);
						}
					}
				}
				else
				{
					transient_position_ = move_position_target->value;
				}

				move_position_target = nullptr;
			}

			// 保存上一次接地模拟状态
			character_movement_->last_grounding_status = character_movement_->grounding_status;

			//初始化新的接地模拟状态
			character_movement_->grounding_status = CharacterGroundingReport();
			character_movement_->grounding_status.ground_normal = character_up_;

			if (solve_movement_collisions)
			{
				vec3 resolution_direction = cached_world_up;
				float resolution_distance = 0.f;
				int iterations_made = 0;
				bool overlap_solved = false;

				// 小于检测冲突次数并且有冲突没有解决完
				while (iterations_made < character_->max_decollision_iterations && !overlap_solved)
				{
					auto overlaps = CharacterCollisionsOverlap(transient_position_, transient_rotation_);
					if (overlaps.size() > 0)
					{
						//解决与动态刚体或物理移动器无关的重叠(跟eKINEMATIC的重叠)
						for (auto& overlap : overlaps)
						{
							// 不是physics mover或者运动刚体
							if (GetInteractiveRigidbody(overlap) == nullptr)
							{
								PxShape* overlap_shape;
								overlap->getShapes(&overlap_shape, 1);
								auto overlap_transform = overlap->getGlobalPose();

								auto my_tmp_transform = PhysicsUtility::Transform(transient_position_, transient_rotation_);

								PxVec3 resolution_direction;
								float resolution_distance;
								if (PxGeometryQuery::computePenetration(
									resolution_direction,
									resolution_distance,
									px_caspule_,
									my_tmp_transform * shape_transform_,
									overlap_shape->getGeometry().any(),
									overlap_transform * overlap_shape->getLocalPose()))
								{
									vec3 resolution_dir = PhysicsUtility::Vec3(resolution_direction);
									resolution_dir = GetObstructionNormal(resolution_dir, IsStableOnNormal(resolution_dir));
									
									// 这里用的是修正后的方向,有可能移动后还是会重叠,但是这里每次会重新检查
									transient_position_ += (resolution_dir * (resolution_distance + kCollisionOffset));
									
									if (overlaps_count_ < overlaps_.size())
									{
										overlaps_[overlaps_count_++] = OverlapResult{ resolution_dir };
									}

									break;
								}
							}
						}
					}
					else
					{
						overlap_solved = true;
					}

					++iterations_made;
				}

			}

			//地面探测和捕捉
			if (solve_grounding)
			{
				if (MustUnground())
				{
					// 加一个最低探测距离的高度，强制分离地面
					transient_position_ += character_up_ * (kMinimumGroundProbingDistance * 1.5f);
				}
				else
				{
					//选择合适的地面探测距离
					float selected_ground_probing_distance = kMinimumGroundProbingDistance;
					if (!character_movement_->last_grounding_status.snpping_prevented && 
						(character_movement_->last_grounding_status.is_stable_on_ground || character_movement_->last_movement_iteration_found_any_ground))
					{
						if (character_->support_step)
						{
							selected_ground_probing_distance = std::max(character_->max_step_height, px_caspule_.radius);
						}
						else
						{
							selected_ground_probing_distance = px_caspule_.radius;
						}

						// 加上配置的额外检测距离,加速响应
						selected_ground_probing_distance += character_->ground_detection_extra_distance;
					}

					ProbeGround(transient_position_, transient_rotation_, selected_ground_probing_distance, character_movement_->grounding_status);

					//从空中到落地,去掉y轴的速度
					if (!character_movement_->last_grounding_status.is_stable_on_ground && character_movement_->grounding_status.is_stable_on_ground)
					{
						character_movement_->base_velocity = ProjectOnPlane(character_movement_->base_velocity, character_up_);
						character_movement_->base_velocity = GetDirectionTangentToSurface(
							character_movement_->base_velocity,
							character_movement_->grounding_status.ground_normal) * 
							glm::length(character_movement_->base_velocity);
					}
				}
			}

			character_movement_->last_movement_iteration_found_any_ground = false;

			/*
			if (_solveGrounding)
			{
				CharacterController.PostGroundingUpdate(deltaTime);
			}
			*/

			if (character_->interactive_rigid_body_handling)
			{
				character_movement_->last_attached_rigidbody = character_movement_->attached_rigidbody;
				// 指定要连接的刚体
				if (nullptr)
				{
					//_attachedRigidbody = AttachedRigidbodyOverride;
					//character_movement_->attached_rigidbody_ == 
				}
				else
				{
					// 从地面检测连接的可交互刚体
					if (character_movement_->grounding_status.is_stable_on_ground && character_movement_->grounding_status.ground_collider)
					{
						PxRigidDynamic* interactive_rigidbody = GetInteractiveRigidbody(character_movement_->grounding_status.ground_collider);
						if (interactive_rigidbody)
						{
							character_movement_->attached_rigidbody = interactive_rigidbody;
						}
					}
					else
					{
						character_movement_->attached_rigidbody = nullptr;
					}
				}

				vec3 tmp_velocity_from_current_attached_rigidbody = vec3(0);
				if (character_movement_->attached_rigidbody)
				{
					tmp_velocity_from_current_attached_rigidbody = 
						GetVelocityFromRigidbodyMovement(character_movement_->attached_rigidbody,transient_position_, dt);
				}

				//离开运动的刚体时保持惯性速度
				if (character_->preserve_attached_rigidbody_momentum &&
					character_movement_->last_attached_rigidbody != nullptr &&
					character_movement_->last_attached_rigidbody != character_movement_->attached_rigidbody)
				{
					character_movement_->base_velocity += character_movement_->attached_rigidbody_velocity;
					character_movement_->base_velocity -= tmp_velocity_from_current_attached_rigidbody;
				}

				// 处理附加刚体的附加速度
				character_movement_->attached_rigidbody_velocity = vec3(0);
				if (auto rigidbody = character_movement_->attached_rigidbody)
				{
					character_movement_->attached_rigidbody_velocity = tmp_velocity_from_current_attached_rigidbody;
					
					// 从附加的刚体旋转
					vec3 new_forward = Normalize(ProjectOnPlane(
						quat(glm::degrees(PhysicsUtility::Vec3(rigidbody->getAngularVelocity() * dt))) * (transient_rotation_ * vec3(0, 0, 1)),
						character_up_));

					transient_rotation_ = LookRotation(new_forward, character_up_);
				}

				// 在附着的刚提上着陆时抵消水平速度
				if (character_movement_->grounding_status.ground_collider &&
					character_movement_->grounding_status.ground_collider == character_movement_->attached_rigidbody &&
					character_movement_->attached_rigidbody != nullptr &&
					character_movement_->last_attached_rigidbody == nullptr
					)
				{
					character_movement_->base_velocity -= ProjectOnPlane(character_movement_->attached_rigidbody_velocity, character_up_);
				}

				// 从附着的刚体移动
				if (glm::dot(character_movement_->attached_rigidbody_velocity, character_movement_->attached_rigidbody_velocity) > 0.f)
				{
					is_moving_from_attached_rigidbody = true;
					
					if (solve_movement_collisions)
					{
						InternalCharacterMove(character_movement_->attached_rigidbody_velocity, dt);
					}
					else
					{
						transient_position_ += character_movement_->attached_rigidbody_velocity * dt;
					}

					is_moving_from_attached_rigidbody = false;
				}
			}

			registry.replace<Transform>(e, transient_position_, transient_rotation_);
			INFO("update 1 transient_position_ x{} y{} z{}", transient_position_.x, transient_position_.y, transient_position_.z);
		}

		registry.clear<MovePositionTarget>();
	}

	void UpdatePhase2(float dt)
	{
		auto view = registry.view<Local, Transform, CharacterMovement,MovementForward,Character, Collider, PhysicsActor>();
		for (auto e : view)
		{
			overlaps_count_ = 0;

			auto& [transform, movement, forward, character, collider, physics_actor] =
				view.get<Transform, CharacterMovement, MovementForward, Character, Collider, PhysicsActor>(e);

			character_ = &character;
			character_movement_ = &movement;

			// 获取physx相关的数据
			PxShape* shape;
			physics_actor.actor->getShapes(&shape, 1);
			
			assert(shape->getGeometryType() == PxGeometryType::eCAPSULE && "not capsule!");
			shape_transform_ = shape->getLocalPose();
			shape->getCapsuleGeometry(px_caspule_);

			px_transform_ = physics_actor.actor->getGlobalPose();
			rigid_ = physics_actor.actor;

			// 清除推动的物体
			rigidbodies_pushed_this_move.clear();

			/* todo
			// Handle rotation
			CharacterController.UpdateRotation(ref _transientRotation, deltaTime);
			TransientRotation = _transientRotation;
			*/

			transient_position_ = transform.position;
			if (forward.value != vec3(0))
			{
				SetTransientRotation(LookRotation(forward.value, vec3(0, 1, 0)));
			}
			else
			{
				SetTransientRotation(transform.rotation);
			}
			

			if (registry.all_of<MovePositionTarget>(e))
			{
				SetTransientRotation(registry.get<MovePositionTarget>(e).value);
			}

			if (solve_movement_collisions && character_->interactive_rigid_body_handling)
			{
				// 求解可能的刚体重叠(UpdatePhase1移动后可能有重叠)
				if (character_movement_->attached_rigidbody)
				{
					float upwards_offset = px_caspule_.radius;
					
					PxSweepHit closest_hit;
					if (CharacterGroundSweep(
						transient_position_ + (character_up_ * upwards_offset),
						transient_rotation_,
						-character_up_,
						upwards_offset,
						closest_hit))
					{
						if (closest_hit.actor == character_movement_->attached_rigidbody && IsStableOnNormal(PhysicsUtility::Vec3(closest_hit.normal)))
						{
							float distance_moveed_up = (upwards_offset - closest_hit.distance);
							transient_position_ = transient_position_ + (character_up_ * distance_moveed_up) + (character_up_ * kCollisionOffset);
						}
					}
				}

				// 解决可能有旋转或者物理移动气模拟推动角色而导致的重叠
				vec3 resolution_direction = cached_world_up;
				float resolution_distance = 0.f;
				int iterations_made = 0;
				bool overlap_solved = false;
				while (iterations_made < character_->max_decollision_iterations && !overlap_solved)
				{
					auto overlaps = CharacterCollisionsOverlap(transient_position_, transient_rotation_);
					if (overlaps.size() > 0)
					{
						for (auto& overlap : overlaps)
						{
							auto rigid = overlap;
							PxShape* collision_shape;
							rigid->getShapes(&collision_shape, 1);

							PxTransform my_tmp_transform = PhysicsUtility::Transform(transient_position_, transient_rotation_);

							PxVec3 resolution_direction;
							float resolution_distance;
							if (PxGeometryQuery::computePenetration(
								resolution_direction,
								resolution_distance,
								px_caspule_,
								my_tmp_transform * shape_transform_,
								collision_shape->getGeometry().any(),
								rigid->getGlobalPose() * collision_shape->getLocalPose()))
							{
								vec3 resolution_dir = PhysicsUtility::Vec3(resolution_direction);
								resolution_dir = GetObstructionNormal(resolution_dir, IsStableOnNormal(resolution_dir));
								
								//解决重叠
								vec3 resolution_movement = resolution_dir * (resolution_dir + kCollisionOffset);
								transient_position_ += resolution_movement;
								if (transient_position_.y > 1.f)
								{
									int v = 10;
								}

								//如果是交互式敢提，则注册为刚体命中速度
								if (character_->interactive_rigid_body_handling)
								{
									PxRigidDynamic* probed_rigid_body = GetInteractiveRigidbody(overlap);
									if (probed_rigid_body)
									{
										bool is_stable = IsStableOnNormal(resolution_dir);
										if (is_stable)
										{
											character_movement_->last_movement_iteration_found_any_ground = is_stable;
										}
										if (probed_rigid_body != character_movement_->attached_rigidbody)
										{
											vec3 estimated_collision_point = transient_position_;
											vec3 velocity = character_movement_->base_velocity + character_movement_->attached_rigidbody_velocity;

											StoreRigidbodyHit(probed_rigid_body, velocity, estimated_collision_point, resolution_dir, is_stable);
										}
									}
								}

								// 记录重叠
								if (overlaps_count_ < overlaps_.size())
								{
									overlaps_[overlaps_count_++] = { resolution_dir };
								}

								break;
							}
						}
					}
					else
					{
						overlap_solved = true;
					}

					iterations_made++;
				}
			}
			// Handle velocity
			//CharacterController.UpdateVelocity(ref BaseVelocity, deltaTime);
			UpdateVelocity(character_movement_->base_velocity, dt);

			if (glm::length(character_movement_->base_velocity) < kMinVelocityMagnitude)
			{
				character_movement_->base_velocity = vec3(0);
			}
			
			// 从基本速度计算角色移动
			if (glm::dot(character_movement_->base_velocity, character_movement_->base_velocity) > 0.f)
			{
				if (solve_movement_collisions)
				{
					InternalCharacterMove(character_movement_->base_velocity, dt);
				}
				else
				{
					transient_position_ += character_movement_->base_velocity * dt;
				}
			}

			// 处理刚体碰撞/重叠以影响速度
			if (character_->interactive_rigid_body_handling)
			{
				ProcessVelocityForRigidbodyHits(character_movement_->base_velocity, dt);
			}

			// Handle planar constraint
			/*
			if (HasPlanarConstraint)
			{
				_transientPosition = _initialSimulationPosition + Vector3.ProjectOnPlane(_transientPosition - _initialSimulationPosition, PlanarConstraintAxis.normalized);
			}
			*/
			registry.replace<Transform>(e, transient_position_, transient_rotation_);
		}

		registry.clear<MovePositionTarget>();
	}

	void BeforeCharacterUpdate(float dt)
	{

	}

	void Finalize() override
	{

	}

	void UpdateVelocity(vec3& current_velocity,float dt)
	{
		if (character_movement_->grounding_status.is_stable_on_ground && !registry.all_of<IgnoreGravity>(e_))
		{
			// Reorient source velocity on current ground slope (this is because we don't want our smoothing to cause any velocity losses in slope changes)
			current_velocity = GetDirectionTangentToSurface(current_velocity, character_movement_->grounding_status.ground_normal) * glm::length(current_velocity);

		}
		else
		{
			if (current_velocity != vec3(0))
			{
				// If we want to move, add an acceleration to the velocity
				/*Vector3 targetMovementVelocity = Motor.CharacterForward * _forwardAxis * MaxAirMoveSpeed;
				Vector3 velocityDiff = Vector3.ProjectOnPlane(targetMovementVelocity - currentVelocity, Gravity);
				currentVelocity += velocityDiff * AirAccelerationSpeed * deltaTime;*/
			}
			
			// Gravity
			if(!registry.all_of<IgnoreGravity>(e_))
			{
				current_velocity.y += -100.f * dt;
			}
			
			// Drag
			//currentVelocity *= (1f / (1f + (Drag * deltaTime)));
			current_velocity *= (1.f / (1.f + 0.1 * dt));
		}
	}

	void SetTransientRotation(quat rotation)
	{
		transient_rotation_ = rotation;
		character_up_ = rotation * cached_world_up;
		character_forward_ = rotation * character_forward_;
	}

	// 考虑刚体碰撞以增加速度
	void ProcessVelocityForRigidbodyHits(vec3& processed_velocity, float dt)
	{
		for (int i = 0; i < character_movement_->rigidbody_projection_hit_count; ++i)
		{
			RigidbodyProjectionHit body_hit = character_movement_->internal_rigidbody_projection_hits[i];
			if (body_hit.rigidbody &&
				std::find(rigidbodies_pushed_this_move.begin(), rigidbodies_pushed_this_move.end(), body_hit.rigidbody) == rigidbodies_pushed_this_move.end())
			{
				if (body_hit.rigidbody != character_movement_->attached_rigidbody)
				{
					if (PxRigidDynamic* rigidbody = body_hit.rigidbody)
					{
						rigidbodies_pushed_this_move.push_back(body_hit.rigidbody);

						float character_mass = character_->simulated_character_mass;
						vec3 character_velocity = body_hit.hit_velocity;
						bool hit_body_is_dynamic =!(rigidbody->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC);
						float hit_body_mass = rigidbody->getMass();
						float hit_body_mass_at_point = rigidbody->getMass();
						vec3 hit_body_velocity = PhysicsUtility::Vec3(rigidbody->getLinearVelocity());
				
						//计算角色质量占总质量的比例
						float character_to_body_mass_ratio = 1.f;
						{
							if (character_mass + hit_body_mass_at_point > 0.f)
							{
								character_to_body_mass_ratio = character_mass / (character_mass + hit_body_mass_at_point);
							}
							else
							{
								character_to_body_mass_ratio = 0.5f;
							}

							if (!hit_body_is_dynamic)
							{
								character_to_body_mass_ratio = 0.f;
							}
							else if(character_->rigidbody_interaction_type == RigidbodyInteractionType::kKinematic)
							{
								character_to_body_mass_ratio = 1.f;
							}
						}

						vec3 velocity_change_on_character;
						vec3 velocity_change_on_body;

						ComputeCollisionResolutionForHitBody(
							body_hit.effective_hit_normal,
							character_velocity,
							hit_body_velocity,
							character_to_body_mass_ratio,
							velocity_change_on_character,
							velocity_change_on_body);

						processed_velocity += velocity_change_on_character;
						
						if (hit_body_is_dynamic)
						{
							rigidbody->addForce(PhysicsUtility::Vec3(velocity_change_on_body),PxForceMode::eVELOCITY_CHANGE);
						}
						
						

					}
				}
			}
		}
	}

	void ComputeCollisionResolutionForHitBody(
		vec3 hit_normal,
		vec3 character_velocity,
		vec3 body_velocity,
		float character_to_body_mass_ratio,
		vec3& velocity_change_on_character,
		vec3& velocity_change_on_body)
	{
		float body_to_character_mass_ratio = 1.f - character_to_body_mass_ratio;
		float character_velocity_magnitude_on_hit_normal = glm::dot(character_velocity, hit_normal);
		float body_velocity_magnitude_on_hit_normal = glm::dot(body_velocity, hit_normal);

		// 如果角色速度与障碍物相反，则恢复在运动阶段投射的部分速度
		if (character_velocity_magnitude_on_hit_normal < 0.f)
		{
			vec3 restored_character_velocity = hit_normal * character_velocity_magnitude_on_hit_normal;
			velocity_change_on_character += restored_character_velocity;
		}

		if (body_velocity_magnitude_on_hit_normal > character_velocity_magnitude_on_hit_normal)
		{
			vec3 relative_impact_velocity = hit_normal * (body_velocity_magnitude_on_hit_normal - character_velocity_magnitude_on_hit_normal);
			velocity_change_on_character += relative_impact_velocity * body_to_character_mass_ratio;
			velocity_change_on_body += -relative_impact_velocity * character_to_body_mass_ratio;
		}
	}

	// 记录一个刚体碰撞，以便后面处理
	void StoreRigidbodyHit(PxRigidDynamic* rigidbody, vec3 hit_velocity, vec3 hit_point, vec3 obstruction_normal,bool is_stable)
	{
		if (character_movement_->rigidbody_projection_hit_count < character_movement_->internal_rigidbody_projection_hits.size())
		{
			entt::entity e = static_cast<entt::entity>((entt::id_type)rigidbody->userData);
			if (!registry.all_of<CharacterMovement>(e))
			{
				RigidbodyProjectionHit rph;
				rph.rigidbody = rigidbody;
				rph.hit_point = hit_point;
				rph.hit_velocity = hit_velocity;
				rph.effective_hit_normal = obstruction_normal;
				rph.stable_on_hit = is_stable;

				character_movement_->internal_rigidbody_projection_hits[character_movement_->rigidbody_projection_hit_count++] = rph;
			}
		}
	}

	//探测有效接地，如果发生接地捕捉,则改变transient_position
	void ProbeGround(vec3& probing_position, quat at_rotation, float probing_distance, CharacterGroundingReport& grounding_report)
	{
		if (probing_distance < kMinimumGroundProbingDistance)
		{
			probing_distance = kMinimumGroundProbingDistance;
		}

		int ground_sweeps_made = 0;
		PxSweepHit ground_sweep_hit;
		bool ground_sweeping_is_over = false;
		vec3 ground_sweep_position = probing_position;
		vec3 ground_sweep_direction = (at_rotation * -cached_world_up);
		float ground_probe_distance_remaining = probing_distance;
		while (ground_probe_distance_remaining > 0 && (ground_sweeps_made <= kMaxGroundingSweepIterations) && !ground_sweeping_is_over)
		{
			// 检测地面
			if (CharacterGroundSweep(
				ground_sweep_position,
				at_rotation,
				ground_sweep_direction,
				ground_probe_distance_remaining,
				ground_sweep_hit))
			{
				vec3 target_position = ground_sweep_position + (ground_sweep_direction * ground_sweep_hit.distance);
				HitStabilityReport ground_hit_stability_report = EvaluateHitStability(
					ground_sweep_hit.actor,
					PhysicsUtility::Vec3(ground_sweep_hit.normal),
					PhysicsUtility::Vec3(ground_sweep_hit.position),
					target_position,
					transient_rotation_);
				
				grounding_report.found_any_ground = true;
				grounding_report.ground_normal = PhysicsUtility::Vec3(ground_sweep_hit.normal);
				grounding_report.inner_ground_normal = ground_hit_stability_report.inner_normal;
				grounding_report.outer_ground_normal = ground_hit_stability_report.outer_noraml;
				grounding_report.ground_collider = ground_sweep_hit.actor;
				grounding_report.ground_point = PhysicsUtility::Vec3(ground_sweep_hit.position);
				grounding_report.snpping_prevented = false;

				//有找到稳定的地面
				if (ground_hit_stability_report.is_stable)
				{
					// Find all scenarios where ground snapping should be canceled
					// 是否要防止地面捕获，如冲出悬崖这种情况应该不捕获放他保持原来的移动
					// groundingReport.SnappingPrevented = !IsStableWithSpecialCases(ref groundHitStabilityReport, BaseVelocity);
					grounding_report.snpping_prevented = false;

					grounding_report.is_stable_on_ground = true;

					// 地面捕获
					if (!grounding_report.snpping_prevented)
					{
						probing_position = ground_sweep_position + (ground_sweep_direction * (ground_sweep_hit.distance - kCollisionOffset));
					}

					ground_sweeping_is_over = true;
				}
				else
				{
					// 连续向下探测斜坡
					vec3 sweep_movement = (ground_sweep_direction * ground_sweep_hit.distance) +
						(at_rotation * cached_world_up) * std::max(kCollisionOffset, ground_sweep_hit.distance);
					ground_sweep_position += sweep_movement;

					// 设置剩余探测距离
					ground_probe_distance_remaining = std::min(kGroundProbeReboundDistance, std::max(ground_probe_distance_remaining - glm::length(sweep_movement), 0.f));
					
					// 调整探测的方向，沿着碰撞的斜坡探测
					ground_sweep_direction = Normalize(ProjectOnPlane(ground_sweep_direction, PhysicsUtility::Vec3(ground_sweep_hit.normal)));
					
				}
			}
			else
			{
				ground_sweeping_is_over = true;
			}

			ground_sweeps_made++;
		}
	}
	
	// 在刚体的给定点上获得真实的线速度（考虑旋转速度）
	vec3 GetVelocityFromRigidbodyMovement(PxRigidActor* interactive_rigidbody, vec3 at_point, float dt)
	{
		PxRigidDynamic* dynamic_rigidbody = interactive_rigidbody->is<PxRigidDynamic>();
		if (!dynamic_rigidbody)
		{
			return vec3(0);
		}

		if (dt > 0.f)
		{
			vec3 effective_mover_velocity = PhysicsUtility::Vec3(dynamic_rigidbody->getLinearVelocity());
			vec3 angular_velocity = PhysicsUtility::Vec3(dynamic_rigidbody->getAngularVelocity());

			if (angular_velocity != vec3(0))
			{
				vec3 ceneter_of_rotation = PhysicsUtility::Vec3((dynamic_rigidbody->getGlobalPose() * dynamic_rigidbody->getCMassLocalPose()).transform(PxVec3(0)));
				vec3 center_of_rotation_to_point = at_point - ceneter_of_rotation;
				quat rotation_from_interactive_rigidbody = quat(glm::degrees(angular_velocity * dt));
				vec3 final_point_position = ceneter_of_rotation + (rotation_from_interactive_rigidbody * center_of_rotation_to_point);
				effective_mover_velocity += (final_point_position - at_point) / dt;
			}
			return effective_mover_velocity;
		}
		else
		{
			return vec3(0);
		}
	}

	PxRigidDynamic* GetInteractiveRigidbody(PxRigidActor* actor)
	{
		if (actor)
		{
			entt::entity e = static_cast<entt::entity>((entt::id_type)actor->userData);
			//todo
			/*if (registry.valid(e) && registry.all_of<PhysicsMover>(e))
			{
				return actor;
			}*/
			auto dynamic_rigid = actor->is<PxRigidDynamic>();
			if (!(dynamic_rigid && dynamic_rigid->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC))
			{
				return dynamic_rigid;
			}
		}

		return nullptr;
	}

	// 通过给定的运动移动角色的位置,同时考虑印象角色运动的所有物理模拟、步长处理和速度投影规则
	// 如果在结束前无法结束移动,则返回false
	bool InternalCharacterMove(vec3& transient_velocity, float dt)
	{
		if (dt < 0.f)
		{
			return false;
		}

		bool was_completed = true;
		vec3 remaining_movement_direction = Normalize(transient_velocity);
		float remaining_movement_magnitude = glm::length(transient_velocity) * dt;
		vec3 original_velocity_direction = remaining_movement_direction;
		int sweeps_made = 0;
		bool hit_something_this_sweep_iteration = true;
		vec3 tmp_moved_position = transient_position_;
		bool previous_hit_is_srtable = false;
		vec3 previous_veloctiy = cached_zero_vector;
		vec3 previous_obstruction_normal = cached_zero_vector;
		MovementSweepState sweep_state = MovementSweepState::kInitial;

		//在进行扫略之前处理重叠(之前记录下会重叠的碰撞)
		for (int i = 0; i < overlaps_count_; ++i)
		{
			vec3 overlap_normal = overlaps_[i].normal;
			// 大于90°，说明移动方向是往会覆盖到的方向在走，需要处理这种情况
			if (glm::dot(remaining_movement_direction, overlap_normal) < 0.f)
			{
				bool stable_on_hit = IsStableOnNormal(overlap_normal);
				vec3 velocity_before_projection = transient_velocity;
				vec3 obstruction_normal = GetObstructionNormal(overlap_normal, stable_on_hit);

				InternalHandleVelocityProjection(
					stable_on_hit,
					overlap_normal,
					obstruction_normal,
					original_velocity_direction,
					sweep_state,
					previous_hit_is_srtable,
					previous_veloctiy,
					previous_obstruction_normal,
					transient_velocity,
					remaining_movement_magnitude,
					remaining_movement_direction);

				previous_hit_is_srtable = stable_on_hit;
				previous_veloctiy = velocity_before_projection;
				previous_obstruction_normal = obstruction_normal;
			}
		}
			// 扫描所需的运动以检测碰撞
		while (remaining_movement_magnitude > 0.f &&
			(sweeps_made <= character_->max_movement_iterations) &&
			hit_something_this_sweep_iteration)
		{
			bool found_closest_hit = false;
			vec3 closest_sweep_hit_point;
			vec3 closeset_sweep_hit_normal;
			float closest_sweep_hit_distance = 0.f;
			PxRigidActor* closest_sweep_hit_rigid = nullptr;

			if (character_->check_movement_initial_overlaps)
			{
				auto collisions = CharacterCollisionsOverlap(tmp_moved_position, transient_rotation_, 0.f, false);
				if (collisions.size() > 0)
				{
					closest_sweep_hit_distance = 0.f;

					float most_obstructing_overlap_normal_dot_product = 2.f;

					for (int i = 0; i < collisions.size(); i++)
					{
						auto rigid = collisions[i];
						PxShape* collision_shape;
						rigid->getShapes(&collision_shape, 1);

						PxTransform my_tmp_transform = PhysicsUtility::Transform(tmp_moved_position, transient_rotation_);

						PxVec3 resolution_direction;
						float resolution_distance;
						if (PxGeometryQuery::computePenetration(
							resolution_direction,
							resolution_distance,
							px_caspule_,
							my_tmp_transform * shape_transform_,
							collision_shape->getGeometry().any(),
							rigid->getGlobalPose() * collision_shape->getLocalPose()))
						{
							float dot_product = PhysicsUtility::Vec3(remaining_movement_direction).dot(resolution_direction);
							//寻找最近碰撞点，相当于就是更移动方向夹角最大的一个碰撞
							if (dot_product < 0.f && dot_product < most_obstructing_overlap_normal_dot_product)
							{
								most_obstructing_overlap_normal_dot_product = dot_product;

								closeset_sweep_hit_normal = PhysicsUtility::Vec3(resolution_direction);
								closest_sweep_hit_rigid = rigid;
								closest_sweep_hit_point = tmp_moved_position + (transient_rotation_ * vec3(0, px_caspule_.halfHeight, 0)) + PhysicsUtility::Vec3(resolution_direction * resolution_distance);

								found_closest_hit = true;

							}
						}
					}
				}
			}

			PxSweepHit closest_hit;
			std::vector<PxSweepHit> hits;
			if (!found_closest_hit)
			{
				hits = CharacterCollisionsSweep(
					tmp_moved_position,
					transient_rotation_,
					remaining_movement_direction,
					remaining_movement_magnitude + kCollisionOffset,
					closest_hit);
				if (hits.size() > 0)
				{
					closeset_sweep_hit_normal = PhysicsUtility::Vec3(closest_hit.normal);
					closest_sweep_hit_distance = closest_hit.distance;
					closest_sweep_hit_point = PhysicsUtility::Vec3(closest_hit.position);
					closest_sweep_hit_rigid = closest_hit.actor;

					found_closest_hit = true;
				}
			}
			if (found_closest_hit)
			{
				// 移动到碰撞点
				vec3 sweep_movement = (remaining_movement_direction * std::max(0.f, closest_sweep_hit_distance - kCollisionOffset));
				tmp_moved_position += sweep_movement;
				remaining_movement_magnitude -= glm::length(sweep_movement);

				//评估碰撞点是否稳定
				HitStabilityReport move_hit_stability_report = EvaluateHitStability(
					closest_sweep_hit_rigid,
					closeset_sweep_hit_normal,
					closest_sweep_hit_point,
					tmp_moved_position,
					transient_rotation_);

				//处理高于底部胶囊半径的步进点
				//处理走台阶
				bool found_valid_step_hit = false;
				if (solve_grounding && character_->support_step && move_hit_stability_report.vaild_step_detected)
				{
					float obstruction_correlation = std::abs(glm::dot(closeset_sweep_hit_normal, character_up_));
					if (obstruction_correlation <= kCorrelationForVerticalObstruction)
					{
						vec3 step_forward_direction = Normalize(ProjectOnPlane(-closeset_sweep_hit_normal, character_up_));
						vec3 step_cast_start_point = (tmp_moved_position + (step_forward_direction * kSteppingForwardDistance)) +
							(character_up_ * character_->max_step_height);

						//从踏步陶都的顶部向下投射
						PxSweepHit closest_step_hit;
						auto hits = CharacterCollisionsSweep(
							step_cast_start_point,
							transient_rotation_,
							-character_up_,
							character_->max_step_height,
							closest_step_hit,
							0.f,
							true);

						//检查与台阶碰撞对应的命中
						for (auto& hit : hits)
						{
							if (hit.actor == move_hit_stability_report.stepped_collider)
							{
								vec3 end_step_position = step_cast_start_point + (-character_up_ * (hit.distance - kCollisionOffset));
								tmp_moved_position = end_step_position;
								found_valid_step_hit = true;

								transient_velocity = ProjectOnPlane(transient_velocity, character_up_);
								remaining_movement_direction = Normalize(transient_velocity);

								break;
							}
						}
					}
				}

				if (!found_valid_step_hit)
				{
					vec3 obstruction_noraml = GetObstructionNormal(closeset_sweep_hit_normal, move_hit_stability_report.is_stable);

					// todo
					// Movement hit callback
					//CharacterController.OnMovementHit(closestSweepHitCollider, closestSweepHitNormal, closestSweepHitPoint, ref moveHitStabilityReport);

					//记录下命中的刚体
					if (character_->interactive_rigid_body_handling && closest_sweep_hit_rigid->is<PxRigidDynamic>())
					{
						StoreRigidbodyHit(
							closest_sweep_hit_rigid->is<PxRigidDynamic>(),
							transient_velocity,
							closest_sweep_hit_point,
							obstruction_noraml,
							move_hit_stability_report.is_stable);
					}

					bool stable_on_hit = move_hit_stability_report.is_stable && !MustUnground();
					vec3 velocity_before_proj = transient_velocity;
					//下次迭代速度
					InternalHandleVelocityProjection(
						stable_on_hit,
						closeset_sweep_hit_normal,
						obstruction_noraml,
						original_velocity_direction,
						sweep_state,
						previous_hit_is_srtable,
						previous_veloctiy,
						previous_obstruction_normal,
						transient_velocity,
						remaining_movement_magnitude,
						remaining_movement_direction);
					previous_hit_is_srtable = stable_on_hit;
					previous_veloctiy = velocity_before_proj;
					previous_obstruction_normal = obstruction_noraml;
				}
			}
			else
			{
				//如果没用碰撞到
				hit_something_this_sweep_iteration = false;
			}

			++sweeps_made;
			if (sweeps_made > character_->max_movement_iterations)
			{
				if (character_->kill_remaining_movement_when_exceed_max_movement_iterations)
				{
					remaining_movement_magnitude = 0.f;
				}

				if (character_->kill_velocity_when_exceed_max_movement_iterations)
				{
					transient_velocity = vec3(0);
				}

				was_completed = false;
			}
		}

		//移动剩余移动的位置
		tmp_moved_position += (remaining_movement_direction * remaining_movement_magnitude);
		transient_position_ = tmp_moved_position;

		return was_completed;
	}

	// 检测到命中时处理运动投影
	void InternalHandleVelocityProjection(bool stable_on_hit, vec3 hit_normal, vec3 obstruction_normal, vec3 original_direction,
		MovementSweepState& sweep_state, bool previous_hit_is_stable, vec3 previous_velocity, vec3 previous_obstruction_normal,
		vec3& transient_velocity, float& remaining_movement_magnitude, vec3& remaining_movement_direction)
	{
		if (glm::dot(transient_velocity, transient_velocity) <= 0.f)
		{
			return;
		}

		vec3 veloctiy_before_projection = transient_velocity;
		if (stable_on_hit)
		{
			character_movement_->last_movement_iteration_found_any_ground = true;
			HandleVelocityProjection(transient_velocity, obstruction_normal, stable_on_hit);
			
		}
		else 
		{
			if (sweep_state == MovementSweepState::kInitial)
			{
				HandleVelocityProjection(transient_velocity, obstruction_normal, stable_on_hit);
				sweep_state = MovementSweepState::kAfterFirstHit;
			}
			else if (sweep_state == MovementSweepState::kAfterFirstHit)
			{
				// 处理折痕(高低起伏处或拐角)
				bool found_crease;
				vec3 crease_direction;

				EvaluateCrease(
					transient_velocity,
					previous_velocity,
					obstruction_normal,
					previous_obstruction_normal,
					stable_on_hit,
					previous_hit_is_stable,
					character_movement_->grounding_status.is_stable_on_ground && !MustUnground(),
					found_crease,
					crease_direction);

				if (found_crease)
				{
					if (character_movement_->grounding_status.is_stable_on_ground && !MustUnground())
					{
						transient_velocity = vec3(0);
						sweep_state = MovementSweepState::kFoundBlockingCorner;
					}
					else
					{
						transient_velocity = Project(transient_velocity, crease_direction);
						sweep_state = MovementSweepState::kFoundBlockingCrease;
					}
				}
				else
				{
					HandleVelocityProjection(transient_velocity, obstruction_normal, stable_on_hit);
				}
			}
			else if (sweep_state == MovementSweepState::kFoundBlockingCrease)
			{
				transient_velocity = vec3(0);
				sweep_state = MovementSweepState::kFoundBlockingCorner;
			}
		}

		float new_velocity_factor = glm::length(transient_velocity) / glm::length(veloctiy_before_projection);
		remaining_movement_magnitude *= new_velocity_factor;
		remaining_movement_direction = Normalize(transient_velocity);
	}

	// 评估折痕
	void EvaluateCrease(
		vec3 current_character_velocity,
		vec3 previous_character_velocity,
		vec3 current_hit_nomral,
		vec3 previous_hit_normal,
		bool current_hit_is_stable,
		bool previous_hit_is_stable,
		bool character_is_stable,
		bool& is_valid_carease,
		vec3& crease_direction)
	{
		is_valid_carease = false;
		crease_direction = vec3(0);

		if (!character_is_stable || !current_hit_is_stable || !previous_hit_is_stable)
		{
			vec3 tmp_blocking_crease_direction = Normalize(glm::cross(current_hit_nomral, previous_hit_normal));
			float dot_planes = glm::dot(current_hit_nomral, previous_hit_normal);
			bool is_velocity_constrained_by_crease = false;

			// 如果两个平面相同,请避免计算
			if (dot_planes < 0.999f)
			{
				vec3 normal_a_on_crease_plane = Normalize(ProjectOnPlane(current_hit_nomral, tmp_blocking_crease_direction));
				vec3 normal_b_on_crease_plane = Normalize(ProjectOnPlane(previous_hit_normal, tmp_blocking_crease_direction));
				float dot_planes_on_crease_plane = glm::dot(normal_a_on_crease_plane, normal_b_on_crease_plane);

				vec3 entering_velocity_direction_on_crease_plane = Normalize(ProjectOnPlane(previous_character_velocity, tmp_blocking_crease_direction));
				
				if (dot_planes_on_crease_plane <= (glm::dot(-entering_velocity_direction_on_crease_plane, normal_a_on_crease_plane) + 0.001f) &&
					dot_planes_on_crease_plane <= (glm::dot(-entering_velocity_direction_on_crease_plane, normal_b_on_crease_plane) + 0.001f))
				{
					is_velocity_constrained_by_crease = true;
				}
			}

			if (is_velocity_constrained_by_crease)
			{
				//翻转折痕方向,时期代表我们的速度投射到真实方向
				if (glm::dot(tmp_blocking_crease_direction, current_character_velocity) < 0.f)
				{
					tmp_blocking_crease_direction = -tmp_blocking_crease_direction;
				}

				is_valid_carease = true;
				crease_direction = tmp_blocking_crease_direction;
			}
			
		}
	}

	//将速度投射到障碍物上
	void HandleVelocityProjection(vec3& velocity, vec3 obstruction_normal, bool stable_on_hit)
	{
		if (character_movement_->grounding_status.is_stable_on_ground && !MustUnground())
		{
			//在稳定的斜坡上，只需重新调整运动方向，不会有任何损失
			if (stable_on_hit)
			{
				velocity = GetDirectionTangentToSurface(velocity, obstruction_normal) * glm::length(velocity);
			}
			else
			{
				// 沿接地平面在障碍物上投影移动
				vec3 obstruction_right_along_ground = Normalize(glm::cross(obstruction_normal, character_movement_->grounding_status.ground_normal));
				vec3 obstruction_up_along_ground = Normalize(glm::cross(obstruction_right_along_ground, obstruction_normal));
				
				velocity = GetDirectionTangentToSurface(velocity, obstruction_up_along_ground) * glm::length(velocity);
				velocity = ProjectOnPlane(velocity, obstruction_normal);
			}
		}
		else
		{
			if (stable_on_hit)
			{
				velocity = ProjectOnPlane(velocity, character_up_);
				velocity = GetDirectionTangentToSurface(velocity, obstruction_normal) * glm::length(velocity);
			}
			else
			{
				velocity = ProjectOnPlane(velocity, obstruction_normal);
			}
		}
	}

	// 返回调整为相对于角色向上反向于指定表面发现相切的方向
	vec3 GetDirectionTangentToSurface(vec3 direction, vec3 surface_normal)
	{
		vec3 right = glm::cross(direction, character_up_);
		return Normalize(glm::cross(surface_normal, right));
	}

	// 根据当前接地状态获取运动障碍的有效法线
	vec3 GetObstructionNormal(vec3 hit_normal, bool stable_on_hit)
	{
		vec3 obstruction_normal = hit_normal;

		// 如果角色已经稳定在地面上此时碰到切没有强制分离地面，且不能站在碰撞体上
		// 则调整移动方向为地面发现和碰撞修正方向的叉乘，水平移出碰撞体，防止角色插入地面
		if (character_movement_->grounding_status.is_stable_on_ground && !MustUnground() && !stable_on_hit)
		{
			vec3 obstruction_left_along_ground = Normalize(glm::cross(character_movement_->grounding_status.ground_normal, obstruction_normal));
			obstruction_normal = Normalize(glm::cross(obstruction_left_along_ground, character_up_));
		}
		
		// 验证有效性
		if (glm::dot(obstruction_normal,obstruction_normal) == 0.f)
		{
			obstruction_normal = hit_normal;
		}

		return obstruction_normal;
	}

	// 确定给定方向是否可以稳定
	bool IsStableOnNormal(vec3 normal)
	{
		float v1 = glm::degrees(glm::acos(glm::dot(normal, character_up_)));
		int v2 = v1;
		return v1 <= character_->max_stable_slope_angle;
	}

	vec3 GetVelocityFromMovement(vec3 movement, float dt)
	{
		if (dt <= 0.f)
		{
			return vec3(0);
		}

		return movement / dt;
	}

	bool MustUnground()
	{
		return false;
	}

	// 确定在给定碰撞点是否可以站稳
	HitStabilityReport EvaluateHitStability(PxRigidActor* actor, vec3 hit_normal, vec3 hit_point, vec3 at_character_position, quat at_character_rotation)
	{
		HitStabilityReport stability_report;

		if (!solve_grounding)
		{
			stability_report.is_stable = false;
			return stability_report;
		}

		vec3 at_character_up = at_character_rotation * cached_world_up;
		vec3 inner_hit_direction = Normalize(ProjectOnPlane(hit_normal, at_character_up));

		stability_report.is_stable = IsStableOnNormal(hit_normal);
		stability_report.inner_normal = hit_normal;
		stability_report.outer_noraml = hit_normal;

		// 
		if (character_->support_step && !stability_report.is_stable)
		{
			if (!(actor && actor->is<PxRigidDynamic>() && !(actor->is<PxRigidDynamic>()->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)))
			{
				DetectSteps(at_character_position, at_character_rotation, hit_point,inner_hit_direction,stability_report);
				
				if (stability_report.vaild_step_detected)
				{
					stability_report.is_stable = true;
				}
			}
		}

		return stability_report;
	}

	void DetectSteps(vec3 character_position, quat character_rotation, vec3 hit_point, vec3 inner_hit_direction, HitStabilityReport& stability_report)
	{
		PxRigidActor*  tmp_rigid;
		PxSweepHit outer_step_hit;
		vec3 character_up = character_rotation * cached_world_up;
		vec3 vertical_char_to_hit = Project((hit_point - character_position), character_up);
		vec3 horizontal_char_to_hit_direction = Normalize(ProjectOnPlane((hit_point - character_position), character_up));
		vec3 step_check_start_pos = (hit_point - vertical_char_to_hit) + (character_up * character_->max_step_height) + (horizontal_char_to_hit_direction * kCollisionOffset * 3.f);

		//
		auto hits = CharacterCollisionsSweep(
			step_check_start_pos,
			character_rotation,
			-character_up,
			character_->max_step_height + kCollisionOffset,
			outer_step_hit,
			0.f,
			true);

		// 检查碰撞位置有无重叠和障碍物
		if (CheckStepValidity(hits, character_position, character_rotation, inner_hit_direction, step_check_start_pos, tmp_rigid))
		{
			stability_report.vaild_step_detected = true;
			stability_report.stepped_collider = tmp_rigid;
		}
	}

	//  检查台阶有效性
	bool CheckStepValidity(std::vector<PxSweepHit>& hits, vec3 character_position, quat character_rotation, vec3 inner_hit_direction, vec3 step_check_start_pos, PxRigidActor*& hit_rigid)
	{
		hit_rigid = nullptr;

		vec3 character_up = character_rotation * vec3(0, 1, 0);
		
		//找到最远的有效碰撞
		bool found_valid_step_position = false;

		while (!hits.empty() && !found_valid_step_position)
		{
			//寻找距离最远的碰撞
			PxSweepHit farthest_hit;
			float farthest_distance = 0.f;
			auto farthest_it = hits.end();
			for (auto it = hits.begin();it != hits.end();++it)
			{
				auto& hit = *it;
				if (hit.distance > farthest_distance)
				{
					farthest_distance = hit.distance;
					farthest_hit = hit;
					farthest_it = it;
				}
			}

			vec3 character_position_at_hit = step_check_start_pos + (-character_up * (farthest_distance - kCollisionOffset));
			auto overlaps = CharacterCollisionsOverlap(character_position_at_hit, character_rotation);
			if (overlaps.size() <= 0)
			{
				PxRaycastHit outer_slope_hit;
				if (CharacterCollisionsRaycast(
					PhysicsUtility::Vec3(farthest_hit.position) + (character_up * kSecondaryProbesVertical) + (-inner_hit_direction * kSecondaryProbesHorizontal),
					-character_up,
					character_->max_step_height + kSecondaryProbesVertical,
					outer_slope_hit,
					true).size() > 0)
				{
					if (IsStableOnNormal(PhysicsUtility::Vec3(outer_slope_hit.normal)))
					{
						PxSweepHit tmp_up_obstruction_hit;
						// 向上投射胶囊体检测是否有碰撞
						if (CharacterCollisionsSweep(
							character_position,
							character_rotation,
							character_up,
							character_->max_step_height - farthest_hit.distance,
							tmp_up_obstruction_hit).empty())
						{
							bool inner_step_valid = false;
							PxRaycastHit inner_step_hit;
							
							if (character_->allow_stepping_without_stable_grounding)
							{
								inner_step_valid = true;
							}
							else
							{
								//在台阶高度的胶囊中心
								if (CharacterCollisionsRaycast(
									character_position + Project((character_position_at_hit - character_position), character_up),
									-character_up,
									character_->max_step_height,
									inner_step_hit,
									true).size() > 0)
								{
									if (IsStableOnNormal(PhysicsUtility::Vec3(inner_step_hit.normal)))
									{
										inner_step_valid = true;
									}
								}
							}

							if (!inner_step_valid)
							{
								// 在步进点内部
								if (CharacterCollisionsRaycast(
									PhysicsUtility::Vec3(farthest_hit.position) + (inner_hit_direction * kSecondaryProbesHorizontal),
									-character_up,
									character_->max_step_height,
									inner_step_hit,
									true).size() > 0)
								{
									if (IsStableOnNormal(PhysicsUtility::Vec3(inner_step_hit.normal)))
									{
										inner_step_valid = true;
									}
								}
							}

							if (inner_step_valid)
							{
								hit_rigid = farthest_hit.actor;
								found_valid_step_position = true;
								return true;
							}
						}
					}
				}
			}

			if (!found_valid_step_position && farthest_it != hits.end())
			{
				hits.erase(farthest_it);
			}
		}
		
		return false;
	}

	// Casts the character volume in the character's downward direction to detect ground
	// 在角色向下的方向投射角色体积以检测地面
	bool CharacterGroundSweep(vec3 position, quat rotation, vec3 direction, float distance,PxSweepHit& closest_hit)
	{
		// Capsule cast
		PxSweepHit hit[kMaxHitsBudget];
		PxSweepBuffer hit_callback(hit, kMaxHitsBudget);
		PxQueryFilterData filter_data(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePREFILTER);
		IgnoreTiggerQueryCallback filter_callback;
		auto& physics_world = registry.ctx<PhysicsWorld>();
		physics_world.scene->sweep(px_caspule_, PhysicsUtility::Transform(position, rotation) * shape_transform_, PhysicsUtility::Vec3(direction), distance, hit_callback, PxHitFlag::eDEFAULT, filter_data, &filter_callback);


		// Hits filter
		bool found_valid_hit = false;
		float closest_distance = std::numeric_limits<float>::infinity();

		for (int i = 0; i < hit_callback.getNbTouches(); ++i)
		{
			const PxSweepHit& hit = hit_callback.getTouch(i);
			if (hit.distance > 0.f && CheckIfColliderValidForCollisions(hit.actor))
			{
				if (hit.distance < closest_distance)
				{
					closest_hit = hit;
					closest_distance = hit.distance;
					found_valid_hit = true;
				}
			}
		}

		return found_valid_hit;
	}

	// 射线探测
	std::vector<PxRaycastHit> CharacterCollisionsRaycast(vec3 position, vec3 direction, float distance, PxRaycastHit& closest_hit, bool accept_only_stable_ground_layer = false)
	{
		int query_layers = collidable_layers;
		if (accept_only_stable_ground_layer)
		{
			query_layers = character_->stable_ground_layers & query_layers;
		}

		PxRaycastHit hit[kMaxCollisionBudget];
		PxRaycastBuffer hit_callback(hit, kMaxCollisionBudget);
		PxQueryFilterData filter_data(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePREFILTER);
		IgnoreTiggerQueryCallback filter_callback;
		auto& physics_world = registry.ctx<PhysicsWorld>();
		physics_world.scene->raycast(PhysicsUtility::Vec3(position), PhysicsUtility::Vec3(direction), distance, hit_callback, PxHitFlag::eDEFAULT, filter_data, &filter_callback);
		
		float closest_distance = std::numeric_limits<float>::infinity();

		std::vector<PxRaycastHit> hits;
		hits.reserve(hit_callback.getNbTouches());

		for (int i = 0; i < hit_callback.getNbTouches(); ++i)
		{
			auto& hit = hit_callback.getTouch(i);
			float hit_distance = hit.distance;
			if (hit_distance > 0 && CheckIfColliderValidForCollisions(hit.actor))
			{
				hits.push_back(hit);

				if (hit_distance < closest_distance)
				{
					closest_hit = hit;
					closest_distance = hit_distance;
				}
			} 
		}

		return hits;
	}

	std::vector<PxSweepHit> CharacterCollisionsSweep(vec3 position, quat rotation, vec3 direction, float distance, PxSweepHit& closest_hit, float inflate = 0.f, bool accept_only_stable_ground_layer = false)
	{
		int query_layers = collidable_layers;
		if (accept_only_stable_ground_layer)
		{
			query_layers = character_->stable_ground_layers & query_layers;
		}

		PxSweepHit hit[kMaxHitsBudget];
		PxSweepBuffer hit_callback(hit, kMaxHitsBudget);
		PxQueryFilterData filter_data(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePREFILTER);
		IgnoreTiggerQueryCallback filter_callback;
		auto& physics_world = registry.ctx<PhysicsWorld>();
		physics_world.scene->sweep(px_caspule_, PhysicsUtility::Transform(position, rotation) * shape_transform_, PhysicsUtility::Vec3(direction), distance, hit_callback, PxHitFlag::eDEFAULT, filter_data, &filter_callback);

		// Hits filter
		float closest_distance = std::numeric_limits<float>::infinity();

		std::vector<PxSweepHit> hits;
		hits.reserve(hit_callback.getNbTouches());

		for (int i = 0; i < hit_callback.getNbTouches(); ++i)
		{
			const PxSweepHit& hit = hit_callback.getTouch(i);
			if (hit.distance > 0.f && CheckIfColliderValidForCollisions(hit.actor))
			{
				hits.push_back(hit);
				if (hit.distance < closest_distance)
				{
					closest_hit = hit;
					closest_distance = hit.distance;
				}
			}
		}

		return hits;
	}

	// 检测角色胶囊是否与任何可碰撞的物体重叠
	std::vector<PxRigidActor*> CharacterCollisionsOverlap(vec3 position, quat rotation, float inflate = 0.f, bool accept_only_stable_ground_layer = false)
	{
		int query_layers = collidable_layers;
		if (accept_only_stable_ground_layer)
		{
			query_layers = character_->stable_ground_layers & query_layers;
		}

		PxOverlapHit hit[kMaxCollisionBudget];
		PxOverlapBuffer hit_callback(hit, kMaxCollisionBudget);
		PxQueryFilterData filter_data(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePREFILTER);
		IgnoreTiggerQueryCallback filter_callback;
		auto& physics_world = registry.ctx<PhysicsWorld>();
		physics_world.scene->overlap(px_caspule_, PhysicsUtility::Transform(position, rotation) * shape_transform_, hit_callback, filter_data, &filter_callback);

		std::vector<PxRigidActor*> collisions;
		collisions.reserve(hit_callback.getNbTouches());

		for (int i = 0; i < hit_callback.getNbTouches(); ++i)
		{
			if (CheckIfColliderValidForCollisions(hit_callback.getTouch(i).actor))
			{
				collisions.push_back(hit_callback.getTouch(i).actor);
			}
		}

		return collisions;
	}

	// 确认输入的碰撞对碰撞器对碰撞处理是否有效
	bool CheckIfColliderValidForCollisions(PxRigidActor* rigid)
	{
		if (rigid == rigid_)
		{
			return false;
		}

		if (!InternalIsColliderValidForCollisions(rigid))
		{
			return false;
		}

		return true;
	}

	// 确认输入的碰撞器对碰撞处理是否有效
	bool InternalIsColliderValidForCollisions(PxRigidActor* rigid)
	{
		if (rigid)
		{
			auto rigid_dynamic = rigid->is<PxRigidDynamic>();
			if (rigid_dynamic)
			{
				bool is_rigidbody_kinematic = rigid_dynamic->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC;

				// 如果移动来自接AttachedRigidbody则忽略他
				if (is_moving_from_attached_rigidbody && (!is_rigidbody_kinematic || rigid_dynamic == character_movement_->attached_rigidbody))
				{
					return false;
				}

				if (character_->rigidbody_interaction_type == RigidbodyInteractionType::kKinematic && !is_rigidbody_kinematic)
				{
					// wake up rigidbody
					rigid_dynamic->wakeUp();

					return false;
				}
			}

		}

		return true;
	}

private:
	// 角色在其移动计算中的目标位置（在角色更新阶段始终是最新的）
	vec3 transient_position_;
	// 角色在其移动计算中的目标旋转（在角色更新阶段始终是最新的）
	quat transient_rotation_;

	vec3 character_up_;
	vec3 character_forward_;

	// 到目前为止在角色更新期间检测到的重叠数量
	int overlaps_count_;

	// 到目前位置
	std::array<OverlapResult, kMaxRigidbodyOverlapsCount> overlaps_;

	// 缓存当前控制的角色信息
	entt::entity e_;
	Character* character_;
	CharacterMovement* character_movement_;
	PxCapsuleGeometry px_caspule_;
	PxTransform shape_transform_;
	PxTransform px_transform_;
	PxRigidActor* rigid_;
};
