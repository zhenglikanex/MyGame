#include <iostream>

#include "Client/ClientNetworkService.hpp"


int main()
{

	ClientNetworkService service;
	service.Run();
	service.Connect("127.0.0.1", 9523, 5);

	// ע����������һ���߳�
	service.set_connect_handler([](ClientNetworkService::ConnectStatus status) 
		{
			if (status == ClientNetworkService::ConnectStatus::kTypeConnected)
			{
				std::cout << "�������ӳɹ���" << std::endl;
			}
		});

	while (true)
	{
		
	}

	return 0;
}