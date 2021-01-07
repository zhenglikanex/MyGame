#pragma once

#include "Framework/Game/Math.hpp"
#include "Framework/serialize/binary_archive.hpp"

#include "Framework/Game/Data/RootMotionConfig.hpp"

struct Movement
{
	std::unordered_map<fixed16, RootMotion> root_motions;
	
};