#pragma once

#include <type_traits>
#include <set>
#include <unordered_set>

#include "Internal/Traits.hpp"
#include "Internal/Types.hpp"

namespace kanex
{
	template<class Archive,template<class...> class Set,class ... Args>
	std::enable_if_t<is_output_serializable<Archive,typename Set<Args...>::key_type>::value
					&& is_same_or_v<Set<Args...>,std::set<Args...>,std::multiset<Args...>,std::unordered_set<Args...>>>
		Save(Archive& ar,const Set<Args...>& set)
	{
		static_assert(std::is_default_constructible_v<typename Set<Args...>::key_type>, "key_type not default_construct!");

		ar(static_cast<detail::size_type>(set.size()));
		for (auto& v : set)
		{
			ar(v);
		}
	}

	template<class Archive, template<class...> class Set, class ... Args>
	std::enable_if_t<is_input_serializable<Archive, typename Set<Args...>::key_type>::value
		&& is_same_or_v<Set<Args...>, std::set<Args...>, std::multiset<Args...>, std::unordered_set<Args...>>>
		Load(Archive& ar,Set<Args...>& set)
	{
		static_assert(std::is_default_constructible_v<typename Set<Args...>::key_type>,"key_type not default_construct!");

		detail::size_type size;
		ar(size);

		auto iter = set.begin();
		for (detail::size_type i = 0; i < size; ++i)
		{
			typename Set<Args...>::key_type value;
			ar(value);
			set.emplace_hint(iter, std::move(value));
		}
	}
}