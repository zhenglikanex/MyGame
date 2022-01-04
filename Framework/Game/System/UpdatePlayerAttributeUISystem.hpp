#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/UIService.hpp"

#include "Framework/Game/Component/Local.hpp"
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
		auto& ui_service = registry.ctx<Locator>().Ref<UIService>();

		auto view = registry.view<LocalId, GameplayAttributeSet>();
		for (auto e : *updatelist)
		{
			auto& attribute_set = view.get<GameplayAttributeSet>(e);
			ui_service.RefreshUI(
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::Hp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::MaxHp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::Mp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::MaxMp)].current_value);
		}

		updatelist->clear();
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