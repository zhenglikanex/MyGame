#include "ActorNet.h"

#include <thread>

#include "MessageCore.h"
#include "ActorMessage.h"
#include "ActorLoader.h"

// reserve high 8 bits for remote id
#define HANDLE_MASK 0xffffff
#define HANDLE_REMOTE_SHIFT 24

namespace actor_net
{
	ActorNet::ActorNet(Config config)
		:config_(config)
	{

	}

	ActorNet::~ActorNet()
	{
		
	}

	bool ActorNet::Init()
	{
		// 节点id
		harbor_ = config_.harbor;

		network_ptr_ = std::make_shared<network::Network>(config_.port);
		if (!network_ptr_)
		{
			//LOG network init fail
			return false;
		}

		// 启动network线程
		network_thread_ = std::thread([this]() {
			network_ptr_->Run();
			});

		// 启动work线程
		uint32_t thread_count = std::thread::hardware_concurrency();
		for (uint32_t i = 0; i < thread_count; ++i)
		{
			work_threads_.emplace_back(std::thread(
				[this]() 
			{
				std::shared_ptr<MessageQueue> message_queue = nullptr;
				while (true)
				{
					while (!message_queue)
					{
						// 弹出消息队列,保证消息队列不会被并发
						message_queue = message_core_.PopMessageQueue();
						if (!message_queue)
						{
							break;
						}

						std::unique_lock lock(mutex_);
						condition_.wait(lock);
					}

					auto actor = GetActorById(message_queue->actor_id());
					if (!actor)
					{
						message_queue = message_core_.PopMessageQueue();
						continue;
					}

					if (message_queue->IsEmpty())
					{
						message_queue = message_core_.PopMessageQueue();
						//不放回message_core中,等到有消息插入再放回,防止线程空转
						continue;
					}

					actor->OnReceive(message_queue->Pop());

					auto next_queue = message_core_.PopMessageQueue();
					if (next_queue)
					{
						// 如果不为空重新放回消息中心,等待下一次调度
						// 如果为空只有一个actor要处理,没必要继续放回
						message_core_.PushMessgeQueue(message_queue);
						message_queue = next_queue;
					}
				}
			}));
		}

		// 加载Actor
		StartActor("StartActor.dll", "start_actor");

		if (harbor_ == config_.master)
		{
			StartActor("StartActor.dll", "master_actor");
		}
		else
		{
			// 与master进行连接
		}

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
		// 等待线程执行完毕
		if (network_thread_.joinable())
		{
			network_thread_.join();
		}

		for (auto& thread : work_threads_)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}

	void ActorNet::StartActor(const std::string& lib_path, const std::string& actor_name /* = "" */)
	{
		if (id_by_actor_map_.size() > kMaxActorCount)
		{
			// TODO:log
			return;
		}

		// 创建actor
		auto actor_ptr = ActorLoader::GetInstance()->CreateActor(lib_path);

		// 注意:这里actor直接持有actor_net的shared_ptr,actor_net保证会主动释放actor
		if (actor_ptr && actor_ptr->Init(shared_from_this()))
		{
			//TODO 分配ID注册等等
			uint32_t handle = GenHandle();
			actor_ptr->set_id(handle);

			id_by_actor_map_.insert(std::make_pair(handle, actor_ptr));

			// 注册名字
			if (actor_name != "")
			{
				RegisterActorName(handle, actor_name);
			}
			// todo log actor启动成功
		}
	}

	void ActorNet::KillActor(ActorId id)
	{
		auto iter = id_by_actor_map_.find(id);
		if (iter != id_by_actor_map_.end())
		{
			iter->second->Stop();
			// TODO: 输出log,停止服务

			id_by_actor_map_.erase(id);
		}
		else
		{
			// TODO: log 不存在该服务
		}
	}

	void ActorNet::KillActor(const std::string& name)
	{
		auto iter = name_by_acotr_id_map_.find(name);
		if (iter != name_by_acotr_id_map_.end())
		{
			KillActor(iter->first);
		}
		else
		{
			// TODO: log 不存在该服务
		}
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

	void ActorNet::RegisterGlobalActorName(ActorId id, const std::string& name)
	{
		// todo:
	}

	ActorId ActorNet::GetActorIdByName(const std::string& name) const
	{
		std::unique_lock lock(mutex_);

		auto iter = name_by_acotr_id_map_.find(name);
		if (iter != name_by_acotr_id_map_.end())
		{
			return iter->second;
		}

		return kNull;
	}

	IActorPtr ActorNet::GetActorById(ActorId id) const
	{
		std::unique_lock lock(mutex_);

		auto iter = id_by_actor_map_.find(id);
		if (iter != id_by_actor_map_.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	IActorPtr ActorNet::GetActorByName(const std::string& name) const
	{
		auto id = GetActorIdByName(name);
		if (id == kNull)
		{
			return nullptr;
		}

		return GetActorById(id);
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

	void ActorNet::SendActorMessage(ActorId src_id, ActorId dest_id,std::vector<uint8_t>&& data)
	{
		ActorMessage message;
		message.set_src_id(src_id);
		message.set_dest_id(dest_id);
		message.set_data(std::move(data));
		message.Write(data.data(), data.size());
		
		SendActorMessage(std::move(message));
	}

	void ActorNet::SendActorMessage(const std::string& src_actor_name, const std::string& dest_actor_name,std::vector<uint8_t>&& data)
	{
		ActorId src_actor_id = GetActorIdByName(src_actor_name);
		ActorId dest_actor_id = GetActorIdByName(dest_actor_name);

		if (dest_actor_id <= 0)
		{
			// todo : 不存在目标服务
			return;
		}

		SendActorMessage(src_actor_id, dest_actor_id,std::move(data));
	}

	void ActorNet::SessionConnectHandler(const network::SessionPtr& session_ptr)
	{

	}

	void ActorNet::SessionReceiveHandler(const network::SessionPtr& session_ptr, const network::Message& message)
	{

	}

	void ActorNet::SessionCloseHander(const network::SessionPtr& session_ptr)
	{

	}

	uint32_t ActorNet::GenHandle()
	{
		for (uint32_t handle_index = 1; handle_index <= kMaxActorCount; ++handle_index)
		{
			if (id_by_actor_map_.find(handle_index) == id_by_actor_map_.end())
			{
				return handle_index;
			}
		}

		return 0;
	}
}

