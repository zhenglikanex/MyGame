#define CATCH_CONFIG_MAIN
#include "3rdparty/include/catch2/catch.hpp"
#include "Framework/Game/Math.hpp"



struct Serialize
{

};

TEST_CASE_METHOD(Serialize, "Fixed")
{
	kanex::Buffer buffer(1000);
	kanex::BinaryStream stream(buffer);
	kanex::BinaryOutputArchive oar(stream);
	kanex::BinaryInputArchive iar(stream);

	fixed16 src1(100);
	fixed16 src2(200);
	fixed16 src3(0.3);

	oar(src1);
	oar(src2);
	oar(src3);

	fixed16 out1;
	fixed16 out2;
	fixed16 out3;

	iar(out1);
	iar(out2);
	iar(out3);

	REQUIRE(src1 == out1);
	REQUIRE(src2 == out2);
	REQUIRE(src3 == out3);

	REQUIRE(buffer.IsFinish());
}
