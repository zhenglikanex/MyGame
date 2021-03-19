#pragma once

#include <utility>

#include "serialize/traits.hpp"

namespace serialize
{
	/*template<class Archive, class K, class V>
	std::enable_if_t<is_output_serializable<Archive, K,V>::value>
		Save(Archive& ar,const std::pair<K,V>& pair)
	{
		ar(pair.first);
		ar(pair.second);
	}

	template<class Archive, class K, class V>
	std::enable_if_t<is_input_serializable<Archive, K, V>::value>
		Load(Archive& ar,std::pair<K, V>& pair)
	{
		ar(pair.first);
		ar(pair.second);
	}*/

	template<class Archive, class K, class V>
	std::enable_if_t<is_output_serializable<Archive, K, V>::value
					|| is_input_serializable<Archive, K, V>::value>
		Serialize(Archive& ar, std::pair<K, V>& pair)
	{
		ar(pair.first);
		ar(pair.second);
	}
}