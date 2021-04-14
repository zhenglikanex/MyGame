#ifndef ACTOR_H_
#define ACTOR_H_

#include <cstdint>
#include <memory>
#include <string>
#include <functional>
#include <any>
#include <array>
#include <chrono>

#include "Config.hpp"
#include "ActorMessage.h"

namespace actor_net
{
	class ActorNet;

	const ActorId kNull = std::numeric_limits<uint32_t>::max();

	// ����������ӿ�
	class Actor : public std::enable_shared_from_this<Actor>
	{
	public:
		Actor(uint32_t id) :id_(id) { }
		virtual ~Actor() = 0 { }

		virtual bool Init(const std::shared_ptr<ActorNet>& actor_net)
		{
			actor_net_ = actor_net;

			return true;
		}

		virtual void Stop()
		{

		}

		void Request(ActorId dest_id, const std::string_view func_name, std::any&& data, const std::function<void(ActorMessage&&)>& callback);
		void Request(const std::string& actor_name, const std::string_view func_name, std::any&& data, const std::function<void(ActorMessage&&)>& callback);
		void Call(ActorId dest_id, const std::string_view func_name, std::any&& data);
		void NewActor(const std::string& name, const std::function<void(ActorId)>& callback);

		uint32_t AddTimer(uint32_t millisec, int32_t repeat, const std::function<void()>& callback);
		void CancelTimer(uint32_t id);

		void OnReceive(ActorMessage&& actor_msg);
		virtual void Receive(ActorMessage&& actor_msg) = 0;

		void set_name(std::string_view name) { name_ = name; }

		ActorId id() const { return id_; }
		const std::string& name() const { return name_; }
		const std::shared_ptr<ActorNet>& actor_net() const { return actor_net_; }
	private:
		std::shared_ptr<ActorNet> actor_net_;
		std::string name_;
		ActorId id_;
		
		const static ActorMessage::SessionType kMaxSession = 2 ^ 16;
		ActorMessage::SessionType session_id_;
		std::array<std::function<void(ActorMessage&&)>, kMaxSession> request_callbacks_;
	};

	typedef std::shared_ptr<Actor> ActorPtr;
}

extern "C" __declspec(dllexport) actor_net::Actor* CreateActor(unsigned int);

#define ACTOR_IMPLEMENT(actor_name) \
extern "C" __declspec(dllexport) actor_net::Actor* CreateActor(unsigned int id) \
{ \
	return new actor_name(id); \
}

#endif
