#include <unordered_map>
#include <cstdint>

#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Kanex.hpp"

struct ActorData
{
	Transform transform;
	AnimationClip clip;
};

NON_MEMEBER_BAR(ActorData, obj.transform,obj.clip);

struct FrameData
{
	uint32_t frame;
	std::unordered_map<uint32_t, ActorData> actors;
};

NON_MEMEBER_BAR(FrameData, obj.frame, obj.actors);