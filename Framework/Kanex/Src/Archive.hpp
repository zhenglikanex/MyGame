#pragma once

#include <type_traits>

#include "Internal/Traits.hpp"
#include "Access.hpp"

namespace kanex
{
	/* CRPTģʽ*/
	template<class ArchiveType>
	class OutputArchive
	{
	public:
		OutputArchive(const OutputArchive&) = delete;
		OutputArchive& operator=(const OutputArchive&) = delete;

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
			Access::Serialize(*self_, const_cast<T&>(data));	//�Ƴ�costʹoutput��input�ܹ���serialze����
		}

		template<class T>
		std::enable_if_t<has_member_Save<T, ArchiveType>::value, void>
			SerializeImpl(const T& data)		//��Ϊconstʹ֮�ܴ�����ʱ����������ֵ
		{
			Access::Save(*self_,data);	
		}

		//ͨ���ǳ�Ա����serialize
		template<class T>
		std::enable_if_t<has_non_member_Serialize<T, ArchiveType>::value, void>
			SerializeImpl(const T& data)	//��Ϊconstʹ֮�ܴ�����ʱ����������ֵ
		{
			kanex::Serialize(*self_,const_cast<T&>(data));	//�Ƴ�costʹoutput��input�ܹ���serialze����
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
	class InputArchive
	{
	public:
		InputArchive(const InputArchive&) = delete;
		InputArchive& operator=(const InputArchive&) = delete;

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
			Access::Serialize(*self_, data);
		}

		//ͨ���ǳ�Ա����serialize
		template<class T>
		std::enable_if_t<has_non_member_Serialize<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			kanex::Serialize(*self_, data);
		}

		//ͨ���ǳ�Ա����Load
		template<class T>
		std::enable_if_t<has_non_member_Load<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			Load(*self_, data);
		}

		//ͨ����Ա����Load
		template<class T>
		std::enable_if_t<has_member_Load<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			Access::Load(*self_, data);
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