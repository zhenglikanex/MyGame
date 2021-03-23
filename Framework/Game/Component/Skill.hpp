#pragma once

#include <vector>
#include <unordered_set>

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"

// 技能有移动方式
//1.锁定(必定不移动,直接指定Target比如治疗之类，自身加buff)
//	1.Skill + Target 不包含ColliderInfo(kLockTarget)
//2.不锁定(通过BoundingBox检测碰撞目标,动态添加Target)
	//1.不移动Skill + ColliderInfo + Transform(kNotLockFixedPoint)
	//2.根据方向速度移动 Skill + ColliderInfo + Transform + Movement(kNotLockMovment)
	//3.根据技能动画中指定的骨骼移动 Skill + ColliderInfo + Transform + SkillAttacthBone(考虑改用parent)(kNotLockAnim)
enum class SkillType
{
	kLockTarget,
	kNotLockFixedPoint,
	kNotLockMovment,
	kNotLockAnim
};

struct Skill
{
	entt::entity owner;
	fixed16 life;
	fixed16 time;
	std::vector<entt::entity> targets;
	std::unordered_set<entt::entity> hit_target;

	Skill() = default;

	Skill(entt::entity _owner, fixed16 _life)
		: owner(_owner)
		, life(_life)
		, time(0)
	{

	}
};

struct SkillAttacthBone
{
	std::string anim_name;
	std::string bone_name;

	SkillAttacthBone(const std::string& _anim_name, const std::string& _bone_name)
		: anim_name(_anim_name)
		, bone_name(_bone_name)
	{

	}
};

//NON_MEMEBER_BAR(Skill, obj.owner, obj.life, obj.time, obj.targets, obj.hit_target);