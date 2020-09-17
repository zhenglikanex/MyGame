#pragma once

#include <string>

#include "serialize/internal/types.hpp"
#include "serialize/traits.hpp"

namespace serialize
{
	template<class Archive,class CharType,class CharTraits,class Alloc>
	std::enable_if_t<is_output_serializable<Archive,detail::BinaryData<CharType>>::value>
		Save(Archive& ar, const std::basic_string<CharType, CharTraits,Alloc>& str)
	{
		detail::size_type size = static_cast<detail::size_type>(str.size());
		ar(size);
		ar(detail::MakeBinaryData(str.data(), sizeof(CharType) * size));
	}

	template<class Archive, class CharType, class CharTraits, class Alloc>
	std::enable_if_t<is_input_serializable<Archive, detail::BinaryData<CharType>>::value>
		Load(Archive& ar, std::basic_string<CharType, CharTraits, Alloc>& str)
	{
		detail::size_type size;
		ar(size);
		str.resize(size);
		ar(detail::MakeBinaryData(const_cast<CharType*>(str.data()), sizeof(CharType) * size));
	}
}