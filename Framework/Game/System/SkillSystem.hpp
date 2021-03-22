#pragma once

#include "Framework/Game/Component/Skill.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/Contact.hpp"
#include "Framework/Game/Component/Health.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"

#include "Framework/Game/System.hpp"

struct SkillSystem : public System
{
	SkillSystem(entt::registry& _registry) : System(_registry) { }
	~SkillSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		UpdateSkillTranform();
		FindSkillTarget(dt);
		HitSkill(dt);
		UpdateSkillTime(dt);
	}

	const Bone* GetBone(const Animation* animation,const AnimationClip* clip,const std::string& bone)
	{
		auto iter = animation->value->clips.find(clip->name);
		if (iter != animation->value->clips.end())
		{
			auto it = std::find_if(iter->second.skeletons.crbegin(), iter->second.skeletons.crend(), [clip](const Skeleton& skeleton)
				{
					return clip->time >= skeleton.time;
				});

			if (it != iter->second.skeletons.crend())
			{
				auto bone_it = it->bones.find(bone);
				if (bone_it != it->bones.end())
				{
					return &bone_it->second;
				}
			}
		}
		
		return nullptr;
	}

	void UpdateSkillTranform()
	{
		auto view = registry.view<Skill, SkillAttacthBone, Transform>();
		for (auto e : view)
		{
			const auto& skill = view.get<Skill>(e);
			const auto& attacth_anim_clip = view.get<SkillAttacthBone>(e);

			if (registry.valid(skill.owner))
			{
				const auto owner_transform = registry.try_get<Transform>(skill.owner);
				const auto animation = registry.try_get<Animation>(skill.owner);
				const auto anim_clip = registry.try_get<AnimationClip>(skill.owner);

				if (owner_transform && animation && anim_clip && anim_clip->name == attacth_anim_clip.anim_name)
				{
					auto bone = GetBone(animation, anim_clip, attacth_anim_clip.bone_name);
					if (bone)
					{
						auto m = GetMatrix4x4(*owner_transform) * bone->transform;
						registry.emplace_or_replace<Transform>(e,m);
					}
				}
			}
		}
	}

	void FindSkillTarget(fixed16 dt)
	{
		auto view = registry.view<Skill, ContactList>();
		for (auto e : view)
		{
			auto& skill = view.get<Skill>(e);
			const auto& contact_list = view.get<ContactList>(e);

			for (const auto& contact : contact_list.value)
			{
				if (contact.entity != skill.owner && registry.valid(contact.entity) && registry.has<ModifyHealthList>(contact.entity))
				{
					skill.targets.emplace_back(contact.entity);
				}
			}
		}
	}

	void HitSkill(fixed16 dt)
	{
		auto view = registry.view<Skill>();
		for (auto e : view)
		{
			auto& skill = view.get<Skill>(e);
			for (auto target : skill.targets)
			{
				if (registry.valid(target) && registry.has<ModifyHealthList>(target) && skill.hit_target.find(target) == skill.hit_target.end())
				{
					INFO("hit target");
					ActionStateUtility::ChangeState(registry, target, ActorStateType::kHurt);
					skill.hit_target.emplace(target);
					registry.patch<ModifyHealthList>(target, [&skill, this](ModifyHealthList &comp) 
					{ 
						comp.value.emplace_back(GetSkillModifyHealth(skill)); 
					});
				}

				//todo：添加buff
				/*for (auto buff : skill)
				{
					registry.create buff()
				}*/
			}
		}
	}

	void UpdateSkillTime(fixed16 dt)
	{
		auto view = registry.view<Skill>();
		for (auto e : view)
		{
			auto& skill = view.get<Skill>(e);

			skill.time += dt;
			if (skill.time >= skill.life)
			{
				auto collider_info = registry.try_get<ColliderInfo>(e);
				if (collider_info)
				{
					if (registry.valid(e))
					{
						registry.destroy(collider_info->collider);
					}
				}
				registry.destroy(e);
			}
		}
	}

	ModifyHealth GetSkillModifyHealth(const Skill& skill)
	{
		// todo，可以考虑放到脚本中执行
		return ModifyHealth();
	}

	void Finalize() override
	{

	}
};