#pragma once

#include <functional>

#include "Framework/Game/Component/DelayTask.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct DelayTaskSystem : System
{
public:
	std::unordered_map<std::string, std::function<void(void*)>> events;

	DelayTaskSystem(entt::registry& _registry) : System(_registry) 
	{
		events.emplace("test", [&](void* data)
			{
				
			});
	}
	~DelayTaskSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		float time = static_cast<float>(dt);

		if (auto delay_task = registry.try_ctx<DelayTask>())
		{
			auto it = delay_task->entity_by_events.begin();
			while (it != delay_task->entity_by_events.end())
			{
				if (registry.valid(it->first))
				{
					auto event_it = it->second.begin();
					while (event_it != it->second.end())
					{
						auto& event = *event_it;
						event.time -= time;
						if (event.time <= 0)
						{
							if (event.callback)
							{
								event.callback();
							}

							event_it = it->second.erase(event_it);
						}
						else
						{
							++event_it;
						}
					}

					++it;
				}
				else
				{
					it = delay_task->entity_by_events.erase(it);
				}
			}
		}
	}

	void Finalize() override
	{

	}
};