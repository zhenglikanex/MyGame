#include "MasterActor.hpp"

MasterActor::MasterActor()
{

}

MasterActor::~MasterActor()
{

}

bool MasterActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!IActor::Init(actor_net))
	{
		return false;
	}

	network_ptr_ = std::make_shared<network::Network>(actor_net->config().master_port);
	if (!network_ptr_)
	{
		//LOG network init fail
		return false;
	}
	network_ptr_->Run();
	std::cout << "MasterActor start!!!" << std::endl;
	return true;
}

void MasterActor::Stop()
{

}

void MasterActor::OnReceive(const ActorMessagePtr& actor_msg)
{
	char* p = new char[actor_msg->size() + 1];
	std::memcpy(p, actor_msg->data().data(), actor_msg->size());
	p[actor_msg->size()] = 0;

	std::cout << p << std::endl;
}

ACTOR_IMPLEMENT(MasterActor)