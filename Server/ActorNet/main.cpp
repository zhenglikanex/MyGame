#include <iostream>

#include "ActorNet.h"
#include <memory>

int main()
{
	auto actor_net = std::make_shared<actor_net::ActorNet>();
	if (!actor_net->Start())
	{
		std::cout << "Start Failed!" << std::endl;
	}
	while (true)
	{
		std::string str;
		while (true)
		{
			getline(std::cin, str);
			std::vector<uint8_t> data(str.size());
			std::memcpy(data.data(), str.c_str(), data.size());

			actor_net->SendActorMessage("", "start_actor", data);

			if (str == "quit")
			{
				break;
			}
		}
	}
	actor_net->Stop();

	system("pause");
	return 0;
}