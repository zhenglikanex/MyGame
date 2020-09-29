#pragma once

#include "Framework/Game/Math.hpp"

struct Position
{
	vec3 pos;
	vec3 forward;
	BAR(pos,forward);
};