#ifndef MESSAGE_CORE_H_
#define MESSAGE_CORE_H_

#include <list>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <cassert>

#include "ActorMessage.h"

namespace actor_net
{
	class MessageQueue
	{
	public:
		MessageQueue(ActorId id);
		MessageQueue(const MessageQueue&) = delete;
		MessageQueue(MessageQueue&&) = delete;
		MessageQueue& operator=(const MessageQueue&) = delete;
		MessageQueue& operator=(MessageQueue&&) = delete;

		bool IsEmpty() const;
		void Push(ActorMessage&& message);
		ActorMessage Pop();

		ActorId actor_id() const { return actor_id_; }
	private:
		std::mutex mutex_;
		ActorId actor_id_;
		std::queue<ActorMessage> messages_;
	};

	class MessageCore
	{
	public:
		bool IsEmtpy() const;
		void PushActorMessage(ActorId id,ActorMessage&& actor_message);
		void PushMessgeQueue(const std::shared_ptr<MessageQueue>& queue);
		std::shared_ptr<MessageQueue> PopMessageQueue();
		void RemoveMessageQueueById(ActorId id);
	private:
		std::shared_ptr<MessageQueue> GetMessageQueueById(ActorId id);

		std::mutex mutex_;

		std::queue<std::shared_ptr<MessageQueue>> actor_msg_queues_;
		std::unordered_map<ActorId, std::shared_ptr<MessageQueue>> id_by_queue_map_;	// ���ڿ��ٲ��ҷ������
	};
}

#endif