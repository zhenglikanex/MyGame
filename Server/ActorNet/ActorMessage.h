#ifndef ACTORE_MESSAGE_H_
#define ACTORE_MESSAGE_H_

#include <memory>
#include <vector>
#include <any>
#include <string_view>

#include "Config.hpp"

namespace actor_net
{
	// 服务之间消息
	class ActorMessage
	{
	public:
		enum class MessageType : uint8_t
		{
			kTypeSystem,		//
			kTypeNetwork,		// socket消息
			kTypeTimer,			// 
			kTypeRequest,		// 
			kTypeResponse,		// Response消息
			kTypeActor,
		};

		typedef uint16_t SessionType;

		ActorMessage()
			: src_id_(0)
			, dest_id_(0)
			, session_(0)
			, type_(MessageType::kTypeActor)
		{

		}

		ActorMessage(ActorId src_id, ActorId dest_id, SessionType session, MessageType type,std::string_view name,std::any&& data = std::any())
			: src_id_(src_id)
			, dest_id_(dest_id)
			, session_(session)
			, type_(type)
			, name_(name)
			, data_(std::move(data))
		{

		}

		ActorMessage(ActorMessage&& rhs) noexcept
			: src_id_(rhs.src_id_)
			, dest_id_(rhs.dest_id_)
			, type_(rhs.type_)
			, session_(rhs.session_)
			, name_(std::move(rhs.name_))
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
			name_ = std::move(rhs.name_);
			data_ = std::move(rhs.data_);
		}

		ActorMessage(const ActorMessage&) = delete;
		ActorMessage& operator=(const ActorMessage&) = delete;

		ActorId src_id() const { return src_id_; }
		ActorId dest_id() const { return dest_id_; }
		SessionType session() const { return session_; }
		MessageType type() const { return type_; }
		const std::string& name() const { return name_; }
		const std::any& data() const { return data_; }

		void set_src_id(ActorId src_id) { src_id_ = src_id; }
		void set_dest_id(ActorId dest_id) { dest_id_ = dest_id; }
		void set_session(SessionType session) { session_ = session; }
		void set_type(MessageType type) { type_ = type; }
		void set_name(std::string_view name) { name_ = name; }
		void set_data(std::any&& data) { data_ = data; }
	private:
		ActorId src_id_;
		ActorId dest_id_;
		SessionType session_;
		MessageType type_;
		std::string name_;
		std::any data_;
	};
}

#endif