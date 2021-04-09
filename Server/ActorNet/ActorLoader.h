#ifndef ACTOR_LOADER_H_
#define ACTOR_LOADER_H_

#include <unordered_map>
#include <memory>

#include "Singleton.h"
#include "Actor.h"

namespace actor_net
{
	class ActorLoader : public Singleton<ActorLoader>
	{
	public:
		ActorLoader();
		~ActorLoader();

		// ����һ������
		ActorPtr CreateActor(const std::string& lib_path,ActorId id);
	private:
		// ��ȡactor��̬���ʵ��
		void* GetActorInstance(const std::string& lib_path);
	private:
		std::unordered_map<std::string,void*> actor_instance_map_;
	};
}

#endif