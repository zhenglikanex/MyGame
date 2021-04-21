#include "Actor.h"

#include <functional>

#include "ActorNet.h"

#include <iostream>

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
				// todo log
				return;
			}
		}
		else if(actor_msg.type() == ActorMessage::MessageType::kTypeTimer)
		{			
			auto& callback = std::any_cast<std::function<void()>>(actor_msg.data());
			if (callback)
			{
				callback();
			}

			return;
		}
		else if (actor_msg.type() == ActorMessage::MessageType::kTypeActor)
		{
			auto iter = actor_handlers_.find(actor_msg.name());
			if (iter != actor_handlers_.end())
			{
				iter->second(actor_msg.data());
			}

			return;
		}
		else if (actor_msg.type() == ActorMessage::MessageType::kTypeRequest)
		{
			auto iter = request_handlers_.find(actor_msg.name());
			if (iter != request_handlers_.end())
			{
				actor_net_->SendActorMessage(
					actor_msg.dest_id(), 
					actor_msg.src_id(),
					actor_msg.session(),
					ActorMessage::MessageType::kTypeResponse,
					"",
					iter->second(actor_msg.data()));
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

	ActorId Actor::StartActor(const std::string& lib_path)
	{
		return actor_net_->StartActor(lib_path);
	}

	ActorId Actor::StartUniqueActor(const std::string& lib_path, const std::string& name)
	{
		return actor_net_->StartUniqueActor(lib_path, name);
	}

	void Actor::KillActor(ActorId id)
	{
		actor_net_->KillActor(id);
	}

	void Actor::KillActor(const std::string& name)
	{
		actor_net_->KillActor(name);
	}

	uint32_t Actor::AddTimer(uint32_t millisec, int32_t repeat, const std::function<void()>& callback)
	{
		return actor_net_->AddTimer(id_, millisec, repeat, callback);
	}

	void Actor::CancelTimer(uint32_t id)
	{
		actor_net_->CancelTimer(id);
	}
}