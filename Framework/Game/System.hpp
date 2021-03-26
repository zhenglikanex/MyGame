#pragma once

#include <type_traits>

#include "FixedPoint.hpp"

#include "entt/entt.hpp"

struct System
{
	System(entt::registry& _registry) : registry(_registry) { }
	virtual ~System() = 0 {}

	virtual bool Initialize() = 0;
	virtual void Update(fixed16 dt) = 0;
	virtual void LateUpdate(fixed16 dt) {}
	virtual void Finalize() = 0;

	entt::registry& registry;
};

struct ObserverSystem : System
{
	ObserverSystem(entt::registry& _registry)
		:System(_registry)
	{

	}

	template<class Matcher>
	entt::observer* CreateObserver(Matcher&& matcher)
	{
		return observers_.emplace_back(registry,std::forward(matcher));
	}

	void RemoveObserver(entt::observer* observer)
	{
		auto iter = std::find_if(observers_.begin(), observers_.end(), [observer](const entt::observer& ob)
		{
			return &ob == observer;
		});

		if (iter != observers_.end())
		{
			observers_.erase(iter);
		}
	}

	virtual void Connect()
	{
		
	}

	virtual void Disconnect()
	{

	}

	virtual void Clear()
	{
		for (auto& observer : observers_)
		{
			observer.clear();
		}
	}

private:
	std::vector<entt::observer> observers_;
};