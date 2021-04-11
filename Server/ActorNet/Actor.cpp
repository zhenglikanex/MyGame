#include "Actor.h"

#include "ActorNet.h"

namespace actor_net
{
	void Actor::OnReceive(ActorMessage&& actor_msg)
	{
		if (actor_msg.type() == ActorMessage::MessageType::kTypeResponse)
		{
			if (request_callbacks_[actor_msg.session()])
			{
				request_callbacks_[actor_msg.session()](std::move(actor_msg));
				request_callbacks_[actor_msg.session()] = nullptr;
				return;
			}
			else 
			{
				// todo log;
				return;
			}
		}

		Receive(std::move(actor_msg));
	}

	void Actor::Request(ActorId dest_id, const std::string_view func_name, std::any&& data, const std::function<void(ActorMessage&&)>& callback)
	{
		request_callbacks_[session_id_] = callback;
		actor_net_->SendActorMessage(id_, dest_id, session_id_, ActorMessage::MessageType::kTypeRequest, func_name, std::move(data));
		session_id_ = session_id_ == kMaxSession ? 0 : ++session_id_;
	}

	void Actor::Request(const std::string& actor_name, const std::string_view func_name, std::any&& data, const std::function<void(ActorMessage&&)>& callback)
	{
		request_callbacks_[session_id_] = callback;
		actor_net_->SendActorMessage(id_, actor_name, session_id_, ActorMessage::MessageType::kTypeRequest, func_name, std::move(data));
		session_id_ = session_id_ == kMaxSession ? 0 : ++session_id_;
	}

	void Actor::Call(ActorId dest_id, const std::string_view func_name, std::any&& data)
	{
		actor_net_->SendActorMessage(id_, dest_id, 0, ActorMessage::MessageType::kTypeActor, func_name, std::move(data));
	}

	void Actor::NewActor(const std::string& name, const std::function<void(ActorId)>& callback)
	{
		Request("Start", "", std::any(), [callback](ActorMessage&& actor_msg)
			{
				auto actor_id = std::any_cast<ActorId>(actor_msg.data());
				callback(actor_id);
			});
	}

}