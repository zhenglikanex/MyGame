#pragma once

#include <unordered_map>

#include "Framework/Game/Component/Local.hpp"

struct SyncInstance
{
	uint32_t owner;
	uint32_t identifier;

	SyncInstance() = default;
	SyncInstance(uint32_t _owner, uint32_t _identifier)
		: owner(_owner)
		, identifier(_identifier)
	{
		
	}
};

struct SyncManager
{
	std::unordered_map<uint32_t,SyncInstance> sync_instances;
};