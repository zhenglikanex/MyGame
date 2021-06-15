#define CATCH_CONFIG_MAIN

#include "3rdparty/include/catch2/catch.hpp"
#include <chrono>
#include "Framework/Proto/NetMessage.hpp"
#include "Framework/Proto/Battle.pb.h"

using namespace std::chrono;

struct Serialize1
{
	
};


TEST_CASE_METHOD(Serialize1, "NetMessage")
{
	for (int i = 0; i < 10000; ++i)
	{
		Proto::GameCommandGroup group;
		group.set_frame(i);

		auto commands = group.mutable_commands();
		for (int j = 0; j < 2; ++j)
		{
			(*commands)[j] = Proto::GameCommand();
		}
		auto buffer = Serialize(group);
		REQUIRE(buffer.size() == group.ByteSize());
		NetMessage message;
		message.set_name("push_command_group");
		message.set_data(buffer);

		std::vector<uint8_t> buffer2(message.GetByteSize());
		message.Serialize(buffer2.data());
		std::cout << buffer2.size() << std::endl;
		NetMessage rsp;
		rsp.Parse(buffer2.data(), buffer2.size());
		

		Proto::GameCommandGroup group2;
		group.ParseFromArray(rsp.data().data(), rsp.data().size());

		std::cout << rsp.data().size() << std::endl;
		REQUIRE(rsp.data().size() == buffer.size());
	}
}
