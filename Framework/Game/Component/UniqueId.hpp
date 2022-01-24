#pragma once

#include <cstdint>

#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"

//用来标记需要在所有客户端服务器之间同步的对象
//拥有全局唯一id
struct UniqueId
{
    struct Null 
	{
        [[nodiscard]] constexpr operator UniqueId() const noexcept 
		{
            return std::numeric_limits<uint32_t>::max();
        }

        [[nodiscard]] constexpr bool operator==([[maybe_unused]] const Null other) const noexcept 
		{
            return true;
        }

        [[nodiscard]] constexpr bool operator!=([[maybe_unused]] const Null other) const noexcept 
		{
            return false;
        }

        [[nodiscard]] constexpr bool operator==(const UniqueId id) const noexcept 
		{
			return id == static_cast<UniqueId>(*this);
        }

        template<typename Entity>
        [[nodiscard]] constexpr bool operator!=(const Entity entity) const noexcept 
		{
            return !(entity == *this);
        }
    };

	static constexpr Null null{};

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

[[nodiscard]] inline constexpr bool operator==(const UniqueId id, const UniqueId::Null other) noexcept
{
	return other.operator==(id);
}

[[nodiscard]] inline constexpr bool operator!=(const UniqueId entity, const UniqueId::Null other) noexcept 
{
	return !(other == entity);
}

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