#include "StartActor.h"
#include "ActorNet.h"

StartActor::StartActor()
{

}

StartActor::~StartActor()
{

}

bool StartActor::Init(const std::shared_ptr<ActorNet>& actor_net_ptr)
{
	if (!IActor::Init(actor_net_ptr))
	{
		return false;
	}

	std::cout << "StartActor start!!!" << std::endl;
	return true;
}

void StartActor::Stop()
{

}

void StartActor::OnReceive(ActorMessage&& actor_msg)
{
	auto& [lib_name,actor_name] = std::any_cast<std::tuple<std::string, std::string>>(actor_msg.data());
	actor_net_ptr_->StartActor(lib_name, actor_name);
}

ACTOR_IMPLEMENT(StartActor)