#include <iostream>

#include "3rdparty/include/asio.hpp"

using namespace asio::ip;

enum { max_length = 1024 };

int main()
{
	asio::io_context io_context;

	udp::socket s(io_context, udp::endpoint(udp::v4(), 0));

	udp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 9523);

	while (true)
	{
		std::cout << "Enter message: ";
		char request[max_length];
		std::cin.getline(request, max_length);
		size_t request_length = std::strlen(request);
		s.send_to(asio::buffer(request, request_length), endpoint);
	}

	char reply[max_length];
	udp::endpoint sender_endpoint;
	size_t reply_length = s.receive_from(
		asio::buffer(reply, max_length), sender_endpoint);
	std::cout << "Reply is: ";
	std::cout.write(reply, reply_length);
	std::cout << "\n";
	return 0;
}