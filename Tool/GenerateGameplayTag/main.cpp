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

std::string GenTagName(const std::vector<std::string>& tokens,int size)
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
#include "Framework/Game/GAS/GameplayTag.hpp"

namespace GameplayTagDefine
{
	)";

	
	std::ofstream out("Framework/Game/GAS/GameplayTagDefine.hpp", std::ios_base::out);
	out << head + enums + "\n};";
}

void OutLua(const std::string& enums)
{
	std::string head =
		R"(GameplayTagDefine = 
{
	)";


	std::ofstream out("Unity/ARPG/Assets/Resources/Lua/Script/GAS/GameplayTagDefine.lua", std::ios_base::out);
	out << head + enums + "\n};\nreturn";

}

struct Tag
{
	int64_t mask;
	int64_t value;
};

int main() 
{
	std::cout << std::filesystem::current_path() << std::endl;


	std::ifstream in("GameplayTagDefine.txt");

	std::unordered_map<std::string, Tag> tag_map;

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

		int64_t value = 0;
		int64_t mask = 0;

		for (auto i = 0; i < tokens.size(); ++i)
		{
			int64_t sub_value = 0;

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
					if (i > 0)
					{
						auto v = i - 1;
						std::cout << line << " " << tokens[v] << ":" << "该层级的大于255" << std::endl;
					}
					else
					{
						std::cout << "顶层该层级的大于255" << std::endl;
					}
				}
			}

			
			value = (sub_value << (7 - i) * 8) | value;
			int64_t sub_mask = 0xFF;
			mask = (sub_mask << (7 - i) * 8) | mask;

			auto name = GenTagName(tokens,i + 1);
			if (tag_map.find(name) == tag_map.end())
			{
				tag_map.emplace(name, Tag{ mask,value });
			}
		}

		auto name = GenTagName(tokens, tokens.size());
		if (tag_map.find(name) == tag_map.end())
		{
			tag_map.emplace(name, Tag{ mask,value });
		}
	}

	
	std::set<std::string> order;
	
	for (auto& item : tag_map)
	{
		order.emplace(item.first);
	}

	
	{
		std::string enums;

		int num = 0;
		for (auto& name : order)
		{
			++num;
			enums = enums + "const GameplayTag " + name + " = " + "GameplayTag(" + std::to_string(tag_map[name].mask) + "," + std::to_string(tag_map[name].value) + ")" + ";\n\t";
		}

		OutCPulsPuls(enums);
	}

	{
		std::string enums;

		int num = 0;
		for (auto& name : order)
		{
			++num;
			enums = enums + name + " = " + "GameplayTag.new(" + std::to_string(tag_map[name].mask) + "," + std::to_string(tag_map[name].value) + ")" + ",\n\t";
		}
		OutLua(enums);
	}
	
	return 0;
}

