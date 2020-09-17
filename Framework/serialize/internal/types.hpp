#pragma once

#include <type_traits>

namespace serialize
{
	namespace detail
	{
		// ������װvector��array֮��Ķ���������
		template<class T>
		struct BinaryData
		{
			using value_type = std::remove_pointer_t<T>;
			using pointer_type = std::conditional_t<std::is_const<std::remove_pointer_t<std::remove_cv_t<T>>>::value,
													const void*,
													void*>;

			BinaryData(pointer_type _data,size_t _size) :data(_data),size(_size){}

			pointer_type data;
			size_t size;
		};

		template<class T>
		BinaryData<T> MakeBinaryData(T&& data, size_t size)
		{
			return { data,size };
		}

		// ����std������size����,��֤ͳһ
		using size_type = size_t;
	}
}
