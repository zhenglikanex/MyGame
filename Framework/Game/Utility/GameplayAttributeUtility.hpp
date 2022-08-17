#pragma once

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

namespace details {
	constexpr int32_t i32(const char* s, int32_t v) {
		return *s ? i32(s + 1, v * 256 + *s) : v;
	}

	constexpr uint16_t u16(const char* s, uint16_t v) {
		return *s ? u16(s + 1, v * 256 + *s) : v;
	}

	constexpr uint32_t u32(const char* s, uint32_t v) {
		return *s ? u32(s + 1, v * 256 + *s) : v;
	}
}  // namespace details

constexpr int32_t operator"" _i32(const char* s, size_t) {
	return details::i32(s, 0);
}

constexpr uint32_t operator"" _u32(const char* s, size_t) {
	return details::u32(s, 0);
}

constexpr uint16_t operator"" _u16(const char* s, size_t) {
	return details::u16(s, 0);
}

class gameplay_attribute_utlity
{
public:
	static GameplayAttributeType GetAttributeType(std::string_view name)
	{
		if (strcmp(name.data(), "Hp") == 0)
		{
			return GameplayAttributeType::Hp;
		}

		if (strcmp(name.data(), "MaxHp") == 0)
		{
			return GameplayAttributeType::MaxHp;
		}

		if (strcmp(name.data(), "Mp") == 0)
		{
			return GameplayAttributeType::Mp;
		}

		if (strcmp(name.data(), "MaxMp") == 0)
		{
			return GameplayAttributeType::MaxMp;
		}

		return GameplayAttributeType::MaxCount;
	}

	static constexpr GameplayAttributeType GetAttributeType(uint32_t value)
	{
		if (value == "Hp"_u32)
		{
			return GameplayAttributeType::Hp;
		}

		if (value == "MaxHp"_u32)
		{
			return GameplayAttributeType::MaxHp;
		}

		if (value == "Mp"_u32)
		{
			return GameplayAttributeType::Mp;
		}

		if (value == "MaxMp"_u32)
		{
			return GameplayAttributeType::MaxMp;
		}

		return GameplayAttributeType::MaxCount;
	}
};