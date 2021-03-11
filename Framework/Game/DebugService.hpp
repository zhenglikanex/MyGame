#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "Framework/Game/Service.hpp"
#include "Framework/Game/Fmt.hpp"
#include "Framework/Game/Math.hpp"

#ifdef DEBUG
#define INFO(...)  g_debug_service->Info(__VA_ARGS__);
#define WARNING(...) g_debug_service->Warning(__VA_ARGS__);
#define ERROR(...) g_debug_service->Error(__VA_ARGS__);
#else
#define INFO(...)
#define WARNING(...)
#define ERROR(...)
#endif

class DebugService : public Service
{
public:
	virtual ~DebugService() = 0 {}

	template<class Format,class ... Args>
	std::enable_if_t<std::integral_constant<bool,(sizeof...(Args)>0)>::value,void>
		Info(const Format& fmt, Args&& ... args) const
	{
		Info(fmt::format(fmt, std::forward<Args>(args)...));
	}

	template<class Format, class ... Args>
	std::enable_if_t<std::integral_constant<bool, (sizeof...(Args) > 0)>::value, void>
		Warning(const Format& fmt, Args&& ... args) const
	{
		Warning(fmt::format(fmt, std::forward<Args>(args)...));
	}

	template<class Format, class ... Args>
	std::enable_if_t<std::integral_constant<bool, (sizeof...(Args) > 0)>::value, void>
		Error(const Format& fmt, Args&& ... args) const
	{
		Error(fmt::format(fmt, std::forward<Args>(args)...));
	}

	virtual void Info(std::string_view view) const = 0;
	virtual void Warning(std::string_view view) const = 0;
	virtual void Error(std::string_view view) const = 0;

	virtual void DrawCube(const vec3& position, const quat& rotation, const vec3& size) const = 0;
	virtual void DrawSphere(const vec3& position, fixed16 radius) const = 0;
	virtual void DrawCapsule(const vec3& position, const vec3& a, const vec3& b, fixed16 radius) const = 0;
private:
};

extern std::unique_ptr<DebugService> g_debug_service;