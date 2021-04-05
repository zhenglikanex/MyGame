#ifndef ACTOR_NET_
#define ACTOR_NET_

#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>
#include <memory>
#include <unordered_map>

#include "Network.h"
#include "ISessionHandler.h"
#include "ActorMessage.h"
#include "MessageCore.h"

namespace actor_net
{
	struct Config
	{
		uint32_t harbor;
		uint32_t master;
		std::string ip;
		uint32_t port;
		std::string master_ip;
		uint32_t master_port;
	};

	class ActorNet : public std::enable_shared_from_this<ActorNet> , public network::ISessionHandlder
	{
	public:
		const static uint32_t kMaxActorCount = 255;

		ActorNet(Config config);
		~ActorNet();
	private:
		bool Init();

	public:
		bool Start();
		void Stop();

	public:
		const Config& config() const { return config_; }
	public:
		

		// 启动actor
		ActorId StartActor(const std::string& lib_path, const std::string& actor_name);
		ActorId StartUniqeActor(const std::string& lib_path, const std::string& actor_name);

		// 杀死actor
		void KillActor(ActorId id);
		void KillActor(const std::string& name);
		void KillAllActor();
		
		ActorId QueryActorId(const std::string& name);

		IActorPtr GetActorById(ActorId id);
		IActorPtr GetActorByName(const std::string& name);
	public:
	public:
		void SendActorMessage(ActorMessage&& acotr_msg);
		void SendActorMessage(ActorId src_id, ActorId dest_id, ActorMessage::SessionType session, ActorMessage::MessageType type, std::string_view name, std::any&& data);
		void SendActorMessage(ActorId src_id, const std::string& dest_actor_name,ActorMessage::SessionType session,ActorMessage::MessageType type,std::string_view name,std::any&& data);
	private:
		void RegisterActorName(ActorId id, const std::string& name);
	private:
		uint32_t GenHandle() const;
	private:
		Config config_;

		std::unordered_map<std::string, ActorId> name_by_acotr_id_map_;
		std::unordered_map<ActorId, IActorPtr> id_by_actor_map_;

		MessageCore message_core_;

		std::mutex mutex_;
		std::condition_variable condition_;
		std::vector<std::thread> work_threads_;		// 工作线程
	};
}

#endif