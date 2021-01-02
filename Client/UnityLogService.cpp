#include "UnityLogService.hpp"

#include "Client/UnityBridge.hpp"

void UnityLogService::Info(std::string_view view) const
{
	UnityBridge::Get().CallUnity<void>("LogInfo",view.data());
}

void UnityLogService::Warning(std::string_view view) const
{
	UnityBridge::Get().CallUnity<void>("LogWarning", view.data());
}

void UnityLogService::Error(std::string_view view) const
{
	UnityBridge::Get().CallUnity<void>("LogError", view.data());
}