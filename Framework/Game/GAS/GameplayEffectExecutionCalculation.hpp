#pragma once

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
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
	
	virtual void Capture(Registry& registry, GameplayEffectSpec& spec) = 0;
	virtual void ExecutionCalculation(Registry&,const GameplayEffectSpec& spec) = 0;

protected:
	void Capture(Registry& registry,GameplayEffectSpec& spec,const GameplayAttributeCapture& capture)
	{
		if (capture.snapshot)
		{
			if (capture.owner == GameplayAttributeCaptureOwner::kTarget)
			{
				entt::entity target = registry.get(spec.target);
				spec.target_snapshot_attrs.emplace(capture.attribute, registry.get<GameplayAttributeSet>(target).value[(size_t)capture.attribute].current_value);
			}
			else
			{
				entt::entity source = registry.get(spec.source);
				spec.source_snapshot_attrs.emplace(capture.attribute, registry.get<GameplayAttributeSet>(source).value[(size_t)capture.attribute].current_value);
			}
		}
	}

	float Get(Registry& registry,const GameplayEffectSpec& spec,const GameplayAttributeCapture& capture)
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
				entt::entity target = registry.get(spec.target);
				return registry.get<GameplayAttributeSet>(target).value[(size_t)capture.attribute].current_value;
			}
			else 
			{
				entt::entity source = registry.get(spec.source);
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
	void Capture(Registry& registry,GameplayEffectSpec& spec) override
	{
		GameplayEffectExecutionCalculation::Capture(registry, spec, RpgCapture::Get().attack);
		GameplayEffectExecutionCalculation::Capture(registry, spec, RpgCapture::Get().defend);
	}

	void ExecutionCalculation(Registry& registry,const GameplayEffectSpec& spec) override
	{
		float defend = Get(registry,spec,RpgCapture::Get().defend);
		float attack = Get(registry,spec,RpgCapture::Get().attack);

		float damage = attack - defend;
		auto e = registry.create();
		registry.emplace<ModifierAttribute>(e, spec.source, spec.target, GameplayAttributeType::Hp, -damage);
	}
private:
};