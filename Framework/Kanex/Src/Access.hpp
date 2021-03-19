#pragma once

#include <type_traits>

namespace kanex
{
	/*ͨ����access����Ϊfriend����¶���private����*/
	class Access
	{
	public:
		// ͨ�����÷����ƶ��ж�T�Ƿ���ڳ�Ա����Serialize
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
