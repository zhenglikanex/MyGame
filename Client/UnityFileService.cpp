#include "UnityFileService.hpp"

#include "Client/UnityBridge.hpp"

std::string UnityFileService::OpenFile(std::string_view file) const
{
	return UnityBridge::Get().CallUnity<std::string>("OpenFile", file.data());
}