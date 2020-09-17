#pragma once

#include <map>
#include <unordered_map>

#include "serialize/internal/types.hpp"
#include "serialize/traits.hpp"
#include "serialize/types/pair.hpp"

namespace serialize
{
	template<class Archive, template<class...> class Map,class K,class V,class ... Args,typename = typename Map<K,V,Args...>::mapped_type>
	std::enable_if_t<is_output_serializable<Archive,std::pair<K, V>>::value>	
		Save(Archive& ar,const Map<K,V,Args...>& map)
	{
		static_assert(std::is_default_constructible_v<K> && std::is_default_constructible_v<V>, "K or V not default_construct!");

		ar(static_cast<detail::size_type>(map.size()));
		for (auto& p : map)
		{
			ar(p);
		}
	}

	template<class Archive, template<class...> class Map, class K, class V, class ... Args, typename = typename Map<K, V, Args...>::mapped_type>
	std::enable_if_t<is_input_serializable<Archive, std::pair<K, V>>::value>
		Load(Archive& ar,Map<K,V,Args...>& map)
	{
		static_assert(std::is_default_constructible_v<K> && std::is_default_constructible_v<V>, "K or V not default_construct!");

		detail::size_type size;
		ar(size);

		auto iter = map.begin();
		for (detail::size_type i = 0; i < size; ++i)
		{
			K key;
			V value;
			ar(key);
			ar(value);

			// µ¹Ðò²åÈë
			iter = map.emplace_hint(iter, std::move(key), std::move(value));
		}
	}
}
