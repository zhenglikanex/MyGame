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
		return observers_.emplace_back(std::make_unique<entt::observer>(registry,std::forward<Matcher>(matcher))).get();
	}

	virtual void Connect() = 0;
	virtual void Disconnect() = 0;

	virtual void Clear()
	{
		for (auto& observer : observers_)
		{
			observer->clear();
		}
	}

private:
	std::vector<std::unique_ptr<entt::observer>> observers_;
};