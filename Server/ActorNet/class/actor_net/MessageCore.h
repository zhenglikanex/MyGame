#ifndef MESSAGE_CORE_H_
#define MESSAGE_CORE_H_

#include <list>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <functional>

#include "Singleton.h"
#include "ActorMessage.h"

namespace actor_net
{
	// 消息队列
	typedef std::queue<ActorMessagePtr> MessageQueue;
	typedef std::shared_ptr<std::pair<actor_id, MessageQueue>> ActorMsgQueuePtr;

	class MessageCore
	{
	public:
		// 插入一条服务消息
		void PushActorMessage(actor_id id,const ActorMessagePtr& actor_message);

		// 顺序弹出一条ActorMessage
		ActorMessagePtr PopActorMessage();

	private:
		// 获取服务的消息队列
		ActorMsgQueuePtr GetActorMsgQueue(actor_id id);

		std::mutex mutex_;

		std::queue<ActorMsgQueuePtr> actor_msg_queues_;

		std::unordered_map<actor_id, ActorMsgQueuePtr> id_by_queue_map_;	// 用于快速查找服务队列
	};
}

#endif