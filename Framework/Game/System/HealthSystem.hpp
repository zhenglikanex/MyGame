#pragma once

#include "Framework/Game/Component/Health.hpp"

#include "Framework/Game/System.hpp"

struct HealthSystem : public System
{
	entt::observer update_list{ registry,entt::collector.group<Health,AttributeUnitList>().update<AttributeUnitList>().where<Health>() };

	HealthSystem(entt::registry& _registry) : System(_registry) { }
	~HealthSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		InitHealth();
		UpdateHealth();
	}

	void InitHealth()
	{
		auto view = registry.view<AttributeUnitList>(entt::exclude<Health>);
		if (view.size() > 0)
		{
			std::array<fixed16,(size_t)AttributeType::kMax> percentages;
			for (auto e : view)
			{
				const auto& attribute_units = view.get<AttributeUnitList>(e);
				auto& health = registry.emplace<Health>(e);
				
				CalculateHealth(health, attribute_units, percentages);
				health.cur = health.max;
			}
		}
	}

	void UpdateHealth()
	{
		auto view = registry.view<AttributeUnitList,Health>();
		if (update_list.size() > 0)
		{
			std::array<fixed16, (size_t)AttributeType::kMax> percentages;
			for (auto e : update_list)
			{
				const auto& attribute_units = view.get<AttributeUnitList>(e);
				auto& health = view.get<Health>(e);
				CalculateHealth(health, attribute_units, percentages);
			}
		}
		update_list.clear();
	}

	void CalculateHealth(Health& health, const AttributeUnitList& attribute_units, std::array<fixed16, (size_t)AttributeType::kMax>& percentages)
	{
		for (const auto& unit : attribute_units.value)
		{
			for (auto index = 0; index < unit.attributes.size(); ++index)
			{
				auto& attribute = unit.attributes[index];
				if (attribute.calc_type == CalculateType::kPercentage)
				{
					percentages[index] += attribute.value;
				}
				else
				{
					health.max[index] += attribute.value;
				}
			}
		}

		for (auto index = 0; index < percentages.size(); ++index)
		{
			if (percentages[index] != fixed16(0))
			{
				health.max[index] *= percentages[index];
				percentages[index] = fixed16(0);
			}
		}
	}

	void Finalize() override
	{

	}
};
