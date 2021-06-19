#include "ServerFileService.hpp"

#include <fstream>

std::string ServerFileService::OpenFile(std::string_view file) const
{
	std::ifstream in(file);
	if (in.is_open())
	{
		std::string content;
		in >> content;
		return content;
	}
	return "";
}