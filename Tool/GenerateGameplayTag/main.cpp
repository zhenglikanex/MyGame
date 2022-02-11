#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <unordered_map>
#include <filesystem>
#include <set>




std::vector<std::string> Split(std::string_view strv,std::string_view delims = ".")
{
	std::vector<std::string> tokens;

	size_t first = 0;

	while (first < strv.size())
	{
		const auto second = strv.find_first_of(delims, first);

		if (first != second)
			tokens.emplace_back(std::string(strv.substr(first, second - first)));

		if (second == std::string_view::npos)
			break;

		first = second + 1;
	}

	return tokens;
}

std::string GenTagName(const std::vector<std::string>& tokens,size_t size)
{
	std::string name = "k";
	for (int i = 0; i < size; ++i)
	{
		name += tokens[i];
	}

	return name;
}

void OutCPulsPuls(const std::string& enums)
{
	std::string head = 
R"(#pragma once

enum class GameplayTagEnums : uint64_t
{
	)";

	
	std::ofstream out("Framework/Game/GAS/GameplayTagEnums.hpp", std::ios_base::out);
	out << head + enums + "\n};";

}

void OutLua(const std::string& enums)
{
	std::string head =
		R"(GameplayTagEnums = 
{
	)";


	std::ofstream out("Asset/Lua/GAS/GameplayTagEnums.lua", std::ios_base::out);
	out << head + enums + "\n};";

}

int main() 
{
	std::cout << std::filesystem::current_path() << std::endl;


	std::ifstream in("GameplayTagDefine.txt");

	std::unordered_map<std::string, uint64_t> tag_map;

	std::array<std::unordered_map<std::string, uint8_t>, 8> id_map_array;

	char line[4096];
	while (in.getline(line, 4096))
	{
		if (line[0] == '/')
		{
			continue;
		}

		auto tokens = Split(line);
		
		if (tokens.size() <= 0)
		{
			continue;
		}

		if (tokens.size() >= 8)
		{
			std::cout << line << " " << "Tag层级超过8层!" << std::endl;
			continue;
		}

		uint64_t value = 0;

		for (auto i = 0; i < tokens.size(); ++i)
		{
			uint8_t sub_value = 0;

			auto& id_map = id_map_array[i];
			auto it = id_map.find(tokens[i]);
			if (it != id_map.end())
			{
				sub_value = it->second;
			}
			else
			{
				sub_value = id_map.size() + 1;
				id_map.emplace(tokens[i], sub_value);

				if (sub_value > 255)
				{
					std::cout << line << " " << tokens[i - 1] << ":" << "该层级的大于255" << std::endl;
				}
			}

			value = (sub_value << (7 - i) * 8) | value;

			auto name = GenTagName(tokens,i + 1);
			if (tag_map.find(name) == tag_map.end())
			{
				tag_map.emplace(name, value);
			}
		}

		auto name = GenTagName(tokens, tokens.size());
		if (tag_map.find(name) == tag_map.end())
		{
			tag_map.emplace(name, value);
		}
	}

	
	std::set<std::string> order;
	
	for (auto& item : tag_map)
	{
		order.emplace(item.first);
	}

	

	std::string enums;

	int num = 0;
	for (auto& name : order)
	{
		++num;
		enums = enums + name + "=" + std::to_string(tag_map[name]) + (order.size() == num ? "" : ",\n\t");
	}

	OutCPulsPuls(enums);
	OutLua(enums);
	return 0;
}

