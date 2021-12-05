#pragma once

#include <vector>
#include <string>

struct DelayTaskEvent
{
	float time;
	std::string fun_name;
	bool is_from_lua;
	void* data;
};

struct DelayTask
{
	std::vector<DelayTaskEvent> events;
};