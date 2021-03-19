#pragma once

#include <type_traits>

namespace kanex
{
	/*通过将access声明为friend来暴露类的private函数*/
	class Access
	{
	public:
		// 通过后置返回推断判断T是否存在成员函数Serialize
		template<class Archive,class T>
		static auto Serialize(Archive& ar,T& data) -> decltype((void)data.Serialize(std::declval<Archive&>()))
		{
			data.Serialize(ar);
		}

		template<class Archive, class T>
		static auto Save(Archive& ar,const T& data) -> decltype((void)data.Save(std::declval<Archive&>()))
		{
			data.Save(ar);
		}

		template<class Archive, class T>
		static auto Load(Archive& ar, T& data) -> decltype((void)data.Load(std::declval<Archive&>()))
		{
			data.Load(ar);
		}
	};
}
