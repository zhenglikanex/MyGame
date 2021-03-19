#pragma once

#include <type_traits>

namespace serialize
{
	/*通过将access声明为friend来暴露类的private函数*/
	class access
	{
	public:
		// 通过后置返回推断判断T是否存在成员函数Serialize
		template<class Archive,class T>
		static auto Serialize(Archive& ar,T& data) -> decltype((void)data.Serialize(std::declval<Archive&>()))
		{
			data.Serialize(ar);
		}
	};
}
