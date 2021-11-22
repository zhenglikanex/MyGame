#pragma once

#pragma once

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "Framework/Game/System.hpp"

struct GameplayAttributeSystem : public System
{
	entt::observer modify_base_list{ registry, entt::collector.group<GameplayAttributeSet,ModifierBaseAttributeSet>().update<ModifierBaseAttributeSet>().where<GameplayAttributeSet>() };

	entt::observer modify_list{ registry, entt::collector.group<GameplayAttributeSet,ModifierAttributeSet>().update<ModifierAttributeSet>().where<GameplayAttributeSet>() };

	GameplayAttributeSystem(entt::registry& _registry) : System(_registry) { }
	~GameplayAttributeSystem() {}

	bool Initialize() override
	{
		return true;
	}

	float Sum(const std::vector<ModifierAttribute>& modifiers,GameplayEffect::ModifierOp modifier_op)
	{
		float sum = 0;
		for (auto& mod : modifiers)
		{
			if (mod.modifier_op == modifier_op)
			{
				sum += mod.value;
			}
		}
		return sum;
	}

	// 计算属性的修改
	void Modify()
	{
		//计算base_value
		if (modify_base_list.size() > 0)
		{
			auto view = registry.view<GameplayAttributeSet, ModifierBaseAttributeSet>();
			for (auto e : modify_base_list)
			{
				auto&[attribute_set, mod_base_attr_list] = view.get<GameplayAttributeSet, ModifierBaseAttributeSet>(e);

				for (int i = 0; i < mod_base_attr_list.value.size(); ++i)
				{
					auto& modifiers = mod_base_attr_list.value[i];

					float additive = Sum(modifiers, GameplayEffect::ModifierOp::Add);
					float multiply = Sum(modifiers, GameplayEffect::ModifierOp::Multiply);
					float divide = Sum(modifiers, GameplayEffect::ModifierOp::Divide);

					attribute_set.value[i].base_value = (attribute_set.value[i].base_value + additive) * multiply * divide;
				}
			}
			
		}

		//计算current_value
		if (modify_list.size() > 0)
		{
			auto view = registry.view<GameplayAttributeSet, ModifierAttributeSet>();
			for (auto e : modify_list)
			{
				auto&[attribute_set, mod_attr_list] = view.get<GameplayAttributeSet, ModifierAttributeSet>(e);
				
				for (int i = 0; i < mod_attr_list.value.size(); ++i)
				{
					auto& modifiers = mod_attr_list.value[i];

					float additive = Sum(modifiers, GameplayEffect::ModifierOp::Add);
					float multiply = Sum(modifiers, GameplayEffect::ModifierOp::Multiply);
					float divide = Sum(modifiers, GameplayEffect::ModifierOp::Divide);

					attribute_set.value[i].current_value = (attribute_set.value[i].base_value + additive) * multiply * divide;
				}
			}
		}

		// 计算override


		registry.clear<ModifierBaseAttributeSet>();

		modify_base_list.clear();
		modify_list.clear();
	}

	void FixedUpdate(fixed16 dt) override
	{
		Modify();
	}

	void Finalize() override
	{

	}
};