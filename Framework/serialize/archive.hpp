#pragma once

#include <type_traits>

#include "internal/noncopy.hpp"
#include "traits.hpp"
#include "access.hpp"

namespace serialize
{
	/* CRPT模式*/
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
		//通过T的成员函数serialize
		template<class T>
		std::enable_if_t<has_member_Serialize<T, ArchiveType>::value,void>
			SerializeImpl(const T& data)		//设为const使之能传递临时变量或字面值
		{
			access::Serialize(*self_, const_cast<T&>(data));	//移除cost使output和input能公用serialze函数
		}

		//通过非成员函数serialize
		template<class T>
		std::enable_if_t<has_non_member_Serialize<T, ArchiveType>::value, void>
			SerializeImpl(const T& data)	//设为const使之能传递临时变量或字面值
		{
			serialize::Serialize(*self_,const_cast<T&>(data));	//移除cost使output和input能公用serialze函数
		}

		//通过非成员函数Save
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
		//通过T的成员函数serialize
		template<class T>
		std::enable_if_t<has_member_Serialize<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			access::Serialize(*self_, data);
		}

		//通过非成员函数serialize
		template<class T>
		std::enable_if_t<has_non_member_Serialize<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			serialize::Serialize(*self_, data);
		}

		//通过非成员函数Save
		template<class T>
		std::enable_if_t<has_non_member_Load<T, ArchiveType>::value, void>
			SerializeImpl(T& data)
		{
			Load(*self_, data);
		}

		template<class T>
		void Serialize(T&& data)
		{
			// 这里不转发,data将变成引用,成为一个左值,用来处理Load过程中的临时变量如MakeBinaryData
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