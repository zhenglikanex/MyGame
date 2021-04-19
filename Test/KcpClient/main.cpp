#include <iostream>

#include "Client/ClientNetworkService.hpp"
using namespace std::chrono;

int main()
{
	std::vector<ClientNetworkService> services(300);

	auto thread1 = std::thread([&services]() {
		for (auto& service : services)
		{
			// 注意这在另外一个线程
			service.set_connect_handler([](ClientNetworkService::ConnectStatus status)
				{
					if (status == ClientNetworkService::ConnectStatus::kTypeConnected)
					{
						//std::cout << "网络连接成功！" << std::endl;
					}
					else if (status == ClientNetworkService::ConnectStatus::kTypeTimeout)
					{
						//std::cout << "网络超时！" << std::endl;
					}
				});
			service.Run();
		}
		ClientNetworkService::io_context_.run();
		});

	for (auto& service : services)
	{
		service.Connect("127.0.0.1", 9523, 5);
	}
	
	auto thread2 = std::thread([&services]()
		{
			while (true)
			{
				uint32_t n = 0;
				uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
				auto ping = 0;
				for (auto& service : services)
				{
					while (!service.IsEmpty())
					{
						auto recv_msg = service.PopMessage();
						uint32_t send_clock;
						std::copy_n(recv_msg->data().data(), sizeof(uint32_t), (uint8_t*)&send_clock);
						ping = now - send_clock;
					}
				}

				if (ping > 0)
				{
					std::cout << ping << std::endl;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
			
		});
	
	while (true)
	{
		uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		//std::cout << "send :" << now << std::endl;
		std::vector<uint8_t> data(sizeof(now + 10240));
		std::copy_n((uint8_t*)&now, data.size(), data.data());
		NetMessage message;
		message.set_session(0);
		message.set_name("ping");
		message.set_data(std::move(data));

		std::vector<uint8_t> buffer(message.GetByteSize());
		message.Serialize(buffer.data());
		for (auto& service : services)
		{
			service.Send(buffer.data(), buffer.size());
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}