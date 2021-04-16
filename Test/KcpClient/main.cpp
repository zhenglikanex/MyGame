#include <iostream>

#include "Client/ClientNetworkService.hpp"

int main()
{
	auto msg = MakeReqKcpConnectMsg();
	auto flag = IsReqKcpConnectMsg(msg.c_str(), msg.size());
	ClientNetworkService service;
	service.Run();
	service.Connect("127.0.0.1", 9523, 5);

	// 注意这在另外一个线程
	service.set_connect_handler([](ClientNetworkService::ConnectStatus status) 
		{
			if (status == ClientNetworkService::ConnectStatus::kTypeConnected)
			{
				std::cout << "网络连接成功！" << std::endl;
			}
			else if(status == ClientNetworkService::ConnectStatus::kTypeTimeout)
			{
				std::cout << "网络超时！" << std::endl;
			}
		});

	while (true)
	{
		
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}

	return 0;
}