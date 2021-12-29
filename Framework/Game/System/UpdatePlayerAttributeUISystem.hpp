#pragma once

#include "Framework/Game/Component/LocalId.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

#include "Framework/Game/System.hpp"

struct UpdatePlayerAttributeUISystem : public ObserverSystem
{
	entt::observer* updatelist;

	UpdatePlayerAttributeUISystem(entt::registry& _registry)
		: ObserverSystem(_registry)
		, updatelist(nullptr)
	{

	}

	~UpdatePlayerAttributeUISystem() {}

	bool Initialize() override
	{
		updatelist = CreateObserver(entt::collector.group<LocalId, GameplayAttributeSet>().update<GameplayAttributeSet>().where<LocalId>());


		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		//auto& ui_manager = registry.ctx<UIManager>
	}

	void Finalize() override
	{

	}

	void Connect() override
	{
		updatelist->connect(registry, entt::collector.group<LocalId, GameplayAttributeSet>().update<GameplayAttributeSet>().where<LocalId>());
	}

	void Disconnect() override
	{
		updatelist->disconnect();
	}
};