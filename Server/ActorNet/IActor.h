#ifndef ACTOR_H_
#define ACTOR_H_

#include <cstdint>
#include <memory>
#include <string>

namespace actor_net
{
	class ActorNet;
	class ActorMessage;

	typedef uint32_t ActorId;
	const ActorId kNull = std::numeric_limits<uint32_t>::max();

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
		void set_name(const std::string& name) { name_ = name; }
		ActorId id() { return id_; }
		const std::string& name() const { return name_; }
	protected:
		std::shared_ptr<ActorNet> actor_net_ptr_;
		std::string name_;
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
