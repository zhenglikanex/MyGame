#pragma once

#include <cstdint>
#include <unordered_map>

#include "Kanex.hpp"

//本地预测生成对象的id
struct LocalId
{
	uint32_t value;
};

NON_MEMEBER_BAR(LocalId, obj.value);

namespace std
{
	template<>
	struct hash<LocalId>
	{
		_NODISCARD size_t operator()(const LocalId& id) const noexcept {
			return id.value;
		}
	};
}

//记录玩家分配出去的id
struct LocalIdByPlayerMap
{
	std::unordered_map<uint32_t, uint32_t> value;
};