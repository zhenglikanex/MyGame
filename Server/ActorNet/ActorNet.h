#ifndef ACTOR_NET_H_
#define ACTOR_NET_H_

#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>
#include <memory>
#include <unordered_map>

#include "Config.hpp"
#include "ActorMessage.h"
#include "MessageCore.h"
#include "Actor.h"
#include "NetworkActor.hpp"
#include "TimerActor.hpp"

namespace actor_net
{
	class NetworkActor;

	struct Config
	{
		uint32_t harbor;
		uint32_t master;
		std::string ip;
		uint32_t port;
		std::string master_ip;
		uint32_t master_port;
	};

	class ActorNet : public std::enable_shared_from_this<ActorNet>
	{
	public:
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
		ActorId StartActor(const std::string& lib_path);
		ActorId StartUniqeActor(const std::string& lib_path, const std::string& actor_name);

		// 杀死actor
		void KillActor(ActorId id);
		void KillActor(const std::string& name);
		void KillAllActor();
		
		ActorId QueryActorId(const std::string& name);

		ActorPtr GetActorById(ActorId id);
		ActorPtr GetActorByName(const std::string& name);
	public:
		void CreateTcpServer(uint32_t src, uint16_t port);
		void CloseTcpServer(uint32_t src);
		void TcpSend(uint16_t connection_id, Buffer&& data);
		void TcpClose(uint16_t connection_id);

		void CreateUdpServer(uint32_t src, uint16_t port);
		void CloseUdpServer(uint32_t src);
		void UdpSend(uint32_t src, const asio::ip::udp::endpoint& udp_remote_endpoint, Buffer&& data);
	public:
		uint32_t AddTimer(ActorId id, uint32_t millisec, int32_t repeat, const std::function<void()>& callback);
		void CancelTimer(uint32_t id);
	public:
		void SendActorMessage(ActorMessage&& acotr_msg);
		void SendActorMessage(ActorId src_id, ActorId dest_id, ActorMessage::SessionType session, ActorMessage::MessageType type, std::string_view name, std::any&& data);
		void SendActorMessage(ActorId src_id, const std::string& dest_actor_name,ActorMessage::SessionType session,ActorMessage::MessageType type,std::string_view name,std::any&& data);
	private:
		void RegisterActorName(ActorId id, const std::string& name);
	private:
		uint32_t GenHandle();
	private:
		Config config_;
		std::unordered_map<std::string, ActorId> name_by_acotr_id_map_;
		std::unordered_map<ActorId, ActorPtr> id_by_actor_map_;
		std::unique_ptr<NetworkActor> network_actor_;
		std::unique_ptr<TimerActor> timer_actor_;
		MessageCore message_core_;


		std::mutex mutex_;
		std::condition_variable condition_;
		std::thread network_thread_;
		std::thread timer_thread_;
		std::vector<std::thread> work_threads_;		// 工作线程
		ActorId alloc_id_;
	};
}

#endif