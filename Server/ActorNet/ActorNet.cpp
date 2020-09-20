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
		// �ڵ�id
		harbor_ = config_.harbor;

		network_ptr_ = std::make_shared<network::Network>(config_.port);
		if (!network_ptr_)
		{
			//LOG network init fail
			return false;
		}

		// ����network�߳�
		network_thread_ = std::thread([this]() {
			network_ptr_->Run();
			});

		// ����work�߳�
		uint32_t thread_count = std::thread::hardware_concurrency();
		for (uint32_t i = 0; i < thread_count; ++i)
		{
			work_threads_.emplace_back(std::thread(
				[this]() 
			{
				// TODO ������Ϣ����
				while (true)
				{
					{
						std::unique_lock<std::mutex> lock(mutex_);

						ActorMessagePtr actor_msg_ptr;

						// ȡ����Ϣ,���û����Ϣ��˯�߳�;
						while ( !(actor_msg_ptr = message_core_.PopActorMessage()) )
						{
							condition_.wait(lock);
						}

						// ȡ��Ŀ��actor
						auto actor = GetActorById(actor_msg_ptr->dest_id());
						if (actor)
						{
							// ������Ϣ����
							actor->OnReceive(actor_msg_ptr);
						}
					}
				}
			}));
		}

		// ����Actor
		StartActor("StartActor.dll", "start_actor");

		if (harbor_ == config_.master)
		{
			StartActor("StartActor.dll", "master_actor");
		}
		else
		{
			// ��master��������
		}

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
	}

	void ActorNet::StartActor(const std::string& lib_path, const std::string& actor_name /* = "" */)
	{
		if (id_by_actor_map_.size() > kMaxActorCount)
		{
			// TODO:log
			return;
		}

		// ����actor
		auto actor_ptr = ActorLoader::GetInstance()->CreateActor(lib_path);

		// ע��:����actorֱ�ӳ���actor_net��shared_ptr,actor_net��֤�������ͷ�actor
		if (actor_ptr && actor_ptr->Init(shared_from_this()))
		{
			//TODO ����IDע��ȵ�
			uint32_t handle = GenHandle();
			actor_ptr->set_id(handle);

			id_by_actor_map_.insert(std::make_pair(handle, actor_ptr));

			// ע������
			if (actor_name != "")
			{
				RegisterActorName(handle, actor_name);
			}
			// todo log actor�����ɹ�
		}
	}

	void ActorNet::KillActor(actor_id id)
	{
		auto iter = id_by_actor_map_.find(id);
		if (iter != id_by_actor_map_.end())
		{
			iter->second->Stop();
			// TODO: ���log,ֹͣ����

			id_by_actor_map_.erase(id);
		}
		else
		{
			// TODO: log �����ڸ÷���
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
			// TODO: log �����ڸ÷���
		}
	}

	void ActorNet::RegisterActorName(actor_id id, const std::string& name)
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

	actor_id ActorNet::GetActorIdByName(const std::string& name)
	{
		auto iter = name_by_acotr_id_map_.find(name);
		if (iter != name_by_acotr_id_map_.end())
		{
			return iter->second;
		}

		return k_invalid_actor_id;
	}

	IActorPtr ActorNet::GetActorById(actor_id id)
	{
		auto iter = id_by_actor_map_.find(id);
		if (iter != id_by_actor_map_.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	IActorPtr ActorNet::GetActorByName(const std::string& name)
	{
		auto id = GetActorIdByName(name);
		if (id == k_invalid_actor_id)
		{
			return nullptr;
		}

		return GetActorById(id);
	}

	void ActorNet::SendActorMessage(const ActorMessagePtr& acotr_msg)
	{
		// TODO:
		// �ж��Ƿ񱾵ط�����Զ�̷���
		if (true)
		{
			// ���ط�����Ϣ
			message_core_.PushActorMessage(acotr_msg->dest_id(), acotr_msg);

			// ����һ���߳�
			condition_.notify_one();
		}
		else
		{
			// ����Զ�̷���
		}
	}

	void ActorNet::SendActorMessage(actor_id src_id, actor_id dest_id, const std::vector<uint8_t>& data)
	{
		ActorMessagePtr message = std::make_shared<ActorMessage>();
		message->set_src_id(src_id);
		message->set_dest_id(dest_id);
		message->Write(data.data(), data.size());
		
		SendActorMessage(message);
	}

	void ActorNet::SendActorMessage(const std::string& src_actor_name, const std::string& dest_actor_name, const std::vector<uint8_t>& data)
	{
		actor_id src_actor_id = 0;
		actor_id dest_actor_id = 0;

		auto iter = name_by_acotr_id_map_.find(src_actor_name);
		if (iter != name_by_acotr_id_map_.end())
		{
			src_actor_id = iter->second;
		}

		iter = name_by_acotr_id_map_.find(dest_actor_name);
		if (iter != name_by_acotr_id_map_.end())
		{
			dest_actor_id = iter->second;
		}

		if (dest_actor_id <= 0)
		{
			// todo : ������Ŀ�����
			return;
		}

		SendActorMessage(src_actor_id, dest_actor_id, data);
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

