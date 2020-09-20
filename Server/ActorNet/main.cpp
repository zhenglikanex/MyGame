#include <iostream>
#include <memory>
#include "ActorNet.h"
#include "rapidjson/document.h"

int main(int argc, char** argv)
{
	if (argc < 1)
	{
		return false;
	}

	rapidjson::Document doc;
	doc.Parse(argv[0]);
	if (doc.HasParseError() || doc.Empty())
	{
		std::cout << argv[0] << " load failed!" << std::endl;
		return false;
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