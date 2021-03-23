#pragma once

#include <type_traits>

#include "Internal/Traits.hpp"
#include "Internal/Types.hpp"

namespace kanex
{
	template<class Archive, class T, size_t Size>
	std::enable_if_t<is_output_serializable<Archive, detail::BinaryData<T>>::value
		&& std::is_arithmetic_v<T>>
		Save(Archive& ar, const std::array<T, Size>& array)
	{
		ar(detail::MakeBinaryData(array.data(), sizeof(T) * Size));
	}

	template<class Archive, class T, size_t Size>
	std::enable_if_t<is_input_serializable<Archive, detail::BinaryData<T>>::value
		&& std::is_arithmetic_v<T>>
		Load(Archive& ar,std::array<T, Size>& array)
	{
		ar(detail::MakeBinaryData(array.data(), sizeof(T) * Size));
	}

	template<class Archive, class T, size_t Size>
	std::enable_if_t<is_output_serializable<Archive, T>::value
		&& !std::is_arithmetic_v<T>>
		Save(Archive& ar, const std::array<T, Size>& array)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			ar(array[i]);
		}
	}

	template<class Archive, class T, size_t Size>
	std::enable_if_t<is_input_serializable<Archive, T>::value
		&& !std::is_arithmetic_v<T> && !std::is_same_v<T, bool>>
		Load(Archive& ar, std::array<T, Size>& array)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			ar(array[i]);
		}
	}
}