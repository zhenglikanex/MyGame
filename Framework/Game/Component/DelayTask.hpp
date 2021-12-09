#pragma once

#include <vector>
#include <string>

struct DelayTaskEvent
{
	bool is_from_lua;
	float time;
	std::string func_name;
	void* data;

	DelayTaskEvent(bool _is_from_lua, float _time, std::string_view _func_name, void* _data)
		: is_from_lua(_is_from_lua)
		, time(_time)
		, func_name(_func_name)
		, data(_data)
	{

	}
};

struct DelayTask
{
	std::vector<DelayTaskEvent> events;
};