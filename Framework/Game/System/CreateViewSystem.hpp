#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/ViewService.hpp"

#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"

#include "Framework/Game/System.hpp"

struct CreateViewSystem : public System
{
	CreateViewSystem(entt::registry& _registry) : System(_registry) { }
	~CreateViewSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{	
		auto& locator = registry.ctx<Locator>();
		auto view = registry.view<ViewAsset>(entt::exclude<View>);
		for (auto e : view)
		{
			const auto& asset = view.get<ViewAsset>(e);
			registry.emplace<View>(e, locator.Ref<ViewService>().Create(asset.value));
		}
	}

	void Finalize() override
	{

	}
};
