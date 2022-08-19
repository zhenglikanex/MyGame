#pragma once

#include "Framework/Game/Service/DebugService.hpp"

class ServerDebugService : public DebugService
{
public:
	void Info(std::string_view view) const override;
	void Warning(std::string_view view) const override;
	void Error(std::string_view view) const override;

	void ClearDraw() const override;
	void DrawCube(const vec3& position, const quat& rotation, const vec3& size) const override;
	void DrawSphere(const vec3& position, float radius) const override;
	void DrawCapsule(const vec3& position, const quat& rotation, float half_height, float radius) const override;
private:
};