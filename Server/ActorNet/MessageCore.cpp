#include "MessageCore.h"

namespace actor_net
{
	MessageQueue::MessageQueue(ActorId id)
		: actor_id_(id)
		, managed_(false)
	{

	}

	bool MessageQueue::IsEmpty() 
	{
		std::unique_lock<std::mutex> lock(mutex_);
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
		auto message =  std::move(messages_.front());
		messages_.pop();
		return message;
	}

	void MessageCore::PushActorMessage(ActorId id,ActorMessage&& actor_message)
	{
		std::unique_lock<std::mutex> lock(mutex_);

		auto queue = GetMessageQueueById(id);
		if (!queue->managed())
		{
			PushMessgeQueue(queue);
		}

		queue->Push(std::move(actor_message));
	}

	void MessageCore::PushMessgeQueue(const std::shared_ptr<MessageQueue>& queue)
	{
		queue->set_mamaged(true);
		actor_msg_queues_.push(queue);
	}

	std::shared_ptr<actor_net::MessageQueue> MessageCore::PopMessageQueue()
	{
		if (actor_msg_queues_.empty())
		{
			return nullptr;
		}

		auto queue = actor_msg_queues_.front();
		actor_msg_queues_.pop();
		return queue;
	}

	std::shared_ptr<MessageQueue> MessageCore::GetMessageQueue(std::shared_ptr<MessageQueue> queue)
	{
		std::unique_lock lock(mutex_);
		
		//如果还有其他队列,放回等待下一次调度,否则继续使用这个队列
		if (queue && !actor_msg_queues_.empty())
		{
			PushMessgeQueue(queue);
		}

		while (true)
		{
			if (queue && !queue->IsEmpty())
			{
				return queue;
			}

			//丢弃空队列,等待有消息在放回,防止线程空转
			if (queue && queue->IsEmpty())
			{
				queue->set_mamaged(false);
			}

			queue = PopMessageQueue();

			if (!queue)
			{
				break;
			}
		}

		return nullptr;
	}

	void MessageCore::RemoveMessageQueueById(ActorId id)
	{
		std::unique_lock lock(mutex_);
		id_by_queue_map_.erase(id);
	}

	std::shared_ptr<MessageQueue> MessageCore::GetMessageQueueById(ActorId id)
	{		
		auto iter = id_by_queue_map_.find(id);
		if (iter != id_by_queue_map_.end())
		{
			return iter->second;
		}

		auto queue = std::make_shared<MessageQueue>(id);
		id_by_queue_map_.emplace(id, queue);

		return queue;
	}
}