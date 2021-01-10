#pragma once

#include "Framework/Game/Data/RootMotionConfig.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

int main()
{
	std::cout << std::filesystem::current_path().string() << std::endl;

	std::cout << "11111" << std::endl;
	std::ifstream in("HeroRootMotion.json");
	if (!in)
	{
		std::cout << "错误" << std::endl;
	}
	std::stringstream stream;
	stream << in.rdbuf();
	std::string str = stream.str();
	std::cout << str << std::endl;

	std::unordered_map<float, float> m;
	m.emplace(1.5, 1.5);
	m.emplace(20.1, 1.5);

	json jj(m);
	std::string s = jj.dump();

	std::is_constructible<std::string, fixed16>::value;
	json j = json::parse(str);

	fixed16 f1(1.222);
	fixed16 f2("1.222");

	if (f1 == f2)
	{
		std::cout << "202021222223333" << std::endl;
	}

	std::unordered_map<fixed16, int> cmp{ {fixed16(1.0),10} };
	json jjj(m);
	std::string sss = jjj.dump();
	std::unordered_map<std::string, std::unordered_map<std::string, RootMotion>> cmap;
	cmap = j.get<std::unordered_map<std::string, std::unordered_map<std::string, RootMotion>>>();

	RootMotionConfig root_motion_config;
	root_motion_config.Load(str);
	try
	{
		
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	system("pause");
	return 0;
}