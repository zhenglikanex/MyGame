#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/ViewService.hpp"
#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/game/Component/Transform.hpp"

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
			auto v = locator.Ref<ViewService>().Create(asset.value);

			Transform transform;
			v->MovePosition(transform.position);

			registry.emplace<View>(e,std::move(v));
			registry.emplace<Transform>(e,std::move(transform));
		}
	}

	void Finalize() override
	{

	}
};
