#include "MessageCore.h"

namespace actor_net
{
	// 插入一条服务消息
	void MessageCore::PushActorMessage(actor_id id,const ActorMessagePtr& actor_message)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		
		// 获取服务的消息队列
		auto actor_msg_queue_ptr = GetActorMsgQueue(id);
		actor_msg_queue_ptr->second.push(actor_message);
	}

	// 顺序弹出一条ActorMessage
	ActorMessagePtr MessageCore::PopActorMessage()
	{
		std::unique_lock<std::mutex> lock(mutex_);

		// 循环遍历所有消息队列
		for (size_t i = 0; i < actor_msg_queues_.size(); ++i)
		{
			// 弹出一个服务队列
			auto actor_msg_queue_ptr = actor_msg_queues_.back();
			actor_msg_queues_.pop();
			
			// 将这个队列放入末尾
			actor_msg_queues_.push(actor_msg_queue_ptr);

			// 弹出一个消息
			auto actor_message = actor_msg_queue_ptr->second.back();
			actor_msg_queue_ptr->second.pop();

			return actor_message;
		}

		return nullptr;
	}

	ActorMsgQueuePtr MessageCore::GetActorMsgQueue(actor_id id)
	{
		auto iter = id_by_queue_map_.find(id);
		if (iter != id_by_queue_map_.end())
		{
			return iter->second;
		}

		// 创建服务消息队列
		auto actor_msg_queue_ptr = std::make_shared<std::pair<actor_id, MessageQueue>>(std::make_pair(id, MessageQueue()));

		actor_msg_queues_.push(actor_msg_queue_ptr);
		id_by_queue_map_.insert(std::make_pair(id, actor_msg_queue_ptr));

		return actor_msg_queue_ptr;
	}
}