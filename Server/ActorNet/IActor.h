#ifndef ACTOR_H_
#define ACTOR_H_

#include <cstdint>
#include <memory>

namespace actor_net
{
	class ActorNet;
	class ActorMessage;

	typedef uint32_t ActorId;
	const ActorId kNull = 0;

	// 服务服务抽象接口
	class IActor
	{
	public:
		IActor() :id_(kNull) { }
		virtual ~IActor() = 0 { }

		virtual bool Init(const std::shared_ptr<ActorNet>& actor_net_ptr)
		{
			actor_net_ptr_ = actor_net_ptr;

			return true;
		}

		virtual void Stop()
		{

		}

		virtual void OnReceive(ActorMessage&& actor_msg) = 0;

		void set_id(ActorId id) { id_ = id; };
		ActorId id() { return id_; }
		
	protected:
		void SendActorMessage(ActorMessage&& actor_msg)
		{

		}

		std::shared_ptr<ActorNet> actor_net_ptr_;
		ActorId id_;
	};

	typedef std::shared_ptr<IActor> IActorPtr;
}

extern "C" __declspec(dllexport) actor_net::IActor* CreateActor();

#define ACTOR_IMPLEMENT(actor_name) \
extern "C" __declspec(dllexport) actor_net::IActor* CreateActor() \
{ \
	return new actor_name(); \
}

#endif
