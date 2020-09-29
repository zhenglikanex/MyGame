#pragma once

#include <string>

#include "Framework/serialize/binary_archive.hpp"
#include "Framework/serialize/types/string.hpp"

struct Asset
{
	std::string value;
	
	BAR(value);
};