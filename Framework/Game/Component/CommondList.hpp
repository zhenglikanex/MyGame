#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/Component/Commond.hpp"

struct CommondGroup
{
	std::unordered_map<uint32_t, std::shared_ptr<Commond>> commonds;

	bool operator==(const CommondGroup& commonds)
	{
		// todo:
		return true;
	}
};