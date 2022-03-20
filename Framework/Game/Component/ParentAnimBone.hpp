#pragma once

#include <string>
#include "Framework/Game/Component/Remote.hpp"

#include "Kanex.hpp"

struct ParentAnimBone
{
	Remote target;
	std::string bone;

	ParentAnimBone() = default;

	ParentAnimBone(Remote _target,std::string_view _bone)
		: target(_target)
		, bone(_bone)
	{

	}
};

NON_MEMEBER_BAR(ParentAnimBone, obj.target, obj.bone);