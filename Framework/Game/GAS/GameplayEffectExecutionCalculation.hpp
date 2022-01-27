#pragma once

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/HpModify.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

#include "3rdparty/include/entt/entt.hpp"

enum class GameplayAttributeCaptureOwner : uint8_t
{
	kSource,
	kTarget,
};

struct GameplayAttributeCapture
{
	GameplayAttributeType attribute;
	GameplayAttributeCaptureOwner owner;
	bool snapshot;

	GameplayAttributeCapture(GameplayAttributeType _attribute, GameplayAttributeCaptureOwner _owner, bool _snapshot)
		: attribute(_attribute)
		, owner(_owner)
		, snapshot(_snapshot)
	{

	}
};

class GameplayEffectExecutionCalculation
{
public:
	virtual ~GameplayEffectExecutionCalculation() = 0
	{

	}
	
	virtual void Capture(entt::registry& registry, GameplayEffectSpec& spec) = 0;
	virtual void ExecutionCalculation(entt::registry&,const GameplayEffectSpec& spec) = 0;

protected:
	void Capture(entt::registry& registry,GameplayEffectSpec& spec,const GameplayAttributeCapture& capture)
	{
		if (capture.snapshot)
		{
			if (capture.owner == GameplayAttributeCaptureOwner::kTarget)
			{
				auto& unique_map = registry.ctx<EntityByUniqueIdMap>();
				entt::entity target = unique_map.value.find(spec.target)->second;
				spec.target_snapshot_attrs.emplace(capture.attribute, registry.get<GameplayAttributeSet>(target).value[(size_t)capture.attribute].current_value);
			}
			else
			{
				auto& unique_map = registry.ctx<EntityByUniqueIdMap>();
				entt::entity source = unique_map.value.find(spec.source)->second;
				spec.source_snapshot_attrs.emplace(capture.attribute, registry.get<GameplayAttributeSet>(source).value[(size_t)capture.attribute].current_value);
			}
		}
	}

	float Get(entt::registry& registry,const GameplayEffectSpec& spec,const GameplayAttributeCapture& capture)
	{
		if (capture.snapshot)
		{
			if (capture.owner == GameplayAttributeCaptureOwner::kTarget)
			{
				return spec.target_snapshot_attrs.find(capture.attribute)->second;
			}
			else
			{
				return spec.source_snapshot_attrs.find(capture.attribute)->second;
			}
		}
		else
		{
			if (capture.owner == GameplayAttributeCaptureOwner::kTarget)
			{
				auto& unique_map = registry.ctx<EntityByUniqueIdMap>();
				entt::entity target = unique_map.value.find(spec.target)->second;

				return registry.get<GameplayAttributeSet>(target).value[(size_t)capture.attribute].current_value;
			}
			else 
			{
				auto& unique_map = registry.ctx<EntityByUniqueIdMap>();
				entt::entity source = unique_map.value.find(spec.source)->second;
				return registry.get<GameplayAttributeSet>(source).value[(size_t)capture.attribute].current_value;
			}
		}
	}
};

struct RpgCapture
{
	GameplayAttributeCapture defend;
	GameplayAttributeCapture attack;

	RpgCapture() 
		: defend(GameplayAttributeType::Defend, GameplayAttributeCaptureOwner::kTarget, false)
		, attack(GameplayAttributeType::Attack, GameplayAttributeCaptureOwner::kSource, true)
	{
		
	}

	static const RpgCapture& Get()
	{
		static RpgCapture capture;
		return capture;
	}
};

class RpgExecutionCalculation : public GameplayEffectExecutionCalculation
{
public:
	void Capture(entt::registry& registry,GameplayEffectSpec& spec) override
	{
		GameplayEffectExecutionCalculation::Capture(registry, spec, RpgCapture::Get().attack);
		GameplayEffectExecutionCalculation::Capture(registry, spec, RpgCapture::Get().defend);
	}

	void ExecutionCalculation(entt::registry& registry,const GameplayEffectSpec& spec) override
	{
		float defend = Get(registry,spec,RpgCapture::Get().defend);
		float attack = Get(registry,spec,RpgCapture::Get().attack);

		float damage = attack - defend;
		auto e = registry.create();
		registry.emplace<ModifierAttribute>(e, spec.source, spec.target, GameplayAttributeType::Hp, -damage);
	}
private:
};