#include "MessageCore.h"

namespace actor_net
{
	MessageQueue::MessageQueue(ActorId id)
		: actor_id_(id)
	{

	}

	bool MessageQueue::IsEmpty() const 
	{
		return messages_.empty();
	}

	void MessageQueue::Push(ActorMessage&& message)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		messages_.push(std::move(message));
	}

	ActorMessage MessageQueue::Pop()
	{
		std::unique_lock<std::mutex> lock(mutex_);

		assert(!IsEmpty() && "mesage queue is empty !");

		auto message = std::move(messages_.front());
		messages_.pop();
		return message;
	}

	bool MessageCore::IsEmtpy() const
	{
		return actor_msg_queues_.empty();
	}

	void MessageCore::PushActorMessage(ActorId id,ActorMessage&& actor_message)
	{
		auto queue = GetMessageQueueById(id);
		if (queue->IsEmpty())
		{
			actor_msg_queues_.push(queue);
		}

		queue->Push(std::move(actor_message));
	}

	void MessageCore::PushMessgeQueue(const std::shared_ptr<MessageQueue>& queue)
	{
		std::unique_lock lock(mutex_);
		actor_msg_queues_.push(queue);
	}

	std::shared_ptr<actor_net::MessageQueue> MessageCore::PopMessageQueue()
	{
		std::unique_lock lock(mutex_);

		if (actor_msg_queues_.empty())
		{
			return nullptr;
		}

		auto queue = actor_msg_queues_.front();
		actor_msg_queues_.pop();
		return queue;
	}

	void MessageCore::RemoveMessageQueueById(ActorId id)
	{
		std::unique_lock lock(mutex_);
		id_by_queue_map_.erase(id);
	}

	std::shared_ptr<MessageQueue> MessageCore::GetMessageQueueById(ActorId id)
	{
		std::unique_lock lock(mutex_);

		auto iter = id_by_queue_map_.find(id);
		if (iter != id_by_queue_map_.end())
		{
			return iter->second;
		}

		auto queue = std::make_shared<MessageQueue>(id);
		actor_msg_queues_.push(queue);
		id_by_queue_map_.emplace(id, queue);

		return queue;
	}

	

}