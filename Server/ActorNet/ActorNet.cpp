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
		: config_(config)
	{

	}

	ActorNet::~ActorNet()
	{
		
	}

	bool ActorNet::Init()
	{
		// ����work�߳�
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
						// ������Ϣ����,��֤��Ϣ���в��ᱻ����
						message_queue = message_core_.PopMessageQueue();
						if (message_queue)
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
						//���Ż�message_core��,�ȵ�����Ϣ�����ٷŻ�,��ֹ�߳̿�ת
						continue;
					}

					actor->OnReceive(message_queue->Pop());

					auto next_queue = message_core_.PopMessageQueue();
					if (next_queue)
					{
						// �����Ϊ�����·Ż���Ϣ����,�ȴ���һ�ε���
						// ���Ϊ��ֻ��һ��actorҪ����,û��Ҫ�����Ż�
						message_core_.PushMessgeQueue(message_queue);
						message_queue = next_queue;
					}
				}
			}));
		}

		// ����Actor
		StartActor("StartActor.dll", "start_actor");

		return true;
	}

	bool ActorNet::Start()
	{
		// ��ʼ������
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
		// �ȴ��߳�ִ�����
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

		KillAllActor();
	}

	ActorId ActorNet::StartActor(const std::string& lib_path, const std::string& actor_name /* = "" */)
	{
		std::unique_lock lock(mutex_);
		auto actor = ActorLoader::GetInstance()->CreateActor(lib_path);
		// ע��:����actorֱ�ӳ���actor_net��shared_ptr,actor_net��֤�������ͷ�actor
		if (actor && actor->Init(shared_from_this()))
		{
			uint32_t handle = GenHandle();
			actor->set_id(handle);

			id_by_actor_map_.insert(std::make_pair(handle, actor));
			// todo log actor�����ɹ�

			return handle;
		}

		return kNull;
	}

	actor_net::ActorId ActorNet::StartUniqeActor(const std::string& lib_path, const std::string& actor_name)
	{
		std::unique_lock lock(mutex_);
		auto actor = ActorLoader::GetInstance()->CreateActor(lib_path);
		// ע��:����actorֱ�ӳ���actor_net��shared_ptr,actor_net��֤�������ͷ�actor
		if (actor && actor->Init(shared_from_this()))
		{
			uint32_t handle = GenHandle();
			actor->set_id(handle);
			id_by_actor_map_.insert(std::make_pair(handle, actor));

			

			// todo log actor�����ɹ�

			RegisterActorName(handle, actor_name);

			return handle;
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
				// TODO: ���log,ֹͣ����

				id_by_actor_map_.erase(it);
			}
			else
			{
				// TODO: log �����ڸ÷���
			}

			name_by_acotr_id_map_.erase(iter);
		}
		else
		{
			// TODO: log �����ڸ÷���
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

	IActorPtr ActorNet::GetActorById(ActorId id)
	{
		std::unique_lock lock(mutex_);

		auto iter = id_by_actor_map_.find(id);
		if (iter != id_by_actor_map_.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	IActorPtr ActorNet::GetActorByName(const std::string& name)
	{
		auto id = QueryActorId(name);
		if (id == kNull)
		{
			return nullptr;
		}

		return GetActorById(id);
	}

	void ActorNet::RegisterActorName(ActorId id, const std::string& name)
	{
		for (auto& entry : name_by_acotr_id_map_)
		{
			if (entry.second == id)
			{
				// todo:���log��Ӧ���ظ�����;
				return;
			}
		}

		auto iter = name_by_acotr_id_map_.find(name);
		if (iter != name_by_acotr_id_map_.end())
		{
			// TODO: ���log��Ӧ������
			return;
		}

		name_by_acotr_id_map_.insert(std::make_pair(name, id));
	}

	void ActorNet::SendActorMessage(ActorMessage&& acotr_msg)
	{
		// TODO:
		// �ж��Ƿ񱾵ط�����Զ�̷���
		if (true)
		{
			// ���ط�����Ϣ
			message_core_.PushActorMessage(acotr_msg.dest_id(), std::move(acotr_msg));

			// ����һ���߳�
			condition_.notify_one();
		}
		else
		{
			// ����Զ�̷���
		}
	}

	void ActorNet::SendActorMessage(ActorId src_id, ActorId dest_id, ActorMessage::SessionType session, ActorMessage::MessageType type, std::string_view name, std::any&& data)
	{
		ActorMessage message;
		message.set_src_id(src_id);
		message.set_dest_id(dest_id);
		message.set_type(type);
		message.set_session(session);
		message.set_data(std::move(data));

		SendActorMessage(std::move(message));
	}

	void ActorNet::SendActorMessage(ActorId src_id, const std::string& dest_actor_name, ActorMessage::SessionType session, ActorMessage::MessageType type, std::string_view name, std::any&& data)
	{
		ActorId dest_actor_id = QueryActorId(dest_actor_name);

		if (dest_actor_id == kNull)
		{
			// todo : ������Ŀ�����
			return;
		}

		SendActorMessage(src_id, dest_actor_id, session, type,name,std::move(data));
	}

	uint32_t ActorNet::GenHandle() const
	{
		if (id_by_actor_map_.size() >= kMaxActorCount)
		{
			return kNull;
		}

		for (uint32_t handle_index = 0; handle_index <= kMaxActorCount; ++handle_index)
		{
			if (id_by_actor_map_.find(handle_index) == id_by_actor_map_.end())
			{
				return handle_index;
			}
		}

		return kNull;
	}
}

