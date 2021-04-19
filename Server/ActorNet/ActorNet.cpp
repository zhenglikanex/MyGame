#include "ActorNet.h"

#include <thread>
#include <tuple>

#include "MessageCore.h"
#include "ActorMessage.h"
#include "ActorLoader.h"

namespace actor_net
{
	ActorNet::ActorNet(Config config)
		: config_(config)
		, alloc_id_(0)
	{

	}

	ActorNet::~ActorNet()
	{
		
	}

	bool ActorNet::Init()
	{
		timer_actor_ = std::make_unique<TimerActor>(shared_from_this());
		timer_thread_ = std::thread([this]
		{
			timer_actor_->Run();
		});

		network_actor_ = std::make_unique<NetworkActor>(shared_from_this());
		network_thread_ = std::thread([this]{
			network_actor_->Run();
		});

		// 启动work线程
		uint32_t thread_count = std::thread::hardware_concurrency() - 2;
		for (uint32_t i = 0; i < thread_count; ++i)
		{
			work_threads_.emplace_back(std::thread(
				[this]() 
			{
				std::mutex mutex;
				std::unique_lock lock(mutex);
				std::shared_ptr<MessageQueue> message_queue = nullptr;
				while (true)
				{
					while (!message_queue)
					{
						// 弹出需要处理消息队列,保证消息队列不会被多个线程获取
						message_queue = message_core_.GetMessageQueue(message_queue);
						if (message_queue)
						{
							break;
						}

						condition_.wait(lock);
					}

					auto actor = GetActorById(message_queue->actor_id());
					if (!actor)
					{
						continue;
					}

					actor->OnReceive(message_queue->Pop());
					message_queue = message_core_.GetMessageQueue(message_queue);
				}
			}));
		}

		// 加载Actor
		StartActor("StartActor.dll");

		return true;
	}

	bool ActorNet::Start()
	{
		// 初始化服务
		if (Init())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ActorNet::Stop()
	{
		if (timer_actor_)
		{
			timer_actor_->Stop();
		}
		
		if (network_actor_)
		{
			network_actor_->Stop();
		}

		if (timer_thread_.joinable())
		{
			timer_thread_.join();
		}

		if (network_thread_.joinable())
		{
			network_thread_.join();
		}

		// 等待线程执行完毕
		for (auto& thread : work_threads_)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}

		KillAllActor();
	}

	ActorId ActorNet::StartActor(const std::string& lib_path)
	{
		std::unique_lock lock(mutex_);
		uint32_t handle = GenHandle();
		if (handle != kNull)
		{
			auto actor = ActorLoader::GetInstance()->CreateActor(lib_path,handle);
			// 注意:这里actor直接持有actor_net的shared_ptr,actor_net保证会主动释放actor
			if (actor)
			{
				if (actor->Init(shared_from_this()))
				{
					// todo log actor启动成功
					id_by_actor_map_.insert(std::make_pair(handle, actor));
				}
				return handle;
			}
		}
		
		return kNull;
	}

	ActorId ActorNet::StartUniqueActor(const std::string& lib_path, const std::string& actor_name)
	{
		std::unique_lock lock(mutex_);

		if (name_by_acotr_id_map_.find(actor_name) != name_by_acotr_id_map_.end())
		{
			return kNull;
		}

		uint32_t handle = GenHandle();
		if (handle != kNull)
		{
			auto actor = ActorLoader::GetInstance()->CreateActor(lib_path, handle);
			// 注意:这里actor直接持有actor_net的shared_ptr,actor_net保证会主动释放actor
			if (actor)
			{
				if (actor->Init(shared_from_this()))
				{
					RegisterActorName(handle, actor_name);
					// todo log actor启动成功
					id_by_actor_map_.insert(std::make_pair(handle, actor));
				}
				return handle;
			}
		}

		return kNull;
	}

	void ActorNet::KillActor(ActorId id)
	{
		
	}

	void ActorNet::KillActor(const std::string& name)
	{
		auto iter = name_by_acotr_id_map_.find(name);
		if (iter != name_by_acotr_id_map_.end())
		{	
			auto it = id_by_actor_map_.find(iter->second);
			if (it != id_by_actor_map_.end())
			{
				it->second->Stop();
				// TODO: 输出log,停止服务

				id_by_actor_map_.erase(it);
			}
			else
			{
				// TODO: log 不存在该服务
			}

			name_by_acotr_id_map_.erase(iter);
		}
		else
		{
			// TODO: log 不存在该服务
		}
	}

	void ActorNet::KillAllActor()
	{
		for (auto& entry : id_by_actor_map_)
		{
			entry.second->Stop();
		}

		name_by_acotr_id_map_.clear();
		id_by_actor_map_.clear();
	}

	ActorId ActorNet::QueryActorId(const std::string& name)
	{
		std::unique_lock lock(mutex_);

		auto iter = name_by_acotr_id_map_.find(name);
		if (iter != name_by_acotr_id_map_.end())
		{
			return iter->second;
		}

		return kNull;
	}

	ActorPtr ActorNet::GetActorById(ActorId id)
	{
		std::unique_lock lock(mutex_);

		auto iter = id_by_actor_map_.find(id);
		if (iter != id_by_actor_map_.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	ActorPtr ActorNet::GetActorByName(const std::string& name)
	{
		auto id = QueryActorId(name);
		if (id == kNull)
		{
			return nullptr;
		}

		return GetActorById(id);
	}

	void ActorNet::CreateTcpServer(uint32_t src,uint16_t port)
	{
		network_actor_->Post(ActorMessage(src, 0, 0, ActorMessage::MessageType::kTypeNetwork,"tcp",port));
	}

	void ActorNet::CloseTcpServer(uint32_t src)
	{
		network_actor_->Post(ActorMessage(src, 0, 0, ActorMessage::MessageType::kTypeNetwork, "close_tcp_server"));
	}

	void ActorNet::TcpSend(uint16_t connection_id, Buffer&& data)
	{
		
		network_actor_->Post(ActorMessage(0, 0, 0, ActorMessage::MessageType::kTypeNetwork, "tcp_send", std::make_tuple(connection_id, std::move(data))));
	}

	void ActorNet::TcpClose(uint16_t connection_id)
	{
		network_actor_->Post(ActorMessage(0, 0, 0, ActorMessage::MessageType::kTypeNetwork, "tcp_close", connection_id));
	}

	void ActorNet::CreateUdpServer(uint32_t src,uint16_t port)
	{
		network_actor_->Post(ActorMessage(src, 0, 0, ActorMessage::MessageType::kTypeNetwork, "udp", port));
	}

	void ActorNet::CloseUdpServer(uint32_t src)
	{
		network_actor_->Post(ActorMessage(src, 0, 0, ActorMessage::MessageType::kTypeNetwork, "close_udp_server"));
	}

	void ActorNet::UdpSend(uint32_t src, const asio::ip::udp::endpoint& udp_remote_endpoint, Buffer&& data)
	{
		network_actor_->Post(ActorMessage(src, 0, 0, ActorMessage::MessageType::kTypeNetwork, "udp_send", std::make_tuple(udp_remote_endpoint, std::move(data))));
	}

	uint32_t ActorNet::AddTimer(ActorId id, uint32_t millisec, int32_t repeat, const std::function<void()>& callback)
	{
		return timer_actor_->AddTimer(id, millisec, repeat, callback);
	}

	void ActorNet::CancelTimer(uint32_t id)
	{
		timer_actor_->CancelTimer(id);
	}

	void ActorNet::RegisterActorName(ActorId id, const std::string& name)
	{
		for (auto& entry : name_by_acotr_id_map_)
		{
			if (entry.second == id)
			{
				// todo:输出log不应该重复命名;
				return;
			}
		}

		auto iter = name_by_acotr_id_map_.find(name);
		if (iter != name_by_acotr_id_map_.end())
		{
			// TODO: 输出log不应该重名
			return;
		}

		name_by_acotr_id_map_.insert(std::make_pair(name, id));
	}

	void ActorNet::SendActorMessage(ActorMessage&& acotr_msg)
	{
		// TODO:
		// 判断是否本地服务还是远程服务
		if (true)
		{
			// 本地服务消息
			message_core_.PushActorMessage(acotr_msg.dest_id(), std::move(acotr_msg));

			// 唤醒一条线程
			condition_.notify_one();
		}
		else
		{
			// 发送远程服务
		}
	}

	void ActorNet::SendActorMessage(ActorId src_id, ActorId dest_id, ActorMessage::SessionType session, ActorMessage::MessageType type, std::string_view name, std::any&& data)
	{
		ActorMessage message;
		message.set_src_id(src_id);
		message.set_dest_id(dest_id);
		message.set_type(type);
		message.set_session(session);
		message.set_name(name);
		message.set_data(std::move(data));

		SendActorMessage(std::move(message));
	}

	void ActorNet::SendActorMessage(ActorId src_id, const std::string& dest_actor_name, ActorMessage::SessionType session, ActorMessage::MessageType type, std::string_view name, std::any&& data)
	{
		ActorId dest_actor_id = QueryActorId(dest_actor_name);

		if (dest_actor_id == kNull)
		{
			// todo : 不存在目标服务
			return;
		}

		SendActorMessage(src_id, dest_actor_id, session, type,name,std::move(data));
	}

	uint32_t ActorNet::GenHandle()
	{
		return alloc_id_++;
	}
}

