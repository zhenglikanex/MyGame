#include <unordered_map>
#include <cstdint>

#include "Framework/Game/Component/Transform.hpp"

#include "Kanex.hpp"

struct ActorData
{
	Transform transform;
};

NON_MEMEBER_BAR(ActorData, obj.transform);

struct FrameData
{
	uint32_t frame;
	std::unordered_map<uint32_t, ActorData> actors;
};

NON_MEMEBER_BAR(FrameData, obj.frame, obj.actors);