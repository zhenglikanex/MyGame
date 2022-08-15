#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/ParentAnimBone.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Animation.hpp"

#include "Framework/Game/System.hpp"

struct ComputeAnimBoneChildTransformSystem : public System
{
	ComputeAnimBoneChildTransformSystem(Registry& _registry) : System(_registry) { }
	~ComputeAnimBoneChildTransformSystem() override {}

	bool Initialize() override
	{
		return true;
	}


	const Bone* GetBone(const Animation* animation,const AnimationClip* clip,const std::string& bone)
	{
		auto iter = animation->value->clips.find(clip->name);
		if (iter != animation->value->clips.end())
		{
			//根据clip的time直接算出来
			uint32_t frame = clip->time / GameConfig::kFrameTime;

			if (frame < iter->second.skeletons.size())
			{
				auto& cur_skeleton = iter->second.skeletons[frame];
				auto it = cur_skeleton.bones.find(bone);
				if (it != cur_skeleton.bones.end())
				{
					return &it->second;
				}
			}
		}
		
		return nullptr;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<ParentAnimBone,Transform>();
		for (auto e : view)
		{
			auto& [parent, transform] = view.get<ParentAnimBone, Transform>(e);
			
			entt::entity target = registry.get(parent.target);
			if (target != entt::null)
			{
				const auto owner_transform = registry.try_get<Transform>(target);
				const auto animation = registry.try_get<Animation>(target);
				const auto anim_clip = registry.try_get<AnimationClip>(target);

				if (owner_transform && animation && anim_clip)
				{
					auto bone = GetBone(animation, anim_clip, parent.bone);
					if (bone)
					{
						auto m = GetMatrix4x4(*owner_transform) * bone->transform;
						registry.emplace_or_replace<Transform>(e, m);
					}
				}
			}
		}
	}

	void Finalize() override
	{

	}
};