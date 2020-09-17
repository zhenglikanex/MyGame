#pragma once

#include <cstdint>
#include <array>
#include <memory>
#include <iostream>

#include "Message.h"

namespace network
{
	class Message
	{
	public:
		static const uint8_t kHeaderSize = 16;
		static const uint32_t kBodySize = 1024 * 64;	//64KB

		typedef uint32_t session_type;
		typedef uint16_t moudle_id_type;
		typedef uint16_t area_id_type;
		typedef uint32_t proto_id_type;
		typedef uint32_t size_type;

		typedef std::array<uint8_t, kBodySize> data_type;

		static void Decode(Message& message,uint8_t* data);
		static void Encode(void* data,const Message& message);

		Message();
		/*Message(
			session_type session,
			moudle_id_type moudle_id,
			area_id_type area_id,
			proto_id_type proto_id,
			size_type size,
			void* data);

		Message(const Message& message);
		Message(Message&& message);
		Message& operator=(const Message& Message);
		Message& operator=(Message&& message);*/

		session_type session() const { return session_; }
		moudle_id_type moudle_id() const { return moudle_id_; }
		area_id_type area_id() const { return area_id_; }
		proto_id_type proto_id() const { return proto_id_; }
		size_type cur_size() const { return cur_size_; }
		size_type size() const { return size_; }

		void set_session(session_type session) { session_ = session; }
		void set_moudle_id(moudle_id_type moudle_id) { moudle_id_ = moudle_id; }
		void set_area_id(area_id_type area_id) { area_id_ = area_id; }
		void set_proto_id(proto_id_type proto_id) { proto_id_ = proto_id; }
		void set_size(size_type size) { size_ = size; }

		void Write(const void* data,uint32_t size);
		void Read(void* out, uint32_t size);

		const std::array<uint8_t, kBodySize>& data() const
		{
			return data_;
		}

		void Clear();
	private:
		// message header
		session_type session_;			//2 byte
		moudle_id_type moudle_id_;		//2 byte
		area_id_type area_id_;			//2 byte
		proto_id_type proto_id_;		//4 byte
		size_type size_;				//4 byte

		data_type data_;
		size_type cur_size_;
	};
}