#pragma once

#include <numeric>
#include <algorithm>

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"
#include "Framework/Game/Component/GAS/ExecuteAttribute.hpp"

#include "Framework/Game/System.hpp"

struct GameplayAttributeSystem : public System
{
	entt::observer modify_list{ registry, entt::collector.group<GameplayAttributeSet,ModifierBaseAttributeSet,InfiniteModifierAttributeSet>().update<ModifierBaseAttributeSet>().update<InfiniteModifierAttributeSet>().where<GameplayAttributeSet>() };

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

	std::tuple<bool,float> Override(const std::vector<ModifierAttribute>& modifiers)
	{
		bool override = false;
		float value = std::numeric_limits<float>::min();
		
		for(auto & mod : modifiers)
		{
			if (mod.modifier_op == GameplayEffect::ModifierOp::Override)
			{
				override = true;
				value = std::max(value, mod.value);
			}
		}

		return { override,value };
	}

	// 计算属性的修改
	void Modify()
	{
		//计算base_value
		if (modify_list.size() > 0)
		{
			auto view = registry.view<GameplayAttributeSet,ModifierBaseAttributeSet,InfiniteModifierAttributeSet>();
			for (auto e : modify_list)
			{
				auto&[attribute_set, mod_base_attr_list,mod_attr_list] = view.get<GameplayAttributeSet, ModifierBaseAttributeSet, InfiniteModifierAttributeSet>(e);

				//计算base_value
				for (int i = 0; i < mod_base_attr_list.value.size(); ++i)
				{
					auto& modifiers = mod_base_attr_list.value[i];

					float additive = Sum(modifiers, GameplayEffect::ModifierOp::Add);
					float multiply = 1 + Sum(modifiers, GameplayEffect::ModifierOp::Multiply);
					float divide = 1 + Sum(modifiers, GameplayEffect::ModifierOp::Divide);

					attribute_set.value[i].base_value = (attribute_set.value[i].base_value + additive) * multiply * divide;

					modifiers.clear();
				}

				// 计算current_value
				for (int i = 0; i < mod_attr_list.value.size(); ++i)
				{
					auto& modifiers = mod_attr_list.value[i];

					float additive = Sum(modifiers, GameplayEffect::ModifierOp::Add);
					float multiply = 1 + Sum(modifiers, GameplayEffect::ModifierOp::Multiply);
					float divide = 1 + Sum(modifiers, GameplayEffect::ModifierOp::Divide);

					attribute_set.value[i].current_value = (attribute_set.value[i].base_value + additive) * multiply * divide;
				}

				// 计算base_value override
				for (int i = 0; i < mod_base_attr_list.value.size(); ++i)
				{
					auto& modifiers = mod_base_attr_list.value[i];
					auto [override, value] = Override(modifiers);
					if (override)
					{
						attribute_set.value[i].base_value += (value - attribute_set.value[i].current_value);
					}
				}

				// 计算current_value override
				for (int i = 0; i < mod_attr_list.value.size(); ++i)
				{
					auto& modifiers = mod_base_attr_list.value[i];
					auto [override, value] = Override(modifiers);
					if (override)
					{
						attribute_set.value[i].current_value = value;
					}
				}
			}
		}

		modify_list.clear();
	}

	void FixedUpdate(fixed16 dt) override
	{
		Modify();
	}

	void Finalize() override
	{

	}

	//在属性的base_value更改后调用
	void PostGameplayEffectExecute(entt::entity e,)
	{

	}
};