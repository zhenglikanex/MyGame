#pragma once

#include <string>
#include "Framework/Game/Component/UniqueId.hpp"

#include "Kanex.hpp"

struct ParentAnimBone
{
	UniqueId target;
	std::string bone;

	ParentAnimBone() = default;

	ParentAnimBone(UniqueId _target,std::string_view _bone)
		: target(_target)
		, bone(_bone)
	{

	}
};

NON_MEMEBER_BAR(ParentAnimBone, obj.target, obj.bone);