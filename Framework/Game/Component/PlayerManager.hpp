#pragma once

#include <unordered_map>

#include "Framework/Game/Component/LocalId.hpp"

struct PlayerData
{
	uint32_t id;
	LocalId local_id_generator;

	PlayerData() = default;
	PlayerData(uint32_t _id,LocalId _local_id_generator)
		: id(_id)
		, local_id_generator(_local_id_generator)
	{
		
	}
};

struct PlayerManager
{
	std::unordered_map<uint32_t,PlayerData> players;
};