#pragma once

#include <cstdint>
#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"

//用来标记需要在所有客户端服务器之间同步的对象
//拥有全局唯一id
struct Remote
{
    struct Null
	{
        [[nodiscard]] operator Remote() const noexcept 
		{
            return static_cast<Remote>(std::numeric_limits<uint32_t>::max());
        }

        [[nodiscard]] constexpr bool operator==([[maybe_unused]] const Null other) const noexcept 
		{
            return true;
        }

        [[nodiscard]] constexpr bool operator!=([[maybe_unused]] const Null other) const noexcept 
		{
            return false;
        }

        [[nodiscard]] bool operator==(const Remote id) const noexcept 
		{
			return id == static_cast<Remote>(*this);
        }

        [[nodiscard]] bool operator!=(const Remote id) const noexcept
		{
            return !(id == *this);
        }
		
    };

	static constexpr Null null{};

	uint32_t id;

	Remote() = default;

    explicit Remote(uint32_t _value)
		: id(_value)
	{
		
	}

    explicit Remote(entt::entity e)
		: id(static_cast<uint32_t>(e))
	{

	}

	bool operator==(Remote other) const
	{
		return id == other.id;
	}

	bool operator!=(Remote other) const
	{
		return id != other.id;
	}
};

NON_MEMEBER_BAR(Remote, obj.id);

[[nodiscard]] inline bool operator==(const Remote id, const Remote::Null other) noexcept
{
	return other.operator==(id);
}

[[nodiscard]] inline bool operator!=(const Remote id, const Remote::Null other) noexcept 
{
	return !(other == id);
}

namespace std
{
	template<>
	struct hash<Remote>
	{
		_NODISCARD size_t operator()(const Remote& id) const noexcept {
			return id.id;
		}
	};
}