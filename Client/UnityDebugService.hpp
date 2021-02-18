#pragma once

#include "Framework/Game/DebugService.hpp"

class UnityDebugService : public DebugService
{
public:
	void Info(std::string_view view) const override;
	void Warning(std::string_view view) const override;
	void Error(std::string_view view) const override;

	void DrawCube(const vec3& position, const quat& rotation, const vec3& size) const override;
	void DrawSphere(const vec3& position, fixed16 radius) const override;
	void DrawCapsule(const vec3& position, const quat& rotation, fixed16 height, fixed16 radius) const override;
};