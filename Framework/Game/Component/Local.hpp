#pragma once

#include <cstdint>
#include <unordered_map>

#include "Kanex.hpp"

//本地可控制(客户端可预测的/服务器所有的)entity
struct Local
{
	uint32_t owner;
	uint32_t id;

	Local() = default;

	Local(uint32_t _owner,uint32_t _id)
		: owner(_owner)
		, id(_id)
	{

	}

	bool operator==(const Local& other) const
	{
		return owner == other.owner && id == other.id;
	}

	bool operator!=(const Local& other) const
	{
		return !(*this == other);
	}
};

NON_MEMEBER_BAR(Local, obj.owner, obj.id);

namespace std
{
	template<>
	struct hash<Local>
	{
		_NODISCARD size_t operator()(const Local& local) const noexcept {
			size_t num = local.owner;
			return num << 32 | local.id;
		}
	};
}
