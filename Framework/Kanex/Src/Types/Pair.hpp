#pragma once

#include <utility>

#include "Internal/Traits.hpp"

namespace kanex
{
	template<class Archive, class K, class V>
	std::enable_if_t<is_output_serializable<Archive, K, V>::value
					|| is_input_serializable<Archive, K, V>::value>
		Serialize(Archive& ar, std::pair<K, V>& pair)
	{
		ar(pair.first);
		ar(pair.second);
	}
}