#pragma once

#include <type_traits>

namespace serialize
{
	/*ͨ����access����Ϊfriend����¶���private����*/
	class access
	{
	public:
		// ͨ�����÷����ƶ��ж�T�Ƿ���ڳ�Ա����Serialize
		template<class Archive,class T>
		static auto Serialize(Archive& ar,T& data) -> decltype((void)data.Serialize(std::declval<Archive&>()))
		{
			data.Serialize(ar);
		}
	};
}
