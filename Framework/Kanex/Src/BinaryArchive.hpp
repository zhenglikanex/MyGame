#pragma once

#include "Internal/Types.hpp"
#include "Archive.hpp"
#include "BinaryStream.hpp"

namespace kanex
{
	class BinaryOutputArchive : public OutputArchive<BinaryOutputArchive>
	{
	public:
		BinaryOutputArchive(BinaryStream& ostream) : OutputArchive(this), stream_(ostream) { }

		void SaveBinary(const void* data,size_t size)
		{
			stream_.buffer().Write(data, size);
		}
	private:
		BinaryStream& stream_;
	};

	class BinaryInputArchive : public InputArchive<BinaryInputArchive>
	{
	public:
		BinaryInputArchive(BinaryStream& ostream) : InputArchive(this), stream_(ostream) { }

		void LoadBinary(void* data, size_t size)
		{
			stream_.buffer().Read(data, size);
		}
	private:
		BinaryStream& stream_;
	};

	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>
		Save(BinaryOutputArchive& ar, T data)
	{
		ar.SaveBinary(&data, sizeof(data));
	}

	template<class T>
	std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>
		Load(BinaryInputArchive& ar, T& data)
	{
		ar.LoadBinary(&data, sizeof(data));
	}

	template<class T>
	void Save(BinaryOutputArchive& ar,const detail::BinaryData<T>& bd)
	{
		ar.SaveBinary(bd.data, bd.size);
	}

	template<class T>
	void Load(BinaryInputArchive& ar, detail::BinaryData<T>& bd)
	{
		ar.LoadBinary(bd.data, bd.size);
	}
}

#define BAR(...)													\
	void Serialize(kanex::BinaryOutputArchive& ar)					\
	{																\
		ar(__VA_ARGS__);											\
	}																\
																	\
	void Serialize(kanex::BinaryInputArchive& ar)					\
	{																\
		ar(__VA_ARGS__);											\
	}									

#define NON_MEMEBER_BAR(class_name,...)								\
namespace kanex {													\
inline void Serialize(kanex::BinaryOutputArchive& ar, class_name& obj)		\
{																	\
	ar(__VA_ARGS__);												\
}																	\
inline void Serialize(kanex::BinaryInputArchive& ar, class_name& obj)		\
{																	\
	ar(__VA_ARGS__);												\
}																	\
}