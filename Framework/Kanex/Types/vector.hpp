#pragma once

#include <type_traits>
#include <vector>

#include "serialize/traits.hpp"
#include "serialize/internal/types.hpp"

namespace serialize
{
	template<class Archive,class T,class A>
	std::enable_if_t<is_output_serializable<Archive, detail::BinaryData<T>>::value
					&& std::is_arithmetic_v<T> && !std::is_same_v<T,bool>>
		Save(Archive& ar,const std::vector<T,A>& vector)
	{
		ar(static_cast<detail::size_type>(vector.size()));
		ar(detail::MakeBinaryData(vector.data(), sizeof(T) * vector.size()));
	}

	template<class Archive,class T,class A>
	std::enable_if_t<is_input_serializable<Archive,detail::BinaryData<T>>::value
					&& std::is_arithmetic_v<T> && !std::is_same_v<T,bool>>
		Load(Archive& ar, std::vector<T,A>& vector)
	{
		detail::size_type size;
		ar(size);
		vector.resize(size);
		ar(detail::MakeBinaryData(vector.data(), sizeof(T) * vector.size()));
	}

	template<class Archive,class T,class A>
	std::enable_if_t<is_output_serializable<Archive,T>::value
					&& !std::is_arithmetic_v<T> && !std::is_same_v<T,bool>>
		Save(Archive& ar, const std::vector<T, A>& vector)
	{
		ar(static_cast<detail::size_type>(vector.size()));
		for (auto& v : vector)
		{
			ar(v);
		}
	}

	template<class Archive,class T,class A>
	std::enable_if_t<is_input_serializable<Archive,T>::value
					&& !std::is_arithmetic_v<T> && !std::is_same_v<T,bool>>
		Load(Archive& ar, std::vector<T, A>& vector)
	{
		detail::size_type size;
		ar(size);
		vector.resize(size);
		for (auto& v : vector)
		{
			ar(v);
		}
	}

	template<class Archive>
	void Save(Archive& ar, const std::vector<bool>& vector)
	{
		ar(static_cast<detail::size_type>(vector.size()));
		for (const auto v : vector) // 注意b的类型是ref的
		{
			ar(v);
		}
	}

	template<class Archive>
	void Load(Archive& ar, std::vector<bool>& vector)
	{
		detail::size_type size;
		ar(size);
		vector.resize(size);
		for (auto v : vector)	// 注意b的类型是ref的
		{
			bool b;
			ar(b);
			v = b;
		}
	}
}