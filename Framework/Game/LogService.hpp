#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "Framework/Game/Service.hpp"
#include "Framework/Game/Fmt.hpp"

#ifdef DEBUG
#define INFO(...)  g_log_service->Info(__VA_ARGS__);
#define WARNING(...) g_log_service->Warning(__VA_ARGS__);
#define ERROR(...) g_log_service->Error(__VA_ARGS__);
#else
#define INFO(...)
#define WARNING(...)
#define ERROR(...)
#endif

class LogService : public Service
{
public:
	virtual ~LogService() = 0 {}

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
private:
};

extern std::unique_ptr<LogService> g_log_service;