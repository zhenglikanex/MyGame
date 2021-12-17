#pragma once

#include <cstdint>
#include <unordered_map>

//本地预测生成对象的id
struct LocalId
{
	uint32_t value;
};

//记录玩家分配出去的id
struct LocalIdByPlayerMap
{
	std::unordered_map<uint32_t, uint32_t> value;
};