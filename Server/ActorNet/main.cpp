#include <iostream>

#include "ActorNet.h"
#include <memory>

int main()
{
	std::shared_ptr<actor_net::ActorNet> actor_net_ptr = std::make_shared<actor_net::ActorNet>();
	if (actor_net_ptr->Start())
	{
		
	}

	actor_net_ptr->Stop();

	system("pause");
	return 0;
}