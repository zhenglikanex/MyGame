#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/Component/Commond.hpp"

struct CommondGroup
{
	std::unordered_map<uint32_t, Commond> value;

	bool operator==(const CommondGroup& commonds)
	{
		// todo:
		return true;
	}
};