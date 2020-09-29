#include <iostream>
#include "nlohmann/json.hpp"
#include <string_view>
#include "Framework/serialize/binary_archive.hpp"
#include "Framework/serialize/binary_stream.hpp"

int main()
{
	nlohmann::json json = 3.14;
	std::cout << json << std::endl;
	return 0;
}