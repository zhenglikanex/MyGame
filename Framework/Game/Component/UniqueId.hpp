#pragma once

#include <cstdint>

#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"

//用来标记需要在所有客户端服务器之间同步的对象
//拥有全局唯一id
struct UniqueId
{
	uint32_t value;

	UniqueId() = default;

	UniqueId(uint32_t _value)
		: value(_value)
	{
		
	}

	UniqueId(entt::entity e)
		: value(static_cast<uint32_t>(e))
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