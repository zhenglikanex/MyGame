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
		typedef uint32_t actor_id_type;
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
		actor_id_type actor_id() const { return actor_id_; }
		proto_id_type proto_id() const { return proto_id_; }
		size_type cur_size() const { return cur_size_; }
		size_type size() const { return size_; }

		void set_session(session_type session) { session_ = session; }
		void set_actor_id(actor_id_type actor_id) { actor_id_ = actor_id; }
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
		session_type session_;			//4 byte
		actor_id_type actor_id_;		//4 byte
		proto_id_type proto_id_;		//4 byte
		size_type size_;				//4 byte

		data_type data_;
		size_type cur_size_;
	};
}