#pragma once

#include <cstdint>

#include "Kanex.hpp"

//全局唯一的id
struct UniqueId
{
	uint32_t value;

	UniqueId(uint32_t _value)
		: value(_value)
	{

	}

	bool operator==(UniqueId other) const
	{
		return value == other.value;
	}

	bool operator!=(UniqueId other) const
	{
		return value != other.value;
	}
};

NON_MEMEBER_BAR(UniqueId, obj.value);

namespace std
{
	template<>
	struct hash<UniqueId>
	{
		_NODISCARD size_t operator()(const UniqueId& id) const noexcept {
			return id.value;
		}
	};
}