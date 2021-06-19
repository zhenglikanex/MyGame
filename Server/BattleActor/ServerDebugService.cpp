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

void ServerDebugService::DrawCube(const vec3& position, const quat& rotation, const vec3& size) const
{

}

void ServerDebugService::DrawSphere(const vec3& position, fixed16 radius) const
{

}

void ServerDebugService::DrawCapsule(const vec3& position, const vec3& a, const vec3& b, fixed16 radius) const
{

}