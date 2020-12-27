#include <iostream>
#include "nlohmann/json.hpp"
#include <string_view>
#include "Framework/serialize/binary_archive.hpp"
#include "Framework/serialize/binary_stream.hpp"

int main()
{
	nlohmann::json json;
	json["result"] = 0;
	std::string str = json.dump();
	nlohmann::json j1 = nlohmann::json::parse(str);
	std::cout << j1["result"] << std::endl;
	return 0;
}