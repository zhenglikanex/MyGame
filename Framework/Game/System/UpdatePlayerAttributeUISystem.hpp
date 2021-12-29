#pragma once

#include "Framework/Game/Component/LocalId.hpp"
#include "Framework/Game/Component/UIManager.hpp"
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
		auto& ui_manager = registry.ctx<UIManager>();
		auto view = registry.view<LocalId, GameplayAttributeSet>();
		for (auto e : *updatelist)
		{
			auto& attribute_set = view.get<GameplayAttributeSet>(e);
			ui_manager.value->RefreshUI(
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::Hp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::MaxHp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::Mp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::MaxMp)].current_value);
		}
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