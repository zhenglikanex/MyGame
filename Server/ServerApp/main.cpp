#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include "ActorNet.h"
#include "rapidjson/document.h"
#include "ActorMessage.h"

int main()
{
	std::ifstream f("server.json");
	if (!f)
	{
		std::cout << "·þÎñÆ÷ÅäÖÃ¼ÓÔØ´íÎó" << std::endl;
		return -1;
	}

	rapidjson::Document doc;
	{
		std::stringstream ss;
		ss << f.rdbuf();
		f.close();
		doc.Parse(ss.str().c_str());
	}

	actor_net::Config config;
	config.harbor = doc["harbor"].GetInt();
	config.master = doc["master"].GetInt();
	config.ip = doc["ip"].GetString();
	config.port = doc["port"].GetUint();
	config.master_ip = doc["master_ip"].GetString();
	config.master_port = doc["master_port"].GetUint();

	auto actor_net = std::make_shared<actor_net::ActorNet>(config);
	if (!actor_net->Start())
	{
		std::cout << "Start Failed!" << std::endl;
	}

	actor_net->StartActor("KcpGateActor.dll");
	actor_net->StartUniqueActor("MatchActor.dll", "MatchActor");

	while (true)
	{
		std::string str;
		while (true)
		{
			getline(std::cin, str);

			if (str == "quit")
			{
				break;
			}

			//actor_net->SendActorMessage(0, "start_actor",0,actor_net::ActorMessage::MessageType::kTypeActor,"print",std::any(std::move(str)));
		}
	}
	actor_net->Stop();

	system("pause");
	return 0;
}