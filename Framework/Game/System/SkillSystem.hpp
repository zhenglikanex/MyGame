#pragma once

#include "Framework/Game/Component/Skill.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/BoundingBox.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/Contact.hpp"
#include "Framework/Game/Component/Health.hpp"

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
		auto view = registry.view<Skill, SkillAttacthBone, Transform, Collider>();
		for (auto e : view)
		{
			const auto& skill = view.get<Skill>(e);
			const auto& attacth_anim_clip = view.get<SkillAttacthBone>(e);
			auto& transform = view.get<Transform>(e);
			const auto& bounding_box = view.get<Collider>(e);

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
						mat4 role_transform(owner_transform->rotation);
						role_transform[3] = vec4(owner_transform->position, fixed16(1));
						mat4 box_transform = role_transform * bone->transform;
						transform.position = box_transform[3];
						transform.rotation = quat(box_transform);
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
				if (registry.valid(contact.entity) && registry.has<ModifyHealthList>(contact.entity))
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
			const auto& skill = view.get<Skill>(e);
			for (auto target : skill.targets)
			{
				if (!registry.valid(target))
				{
					continue;
				}

				auto modify_health_list = registry.try_get<ModifyHealthList>(target);
				if (modify_health_list)
				{
					modify_health_list->value.emplace_back(std::move(GetSkillModifyHealth(skill)));
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