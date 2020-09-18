#include "ActorLoader.h"

using namespace actor_net;

#if defined _WIN32

#include <Windows.h>
#define LoadDLL(dll_name) LoadLibrary(dll_name)
#define GetProc(dll,func_name) GetProcAddress((HMODULE)dll,func_name)
#define FreeDLL(dll)  FreeLibrary(dll)
#else 

// TODO: linux加载so的方法
#include <sys/system>
#define LoadDLL Load

#endif

typedef IActor* (*Creator)();

ActorLoader::ActorLoader()
{

}

ActorLoader::~ActorLoader()
{
	// TODO : 卸载动态库
}

IActorPtr ActorLoader::CreateActor(const std::string& lib_path)
{
	auto actor_instance = GetActorInstance(lib_path);
	if (actor_instance)
	{
		// 
		Creator creator = (Creator)GetProc(actor_instance, "CreateActor");
		if (creator)
		{
			auto p = creator();
			if (p)
			{
				IActorPtr actor_ptr(p);
				return actor_ptr;
			}
		}
	}

	// 返回个空指针
	return IActorPtr();
}

void* ActorLoader::GetActorInstance(const std::string& lib_path)
{
	auto iter = actor_instance_map_.find(lib_path);
	if (iter != actor_instance_map_.end())
	{
		return iter->second;
	}

	void* instance = LoadDLL(lib_path.c_str());
	if (instance) {
		actor_instance_map_.insert(std::make_pair(lib_path, instance));
	}

	return instance;
}