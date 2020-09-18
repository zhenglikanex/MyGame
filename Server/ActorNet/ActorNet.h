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
	class ActorNet : public std::enable_shared_from_this<ActorNet> , public network::ISessionHandlder
	{
	public:
		const static uint32_t s_max_actor_count = 255;

		ActorNet();
		~ActorNet();

	private:
		bool Init();

	public:
		bool Start();
		void Stop();

	public:
		void set_harbor(uint32_t harbor) { harbor_ = harbor; }

		uint32_t harbor() { harbor_; }
	public:
		// 启动actor
		void StartActor(const std::string& lib_path, const std::string& actor_name = "");

		// 杀死actor
		void KillActor(actor_id id);
		void KillActor(const std::string& name);

		// 注册actor名字
		void RegisterActorName(actor_id id, const std::string& name);
		
		actor_id GetActorIdByName(const std::string& name);

		IActorPtr GetActorById(actor_id id);
		IActorPtr GetActorByName(const std::string& name);
	public:
		// 发送本地服务消息
		void SendActorMessage(const ActorMessagePtr& acotr_msg);
		void SendActorMessage(actor_id src_id, actor_id dest_id,const std::vector<uint8_t>& data);
		void SendActorMessage(const std::string& src_actor_name, const std::string& dest_actor_name, const std::vector<uint8_t>& data);

		// 发送远程服务消息
		void SendHarborMessage();

	private:
		// 会话处理函数
		void SessionConnectHandler(const network::SessionPtr& session_ptr) override;
		void SessionReceiveHandler(const network::SessionPtr& session_ptr, const network::Message& message) override;
		void SessionCloseHander(const network::SessionPtr& session_ptr) override;

	private:
		uint32_t GenHandle();
	private:
		uint32_t harbor_;
		uint32_t handle_index_;

		std::unordered_map<std::string, actor_id> name_by_acotr_id_map_;
		std::unordered_map<actor_id, IActorPtr> id_by_actor_map_;

		MessageCore message_core_;

		std::mutex mutex_;
		std::condition_variable condition_;

		network::NetworkPtr network_ptr_;	// 底层网络通讯模块
		std::thread network_thread_;		// network线程
		std::vector<std::thread> work_threads_;		// 工作线程;
	};
}

#endif