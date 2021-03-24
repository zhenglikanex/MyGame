#pragma once

#include <vector>
#include <unordered_set>
#include <string_view>

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"

// �������ƶ���ʽ
//1.����(�ض����ƶ�,ֱ��ָ��Target��������֮�࣬�����buff)
//	1.Skill + Target ������ColliderInfo(kLockTarget)
//2.������(ͨ��BoundingBox�����ײĿ��,��̬���Target)
	//1.���ƶ�Skill + ColliderInfo + Transform(kNotLockFixedPoint)
	//2.���ݷ����ٶ��ƶ� Skill + ColliderInfo + Transform + Movement(kNotLockMovment)
	//3.���ݼ��ܶ�����ָ���Ĺ����ƶ� Skill + ColliderInfo + Transform + SkillAttacthBone(���Ǹ���parent)(kNotLockAnim)
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

	SkillAttacthBone() = default;

	SkillAttacthBone(std::string_view _anim_name,std::string_view _bone_name)
		: anim_name(_anim_name)
		, bone_name(_bone_name)
	{

	}
};

NON_MEMEBER_BAR(Skill, obj.owner, obj.life, obj.time, obj.targets, obj.hit_target);
NON_MEMEBER_BAR(SkillAttacthBone, obj.anim_name, obj.bone_name);