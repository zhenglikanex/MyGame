#pragma once

#include <string>

#include "Framework/serialize/binary_archive.hpp"
#include "Framework/serialize/types/string.hpp"

struct ViewAsset
{
	std::string value;
	
	BAR(value);
};