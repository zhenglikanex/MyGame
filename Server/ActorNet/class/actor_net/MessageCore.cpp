#include "MessageCore.h"

namespace actor_net
{
	// ����һ��������Ϣ
	void MessageCore::PushActorMessage(actor_id id,const ActorMessagePtr& actor_message)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		
		// ��ȡ�������Ϣ����
		auto actor_msg_queue_ptr = GetActorMsgQueue(id);
		actor_msg_queue_ptr->second.push(actor_message);
	}

	// ˳�򵯳�һ��ActorMessage
	ActorMessagePtr MessageCore::PopActorMessage()
	{
		std::unique_lock<std::mutex> lock(mutex_);

		// ѭ������������Ϣ����
		for (size_t i = 0; i < actor_msg_queues_.size(); ++i)
		{
			// ����һ���������
			auto actor_msg_queue_ptr = actor_msg_queues_.back();
			actor_msg_queues_.pop();
			
			// ��������з���ĩβ
			actor_msg_queues_.push(actor_msg_queue_ptr);

			// ����һ����Ϣ
			auto actor_message = actor_msg_queue_ptr->second.back();
			actor_msg_queue_ptr->second.pop();

			return actor_message;
		}

		return ActorMessagePtr();
	}

	ActorMsgQueuePtr MessageCore::GetActorMsgQueue(actor_id id)
	{
		auto iter = id_by_queue_map_.find(id);
		if (iter != id_by_queue_map_.end())
		{
			return iter->second;
		}

		// ����������Ϣ����
		auto actor_msg_queue_ptr = std::make_shared<std::pair<actor_id, MessageQueue>>(std::make_pair(id, MessageQueue()));

		actor_msg_queues_.push(actor_msg_queue_ptr);
		id_by_queue_map_.insert(std::make_pair(id, actor_msg_queue_ptr));

		return actor_msg_queue_ptr;
	}
}