#include "ServerDebugService.hpp"

#include <iostream>

void ServerDebugService::Info(std::string_view view) const
{
	std::cout << "[INFO]:" << view << std::endl;
}

void ServerDebugService::Warning(std::string_view view) const
{
	std::cout << "[WARNING]:" << view << std::endl;
}

void ServerDebugService::Error(std::string_view view) const
{
	std::cout << "[ERROR]:" << view << std::endl;
}

void ServerDebugService::ClearDraw() const
{

}

void ServerDebugService::DrawCube(const vec3& position, const quat& rotation, const vec3& size) const
{

}

void ServerDebugService::DrawSphere(const vec3& position, float radius) const
{

}

void ServerDebugService::DrawCapsule(const vec3& position, const quat& rotation, float half_height, float radius) const
{

}