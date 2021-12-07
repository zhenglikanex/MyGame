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
			auto it = delay_task->events.begin();
			while (it != delay_task->events.end())
			{
				auto& event = *it;
				event.time = event.time - time;
				if (event.time <= 0)
				{
					if (event.is_from_lua)
					{
						auto& lua_state = registry.ctx<sol::state>();
						lua_state[event.fun_name](event.data);
					}
					else
					{
						
					}

					it = delay_task->events.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}

	void Finalize() override
	{

	}
};