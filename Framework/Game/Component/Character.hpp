#pragma once

#include <cstdint>

#include "Framework/Game/Math.hpp"

enum class RigidbodyInteractionType
{
    kNone,
    kKinematic,
    kSimulateDynamic
};

// 用来标记角色移动扫略中碰撞的状态
enum class MovementSweepState
{
    kInitial,
    kAfterFirstHit,
    kFoundBlockingCrease,
    kFoundBlockingCorner
};

using LayerMask = uint32_t;

// 接地状态的所有信息
struct CharacterGroundingReport
{
	
};

struct Character
{
    /**************************************
    ***************地面相关设置*************
    ***************************************/

    //地面监测的额外距离，加快监测到地面的速度
    float ground_detection_extra_distance = 0.f;

    // 角色可以稳定的最大坡度
    float max_stable_slope_angle = 60.f;

    // 角色可以站稳的碰撞层
    LayerMask stable_ground_layers = ~0;


    /**************************************
    ***************走台阶相关设置************
    ***************************************/

    //是否支持走台阶
    bool support_step = true;

    // 角色可以走的最大台阶高度
    float max_step_height = 0.5f;

    // 即使上面站不住角色也要走吗
    bool allow_stepping_without_stable_grounding = true;


    /**************************************
    ***************刚体交互设置************
    ***************************************/

    // 角色如何与dynamic rigidbody交互
    RigidbodyInteractionType rigidbody_interaction_type = RigidbodyInteractionType::kKinematic;

    // 模拟角色推动物体的质量
    float simulated_character_mass = 1.f;

    // 确定角色在脱离地面时是否保留移动平台的速度
    bool preserve_attached_rigidbody_momentum = true;


    /**************************************
    ***************约束相关设置************
    ***************************************/

    //确定角色的移动是否使用平面约束
    bool has_planar_constraint = false;

	// 限制角色移动的平面(仅在启用平面约束时生效)
	vec3 planar_constraint_axis = vec3(0, 0, 1);

	
	/**************************************
	***************其他设置****************
	***************************************/

	// 每次更新可以扫描所少次移动
	uint32_t max_movement_iterations = 5;

	// 每次更新可以检查多少次防支持碰撞冲突
	uint32_t max_decollision_iterations = 1;

	// 在投射移动之前检查重叠，确保即使已经相交的几何体也能检测到所有碰撞（具有性能成本，但可以防止隧道穿过碰撞器）
	bool check_movement_initial_overlaps = true;

	//如果超过最大移动迭代次数，则将速度设置为零
	bool kill_velocity_when_exceed_max_movement_iterations = true;

	// 如果超过最大移动迭代次数，则将剩余移动设置为零
	bool kill_remaining_movement_when_exceed_max_movement_iterations = true;
};