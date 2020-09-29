#pragma once

#include "fpm/fixed.hpp"
#include "fpm/math.hpp"
#include "Framework/serialize/binary_archive.hpp"

using fixed16 = fpm::fixed_16_16;

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

