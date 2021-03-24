#pragma once

#include <type_traits>
#include <set>
#include <unordered_set>

#include "Internal/Traits.hpp"
#include "Internal/Types.hpp"

namespace kanex
{
	template<class Archive,class T,class ... Args>
	std::enable_if_t<is_output_serializable<Archive, T>::value>
		Save(Archive& ar,const std::set<T,Args...>& set)
	{
		static_assert(std::is_default_constructible_v<T>, "key_type not default_construct!");

		ar(static_cast<detail::size_type>(set.size()));
		for (auto& v : set)
		{
			ar(v);
		}
	}

	template<class Archive,class T, class ... Args>
	std::enable_if_t<is_input_serializable<Archive, T>::value>
		Load(Archive& ar, std::set<T, Args...>& set)
	{
		static_assert(std::is_default_constructible_v<T>,"key_type not default_construct!");

		detail::size_type size;
		ar(size);

		for (detail::size_type i = 0; i < size; ++i)
		{
			T value;
			ar(value);
			set.emplace(std::move(value));
		}
	}

	template<class Archive, class T, class ... Args>
	std::enable_if_t<is_output_serializable<Archive, T>::value>
		Save(Archive& ar, const std::unordered_set<T, Args...>& set)
	{
		static_assert(std::is_default_constructible_v<T>, "key_type not default_construct!");

		ar(static_cast<detail::size_type>(set.size()));
		for (auto& v : set)
		{
			ar(v);
		}
	}

	template<class Archive, class T, class ... Args>
	std::enable_if_t<is_input_serializable<Archive, T>::value>
		Load(Archive& ar, std::unordered_set<T, Args...>& set)
	{
		static_assert(std::is_default_constructible_v<T>, "key_type not default_construct!");

		detail::size_type size;
		ar(size);
		set.reserve(size);

		for (detail::size_type i = 0; i < size; ++i)
		{
			T value;
			ar(value);
			set.emplace(std::move(value));
		}
	}

	template<class Archive, class T, class ... Args>
	std::enable_if_t<is_output_serializable<Archive, T>::value>
		Save(Archive& ar, const std::unordered_multiset<T, Args...>& set)
	{
		static_assert(std::is_default_constructible_v<T>, "key_type not default_construct!");

		ar(static_cast<detail::size_type>(set.size()));
		for (auto& v : set)
		{
			ar(v);
		}
	}

	template<class Archive, class T, class ... Args>
	std::enable_if_t<is_input_serializable<Archive, T>::value>
		Load(Archive& ar, std::unordered_multiset<T, Args...>& set)
	{
		static_assert(std::is_default_constructible_v<T>, "key_type not default_construct!");

		detail::size_type size;
		ar(size);

		for (detail::size_type i = 0; i < size; ++i)
		{
			T value;
			ar(value);
			set.emplace(std::move(value));
		}
	}
}