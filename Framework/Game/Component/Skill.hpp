#pragma once

#include <vector>

#include "Framework/Game/Math.hpp"

#include "3rdparty/include/entt/entt.hpp"

// �������ƶ���ʽ
//1.����(�ض����ƶ�,ֱ��ָ��Target��������֮�࣬�����buff)
//	1.Skill ������boundingbox
//2.������(ͨ��BoundingBox�����ײĿ��,��̬���Target)
	//1.���ƶ�Skill + BoundingBox + Transform
	//2.���ݷ����ٶ��ƶ� Skill + BoundingBox + Transform + Movement
	//3.���ݼ��ܶ�����ָ���Ĺ����ƶ� Skill + BoundingBox + Tranform + SkillAttacthBone

struct Skill
{
	entt::entity owner;
	fixed16 life;
	fixed16 time;
	std::vector<entt::entity> targets;

	Skill(entt::entity _owner, fixed16 _life, fixed16 _time)
		: owner(_owner)
		, life(_life)
		, time(_time)
	{

	}
};

struct SkillAttacthAnimationClip
{
	std::string anim_name;
	std::string bone_name;

	SkillAttacthAnimationClip(const std::string& _anim_name, const std::string& _bone_name)
		: anim_name(_anim_name)
		, bone_name(_bone_name)
	{

	}
};
