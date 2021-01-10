#pragma once

#include <cassert>

#include "Framework/Game/Json.hpp"
#include "Framework/serialize/binary_archive.hpp"

#include "fpm/fixed.hpp"
#include "fpm/math.hpp"
#include "fpm/ios.hpp"

using fixed16 = fpm::fixed_16_16;

namespace fpm
{
	// from_json函数需要定义在class的命名空间
	inline void from_json(const json& j, fixed16& fixed)
	{
		float value;
		j.get_to(value);
		fixed = fixed16(value);
	}
}

template <>
struct std::is_constructible<std::string, fixed16> : std::true_type {};

namespace serialize
{
	template<class Archive, class  BaseType, class IntermediateType, unsigned int FractionBits>
	void Save(Archive& ar,const fpm::fixed<BaseType, IntermediateType, FractionBits>& fixed)
	{
		ar(fixed.raw_value());
	}

	template<class Archive, class  BaseType, class IntermediateType, unsigned int FractionBits>
	void Load(Archive& ar, fpm::fixed<BaseType, IntermediateType, FractionBits>& fixed)
	{
		BaseType raw_value;
		ar(raw_value);
		fixed = fpm::fixed<BaseType, IntermediateType, FractionBits>(raw_value);
	}
}

