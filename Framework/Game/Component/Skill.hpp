#pragma once

#include <vector>

#include "Framework/Game/Math.hpp"

#include "3rdparty/include/entt/entt.hpp"

// �������ƶ���ʽ
//1.����(�ض����ƶ�,ֱ��ָ��Target��������֮�࣬�����buff)
//	1.Skill + Target ������boundingbox(kLockTarget) 
//2.������(ͨ��BoundingBox�����ײĿ��,��̬���Target)
	//1.���ƶ�Skill + BoundingBox + Transform(kNotLockFixedPoint)
	//2.���ݷ����ٶ��ƶ� Skill + BoundingBox + Transform + Movement(kNotLockMovment)
	//3.���ݼ��ܶ�����ָ���Ĺ����ƶ� Skill + BoundingBox + Tranform + SkillAttacthBone(kNotLockAnim)
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
