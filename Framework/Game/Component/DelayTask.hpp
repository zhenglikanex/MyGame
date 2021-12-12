#pragma once

#include <vector>
#include <string>
#include <functional>

struct DelayTaskEvent
{
	float time;
	std::function<void()> callback;

	DelayTaskEvent(float _time, const std::function<void()>& _callback)
		: time(_time)
		, callback(_callback)
	{

	}
};

struct DelayTask
{
	std::unordered_map<entt::entity, std::vector<DelayTaskEvent>> entity_by_events;
};