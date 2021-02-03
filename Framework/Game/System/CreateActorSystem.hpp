#pragma once

#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/ViewService.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/ActorState.hpp"

#include "Framework/Game/System.hpp"

struct CreateActorSystem : public System
{
	std::unordered_map<std::string,>

	CreateActorSystem(entt::registry& _registry) : System(_registry) { }
	~CreateActorSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto& locator = registry.ctx<Locator>();
		auto view = registry.view<ActorAsset>(entt::exclude<ActorState>);
		for (auto e : view)
		{
			const auto& asset = view.get<ActorAsset>(e);
			
		}
	}

	void Finalize() override
	{

	}
};
