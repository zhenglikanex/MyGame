#include "UnityDebugService.hpp"

#include <iostream>

#include "Client/UnityBridge.hpp"

UnityDebugService::UnityDebugService()
#ifdef WIN32
	: DebugService(),console_(NULL)
{
	AllocConsole();
	console_ = GetConsoleWindow();
	if (console_ != NULL)
	{
		ShowWindow(console_, SW_SHOW);
		BringWindowToTop(console_);
		FILE* file = NULL;
		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);

		HMENU hmenu = GetSystemMenu(console_, FALSE);
		if (hmenu != NULL)
		{
			DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}
}
#else
:DebugService(){}
#endif

UnityDebugService::~UnityDebugService()
#ifdef WIN32
{
	if (console_ != NULL)
	{
		FreeConsole();
		console_ = NULL;
	}
}
#else
{}
#endif


void UnityDebugService::Info(std::string_view view) const
{
#ifdef WIN32
	std::cout << "[INFO]:" << view << std::endl;
#endif
	UnityBridge::Get().CallUnity<void>("LogInfo",view.data());
}

void UnityDebugService::Warning(std::string_view view) const
{
#ifdef WIN32
	std::cout << "[WARNING]:" << view << std::endl;
#endif
	UnityBridge::Get().CallUnity<void>("LogWarning", view.data());
}

void UnityDebugService::Error(std::string_view view) const
{
#ifdef WIN32
	std::cout << "[ERROR]:" << view << std::endl;
#endif
	UnityBridge::Get().CallUnity<void>("LogError", view.data());
}

void UnityDebugService::ClearDraw() const
{
	UnityBridge::Get().CallUnity<void>("ClearDebugDraw");
}

void UnityDebugService::DrawCube(const vec3& position, const quat& rotation, const vec3& size) const
{
	UnityBridge::Get().CallUnity<void>(
		"DrawCube",
		position.x, position.y, position.z,
		rotation.x, rotation.y, rotation.z, rotation.w,
		size.x, size.y, size.z);
}

void UnityDebugService::DrawSphere(const vec3& position, float radius) const
{
	UnityBridge::Get().CallUnity<void>(
		"DrawSphere",
		position.x, position.y, position.z,
		radius);
}

void UnityDebugService::DrawCapsule(const vec3& position, const quat& rotation, float half_height, float radius) const
{
	UnityBridge::Get().CallUnity<void>(
		"DrawCapsule",
		position.x, position.y, position.z,
		rotation.x, rotation.y, rotation.z, rotation.w,
		half_height,
		radius);
}