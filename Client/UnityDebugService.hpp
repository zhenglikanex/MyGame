#pragma once

#include "Framework/Game/Service/DebugService.hpp"

#ifdef WIN32
#include <windows.h>
#endif

class UnityDebugService : public DebugService
{
public:
	UnityDebugService();
	~UnityDebugService() override;

	UnityDebugService(const UnityDebugService&) = delete;
	UnityDebugService(UnityDebugService&&) = default;
	UnityDebugService& operator=(const UnityDebugService&) = delete;
	UnityDebugService& operator=(UnityDebugService&&) = default;
	

	void Info(std::string_view view) const override;
	void Warning(std::string_view view) const override;
	void Error(std::string_view view) const override;

	void DrawCube(const vec3& position, const quat& rotation, const vec3& size) const override;
	void DrawSphere(const vec3& position, float radius) const override;
	void DrawCapsule(const vec3& position, const quat& rotation,float half_height,float radius) const override;
private:
#ifdef WIN32
	HWND console_;
#endif
};