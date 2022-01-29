#pragma once

#include <numeric>
#include <algorithm>
#include <optional>

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"
#include "Framework/Game/Component/GAS/ExecuteAttribute.hpp"

#include "Framework/Game/System.hpp"

struct GameplayAttributeSystem : public System
{
	entt::observer modify_list{ registry, entt::collector.group<GameplayAttributeSet,InfiniteModifierAttributeSet>().update<InfiniteModifierAttributeSet>().where<GameplayAttributeSet>() };

	GameplayAttributeSystem(Registry& _registry) : System(_registry) { }
	~GameplayAttributeSystem() {}

	bool Initialize() override
	{
		return true;
	}

	float Sum(const std::vector<InfiniteModifierAttribute>& modifiers,GameplayEffect::ModifierOp modifier_op)
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

	std::optional<float> Override(const std::vector<InfiniteModifierAttribute>& modifiers)
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
		
		if (override)
		{
			return value;
		}

		return std::nullopt;
	}

	float CalculationCurrentValue(const std::vector<InfiniteModifierAttribute>& modifiers,float base_value)
	{
		auto result = Override(modifiers);
		if (result.has_value())
		{
			return result.value();
		}
		else
		{
			float additive = Sum(modifiers, GameplayEffect::ModifierOp::Add);
			float multiply = 1 + Sum(modifiers, GameplayEffect::ModifierOp::Multiply);
			float divide = 1 + Sum(modifiers, GameplayEffect::ModifierOp::Divide);

			return (base_value + additive) * multiply / divide;
		}
	}

	void Modify()
	{
		auto view = registry.view<ModifierAttribute>();
		for (auto e : view)
		{
			auto& modifier = view.get<ModifierAttribute>(e);
			
			if (!registry.valid(modifier.target))
			{
				continue;
			}

			entt::entity target = registry.get(modifier.target);			
			auto& [attr_set, infinite_attr_set] = registry.get<GameplayAttributeSet, InfiniteModifierAttributeSet>(target);
			
			auto& attr_data = attr_set.value[(size_t)modifier.attribute];
			attr_data.base_value += modifier.value;

			//ÖØÐÂ¼ÆËãcurrent_value
			{
				auto& modifiers = infinite_attr_set.value[(size_t)modifier.attribute];
				if (modifiers.size() > 0)
				{
					PostGameplayEffectExecute(attr_set,modifier.attribute, CalculationCurrentValue(modifiers, attr_data.base_value));
				}
			}
		}

		if (modify_list.size() > 0)
		{
			auto view = registry.view<GameplayAttributeSet, InfiniteModifierAttributeSet>();
			for (auto e : modify_list)
			{
				auto& [attr_set, infinite_attr_set] = view.get<GameplayAttributeSet, InfiniteModifierAttributeSet>(e);
				for (auto i = 0; i < infinite_attr_set.value.size(); ++i)
				{
					auto& modifiers = infinite_attr_set.value[i];
					if (modifiers.size() > 0)
					{
						PostGameplayEffectExecute(attr_set, static_cast<GameplayAttributeType>(i), CalculationCurrentValue(modifiers, attr_set.value[i].base_value));
					}
				}
			}
		}

		modify_list.clear();
	}

	void FixedUpdate(float dt) override
	{
		Modify();
	}

	void Finalize() override
	{

	}

	// 
	void PostGameplayEffectExecute(GameplayAttributeSet& attribute_set,GameplayAttributeType attribute,float value)
	{
		if (attribute == GameplayAttributeType::Hp)
		{
			attribute_set.value[(size_t)attribute].current_value = std::clamp(value, 0.0f, attribute_set.value[(size_t)GameplayAttributeType::MaxHp].current_value);
		}
		else if (attribute == GameplayAttributeType::Mp)
		{
			attribute_set.value[(size_t)attribute].current_value = std::clamp(value, 0.0f, attribute_set.value[(size_t)GameplayAttributeType::MaxMp].current_value);
		}
	}
};