#include "UnityDebugService.hpp"

#include "Client/UnityBridge.hpp"

void UnityDebugService::Info(std::string_view view) const
{
	UnityBridge::Get().CallUnity<void>("LogInfo",view.data());
}

void UnityDebugService::Warning(std::string_view view) const
{
	UnityBridge::Get().CallUnity<void>("LogWarning", view.data());
}

void UnityDebugService::Error(std::string_view view) const
{
	UnityBridge::Get().CallUnity<void>("LogError", view.data());
}

void UnityDebugService::DrawCube(const vec3& position, const quat& rotation, const vec3& size) const
{
	UnityBridge::Get().CallUnity<void>(
		"DrawCube",
		static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z),
		static_cast<float>(rotation.x), static_cast<float>(rotation.y), static_cast<float>(rotation.z), static_cast<float>(rotation.w),
		static_cast<float>(size.x), static_cast<float>(size.y), static_cast<float>(size.z));
}

void UnityDebugService::DrawSphere(const vec3& position, fixed16 radius) const
{
	UnityBridge::Get().CallUnity<void>(
		"DrawSphere",
		static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z),
		static_cast<float>(radius));
}

void UnityDebugService::DrawCapsule(const vec3& position, const quat& rotation, fixed16 height, fixed16 radius) const
{
	UnityBridge::Get().CallUnity<void>(
		"DrawCapsule",
		static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z),
		static_cast<float>(rotation.x), static_cast<float>(rotation.y), static_cast<float>(rotation.z), static_cast<float>(rotation.w),
		static_cast<float>(height),
		static_cast<float>(radius));
}