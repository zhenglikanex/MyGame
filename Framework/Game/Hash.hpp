#pragma once

#include <cstdint>

namespace details
{
	constexpr int64_t i64(const char* s, int64_t v)
	{
		return *s ? i64(s + 1, v * 256 + *s) : v;
	}

	constexpr uint64_t u64(const char* s, uint64_t v)
	{
		return *s ? u64(s + 1, v * 256 + *s) : v;
	}

    constexpr int32_t i32(const char* s,int32_t v)
    {
		return *s ? i32(s + 1, v * 256 + *s) : v;
    }
    
	constexpr uint32_t u32(const char* s, uint32_t v)
	{
		return *s ? u32(s + 1, v * 256 + *s) : v;
	}

	constexpr int16_t i16(const char* s, int16_t v)
	{
		return *s ? i16(s + 1, v * 256 + *s) : v;
	}

	constexpr uint16_t u16(const char* s, uint16_t v)
	{
		return *s ? u16(s + 1, v * 256 + *s) : v;
	}
}

constexpr int64_t operator ""_i64(const char* s,size_t)
{
	return details::i64(s, 0);
}

constexpr uint64_t operator ""_u64(const char* s, size_t)
{
	return details::u64(s, 0);
}

constexpr int32_t operator ""_i32(const char* s, size_t)
{
	return details::i32(s, 0);
}

constexpr uint32_t operator ""_u32(const char* s, size_t)
{
	return details::u32(s, 0);
}

constexpr int16_t operator ""_i16(const char* s, size_t)
{
	return details::i16(s, 0);
}

constexpr uint16_t operator ""_u16(const char* s, size_t)
{
	return details::u16(s, 0);
}