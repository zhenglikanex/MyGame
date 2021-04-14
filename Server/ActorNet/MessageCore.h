#ifndef MESSAGE_CORE_H_
#define MESSAGE_CORE_H_

#include <list>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <cassert>
#include <atomic>

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

		bool IsEmpty();
		void Push(ActorMessage&& message);
		ActorMessage Pop();

		void set_mamaged(bool managed) { managed_ = managed; }

		ActorId actor_id() const { return actor_id_; }
		bool managed() const { return managed_; }
	private:
		ActorId actor_id_;
		std::queue<ActorMessage> messages_;
		bool managed_;
		std::mutex mutex_;
	};

	class MessageCore
	{
	public:
		void PushActorMessage(ActorId id,ActorMessage&& actor_message);
		std::shared_ptr<MessageQueue> GetMessageQueue(std::shared_ptr<MessageQueue> queue);
		void RemoveMessageQueueById(ActorId id);
	private:
		std::shared_ptr<MessageQueue> GetMessageQueueById(ActorId id);
		void PushMessgeQueue(const std::shared_ptr<MessageQueue>& queue);
		std::shared_ptr<MessageQueue> PopMessageQueue();
		std::mutex mutex_;

		std::queue<std::shared_ptr<MessageQueue>> actor_msg_queues_;
		std::unordered_map<ActorId, std::shared_ptr<MessageQueue>> id_by_queue_map_;	// 用于快速查找服务队列
	};
}

#endif