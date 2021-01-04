#include "FileService.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

std::string FileService::OpenFile(std::string_view file) const
{
	std::filesystem::path path = cur_path_ + file.data();

	if (std::filesystem::exists(path))
	{
		std::ifstream in(path.string());
		if (in)
		{
			std::stringstream sstream;
			sstream << in.rdbuf();
			return sstream.str();
		}
	}

	return "";
}

