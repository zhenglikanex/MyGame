#ifndef ACTORE_MESSAGE_H_
#define ACTORE_MESSAGE_H_

#include <memory>
#include <vector>

#include "IActor.h"

namespace actor_net
{
	// 服务之间消息
	class ActorMessage
	{
	public:
		enum MessageType
		{
			kMsgTypeSystem,		// 系统消息(即主线程发过来的消息)
			kMsgTypeActor,		// Actor之间的消息
			kMsgTypeResponse,	// Response消息
		};

		typedef uint32_t session_type;
		typedef uint32_t size_type;

		ActorMessage()
			:src_id_(0)
			, dest_id_(0)
			, type_(MessageType::kMsgTypeActor)
			, session_(0)
			, cur_size_(0)
		{

		}

		actor_id src_id() { return src_id_; }
		actor_id dest_id() { return dest_id_; }
		MessageType type() { return type_; }
		session_type session() const { return session_; }
		size_t size() { return data_.size(); }
		const std::vector<uint8_t>& data() { return data_; }

		void set_src_id(actor_id src_id) { src_id_ = src_id; }
		void set_dest_id(actor_id dest_id) { dest_id_ = dest_id; }
		void set_type(MessageType type) { type_ = type; }
		void set_session(session_type session) { session_ = session; }

		void Write(const void* data, uint32_t size)
		{
			data_.resize(data_.size() + size);
			std::memcpy(data_.data(), data, size);
		}
		void Read(void* out, uint32_t size)
		{
			if (cur_size_ >= data_.size() || cur_size_ + size >= data_.size())
			{
				return;
			}

			std::memcpy(out, data_.data() + cur_size_, size);
		}
	private:
		actor_id src_id_;
		actor_id dest_id_;
		MessageType type_;
		session_type session_;
		std::size_t cur_size_;
		std::vector<uint8_t> data_;
	};

	typedef std::shared_ptr<ActorMessage> ActorMessagePtr;
}

#endif
