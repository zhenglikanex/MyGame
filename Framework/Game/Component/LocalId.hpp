#pragma once

#include <cstdint>
#include <unordered_map>

#include "Kanex.hpp"

//本地预测生成对象的id
struct LocalId
{
	uint32_t value;

	LocalId() = default;

	LocalId(uint32_t _value)
		: value(_value)
	{

	}

	bool operator==(const LocalId& other) const
	{
		return value == other.value;
	}

	bool operator!=(const LocalId& other) const
	{
		return !(*this == other);
	}
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
