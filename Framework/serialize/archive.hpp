#pragma once

#include <type_traits>

#include "serialize/internal/noncopy.hpp"
#include "serialize/traits.hpp"
#include "serialize/access.hpp"

namespace serialize
{
	/* CRPTģʽ*/
	template<class ArchiveType>
	class OutputArchive : public Noncopy
	{
	public:
		OutputArchive(ArchiveType* self) : self_(self) { }
		virtual ~OutputArchive() = 0 { }

		template<class ... Args>
		std::enable_if_t<is_output_serializable<ArchiveType,Args...>::value, ArchiveType&>
			operator()(Args&& ... args)
		{
			Serialize(std::forward<Args>(args)...);
			return *self_;
		}
	private:
		//ͨ��T�ĳ�Ա����serialize
		template<class T>
		std::enable_if_t<has_member_Serialize<T, ArchiveType>::value,void>
			SerializeImpl(const T& data)		//��Ϊconstʹ֮�ܴ�����ʱ����������ֵ
		{
			access::Serialize(*self_, const_cast<T&>(data));	//�Ƴ�costʹoutput��input�ܹ���serialze����
		}

		//ͨ���ǳ�Ա����serialize
		template<class T>
		std::enable_if_t<has_non_member_Serialize<T, ArchiveType>::value, void>
			SerializeImpl(const T& data)	//��Ϊconstʹ֮�ܴ�����ʱ����������ֵ
		{
			serialize::Serialize(*self_,const_cast<T&>(data));	//�Ƴ�costʹoutput��input�ܹ���serialze����
		}

		//ͨ���ǳ�Ա����Save
		template<class T>
		std::enable_if_t<has_non_member_Save<T,ArchiveType>::value,void>
			SerializeImpl(const T& data)
		{
			Save(*self_, data);
		}

		template<class T>
		void Serialize(T&& data)
		{
			self_->SerializeImpl(std::forward<T>(data));
		}

		template<class First,class ... Other>
		void Serialize(First&& data,Other&& ... other)
		{
			self_->Serialize(std::forward<First>(data));
			self_->Serialize(std::forward<Other>(other)...);
		}

		ArchiveType* self_;
	};

	template<class ArchiveType>
	class InputArchive : public Noncopy
	{
	public:
		InputArchive(ArchiveType* self) : self_(self) { }
		virtual ~InputArchive() = 0 { }

		template<class ... Args>
		std::enable_if_t<is_input_serializable<ArchiveType, Args...>::value, ArchiveType&>
			operator()(Args&& ... args)
		{
			Serialize(std::forward<Args>(args)...);
			return *self_;
		}
	private:
		//ͨ��T�ĳ�Ա����serialize
		template<class T>
		std::enable_if_t<has_member_Serialize<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			access::Serialize(*self_, data);
		}

		//ͨ���ǳ�Ա����serialize
		template<class T>
		std::enable_if_t<has_non_member_Serialize<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			serialize::Serialize(*self_, data);
		}

		//ͨ���ǳ�Ա����Save
		template<class T>
		std::enable_if_t<has_non_member_Load<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			Load(*self_, data);
		}

		template<class T>
		void Serialize(T&& data)
		{
			// ���ﲻת��,data���������,��Ϊһ����ֵ,��������Load�����е���ʱ������MakeBinaryData
			self_->SerializeImpl(data);
		}

		template<class First, class ... Other>
		void Serialize(First&& data, Other&& ... other)
		{
			self_->Serialize(std::forward<First>(data));
			self_->Serialize(std::forward<Other>(other)...);
		}

		ArchiveType* self_;
	};
}