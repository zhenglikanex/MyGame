#pragma once

#include <vector>
#include <string>
#include <functional>

struct DelayTaskEvent
{
	float time;
	std::function<void()> callback;

	DelayTaskEvent(float _time,const std::function<void()>& _callback)
		: time(_time)
		, callback(_callback)
	{

	}
};

struct DelayTask
{
	std::vector<DelayTaskEvent> events;
};