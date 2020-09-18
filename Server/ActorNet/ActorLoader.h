#ifndef ACTOR_LOADER_H_
#define ACTOR_LOADER_H_

#include <unordered_map>
#include <memory>

#include "Singleton.h"
#include "IActor.h"

namespace actor_net
{
	class ActorLoader : public Singleton<ActorLoader>
	{
	public:
		ActorLoader();
		~ActorLoader();

		// ����һ������
		IActorPtr CreateActor(const std::string& lib_path);
	private:
		// ��ȡactor��̬���ʵ��
		void* GetActorInstance(const std::string& lib_path);
	private:
		std::unordered_map<std::string,void*> actor_instance_map_;
	};
}

#endif