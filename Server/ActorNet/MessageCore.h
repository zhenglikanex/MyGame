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
	// ��Ϣ����
	typedef std::queue<ActorMessagePtr> MessageQueue;
	typedef std::shared_ptr<std::pair<actor_id, MessageQueue>> ActorMsgQueuePtr;

	class MessageCore
	{
	public:
		// ����һ��������Ϣ
		void PushActorMessage(actor_id id,const ActorMessagePtr& actor_message);

		// ˳�򵯳�һ��ActorMessage
		ActorMessagePtr PopActorMessage();

	private:
		// ��ȡ�������Ϣ����
		ActorMsgQueuePtr GetActorMsgQueue(actor_id id);

		std::mutex mutex_;

		std::queue<ActorMsgQueuePtr> actor_msg_queues_;

		std::unordered_map<actor_id, ActorMsgQueuePtr> id_by_queue_map_;	// ���ڿ��ٲ��ҷ������
	};
}

#endif