#include "Message.h"

namespace network
{
	void Message::Decode(Message& message, uint8_t* data)
	{
		uint32_t cur_size = 0;

		Message::session_type session;
		std::memcpy(&session, data + cur_size, sizeof(session));
		cur_size += sizeof(session);
		message.set_session(session);
		
		Message::actor_id_type actor_id;
		std::memcpy(&actor_id, data + cur_size, sizeof(actor_id));
		cur_size += sizeof(actor_id);
		message.set_actor_id(actor_id);

		Message::proto_id_type proto_id;
		std::memcpy(&proto_id, data + cur_size, sizeof(proto_id));
		cur_size += sizeof(proto_id);
		message.set_proto_id(proto_id);

		Message::size_type size;
		std::memcpy(&size, data + cur_size, sizeof(size));
		cur_size += sizeof(size);
		message.set_size(size);
	}

	void Message::Encode(void* data, const Message& message)
	{
		std::memcpy(data, &message, kHeaderSize + message.size());
	}

	Message::Message()
		:session_(0)
		, actor_id_(0)
		, proto_id_(0)
		, size_(0)
		, cur_size_(0)
	{
	}

	/*Message::Message(session_type session, moudle_id_type moudle_id, area_id_type area_id, proto_id_type proto_id, size_type size, void* data)
		: session_(session)
		, moudle_id_(moudle_id)
		, area_id_(area_id)
		, proto_id_(proto_id)
		, size_(size_)
		, cur_size_(0)
	{
		if (size_ <= kBodySize) {
			std::memcpy(&data_, data, size_);
		}
	}

	Message::Message(const Message& message)
		: session_(message.session_)
		, moudle_id_(message.moudle_id_)
		, area_id_(message.moudle_id_)
		, proto_id_(message.proto_id_)
		, size_(message.size_)
		, data_(message.data_)
		, cur_size_(message.cur_size_)
	{
	}

	Message::Message(Message&& message)
		: session_(message.session_)
		, moudle_id_(message.moudle_id_)
		, area_id_(message.moudle_id_)
		, proto_id_(message.proto_id_)
		, size_(message.size_)
		, data_(std::move(message.data_))
		, cur_size_(message.cur_size_)
	{
	}

	Message& Message::operator=(const Message& message) 
	{
		session_	= message.session_;
		moudle_id_	= message.moudle_id_;
		area_id_	= message.moudle_id_;
		proto_id_	= message.proto_id_;
		size_		= message.size_;
		std::memcpy(&data_,&message.data_, message.size_);
		cur_size_	= message.cur_size_;

		return *this;
	}

	Message& Message::operator=(Message&& message)
	{
		session_	= message.session_;
		moudle_id_	= message.moudle_id_;
		area_id_	= message.moudle_id_;
		proto_id_	= message.proto_id_;
		size_		= message.size_;
		data_		= std::move(message.data_);
		data_		= message.data_;
		cur_size_ = message.cur_size_;

		return *this;
	}*/

	void Message::Write(const void* data,uint32_t size)
	{
		if (size_ + size > kBodySize) {
			return;
		}

		std::memcpy(data_.data() + size_, data, size);
		size_ += size;
	}

	void Message::Read(void* out, uint32_t size)
	{
		if (cur_size_ + size > size_) {
			return;
		}

		std::memcpy(out, data_.data() + cur_size_, size);
	}

	void Message::Clear()
	{
		session_ = 0;
		actor_id_ = 0;
		proto_id_ = 0;
		size_ = 0;
		cur_size_ = 0;

		// ÖØÖÃÄÚ´æ¿é£»
		std::memset(data_.data(),0,data_.size());
	}
}