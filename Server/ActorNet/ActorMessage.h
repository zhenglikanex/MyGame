#ifndef ACTORE_MESSAGE_H_
#define ACTORE_MESSAGE_H_

#include <memory>
#include <vector>
#include <any>

#include "IActor.h"

namespace actor_net
{
	// 服务之间消息
	class ActorMessage
	{
	public:
		enum class MessageType : uint8_t
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
		{

		}

		ActorMessage(ActorId src_id, ActorId dest_id, MessageType type, session_type session,std::any&& data)
			: src_id_(src_id)
			, dest_id_(dest_id)
			, type_(type)
			, session_(session)
			, data_(std::move(data))
		{

		}

		ActorMessage(ActorMessage&& rhs) noexcept
			: src_id_(rhs.src_id_)
			, dest_id_(rhs.dest_id_)
			, type_(rhs.type_)
			, session_(rhs.session_)
			, data_(std::move(rhs.data_))
		{

		}

		ActorMessage& operator=(ActorMessage&& rhs) noexcept
		{
			if (&rhs == this)
			{
				return *this;
			}

			src_id_ = rhs.src_id_;
			dest_id_ = rhs.dest_id_;
			type_ = rhs.type_;
			session_ = rhs.session_;
			data_(std::move(rhs.data_));
		}

		ActorMessage(const ActorMessage&) = delete;
		ActorMessage& operator=(const ActorMessage&) = delete;

		ActorId src_id() const { return src_id_; }
		ActorId dest_id() const { return dest_id_; }
		MessageType type() const { return type_; }
		session_type session() const { return session_; }

		void set_src_id(ActorId src_id) { src_id_ = src_id; }
		void set_dest_id(ActorId dest_id) { dest_id_ = dest_id; }
		void set_type(MessageType type) { type_ = type; }
		void set_session(session_type session) { session_ = session; }
	private:
		ActorId src_id_;
		ActorId dest_id_;
		MessageType type_;
		session_type session_;
		std::any data_;
	};
}

#endif