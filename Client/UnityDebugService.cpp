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

void UnityDebugService::DrawCapsule(const vec3& position, const vec3& a, const vec3& b, fixed16 radius) const
{
	UnityBridge::Get().CallUnity<void>(
		"DrawCapsule",
		static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z),
		static_cast<float>(a.x), static_cast<float>(a.y), static_cast<float>(a.z),
		static_cast<float>(b.x), static_cast<float>(b.y), static_cast<float>(b.z),
		static_cast<float>(radius));
}