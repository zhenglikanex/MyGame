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

struct GameplayEffectExecutionCalculation
{
	struct CaptureAttribute
	{
		GameplayAttributeType attribute;
		float value;

		CaptureAttribute(GameplayAttributeType _attribute, float _value)
			: attribute(_attribute)
			, value(_value)
		{

		}
	};

	entt::registry& registry;
	std::vector<CaptureAttribute> source_captures;
	std::vector<CaptureAttribute> target_captures;

	GameplayEffectExecutionCalculation(entt::registry& _registry)
		: registry(_registry)
	{

	}

	virtual ~GameplayEffectExecutionCalculation() = 0
	{

	}

	virtual void ExecutionCalculation(entt::registry&) = 0;

	static void Capture(entt::registry& registry,const GameplayAttributeCapture& capture,GameplayEffectSpec& spec)
	{
		auto& unique_map = registry.ctx<EntityByUniqueIdMap>();

		entt::entity source = unique_map.value.find(spec.source)->second;
		entt::entity target = unique_map.value.find(spec.target)->second;

		if (capture.snapshot)
		{
			if (capture.owner == GameplayAttributeCaptureOwner::kTarget)
			{
				spec.target_snapshot_attrs.emplace(capture.attribute, registry.get<GameplayAttributeSet>(target).value[(size_t)capture.attribute].current_value);
			}
			else
			{
				spec.source_snapshot_attrs.emplace(capture.attribute, registry.get<GameplayAttributeSet>(source).value[(size_t)capture.attribute].current_value);
			}
		}
	}

	float Get(const GameplayAttributeCapture& capture_def)
	{
		entt::entity source;
		entt::entity target;
		if (capture_def.snapshot)
		{
			if (capture_def.owner == GameplayAttributeCaptureOwner::kTarget)
			{
				for (auto& capture : target_captures)
				{
					if (capture_def.attribute == capture.attribute)
					{
						return capture.value;
					}
				}
			}
			else
			{
				for (auto& capture : source_captures)
				{
					if (capture_def.attribute == capture.attribute)
					{
						return capture.value;
					}
				}
			}
		}
		else
		{
			if (capture_def.owner == GameplayAttributeCaptureOwner::kTarget)
			{
				return registry.get<GameplayAttributeSet>(target).value[(size_t)capture_def.attribute].current_value;
			}
			else 
			{
				return registry.get<GameplayAttributeSet>(source).value[(size_t)capture_def.attribute].current_value;
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
	static void Capture(entt::registry& registry,GameplayEffectSpec& spec)
	{
		RpgCapture::Get().attack;
	}

	RpgExecutionCalculation(entt::registry& _registry,)
		: GameplayEffectExecutionCalculation(_registry)
	{
		
	}

	void ExecutionCalculation(entt::registry& registry) override
	{
		entt::entity source = entt::null;
		entt::entity target = entt::null;

		float defend = Get(RpgCapture::Get().defend);
		float attack = Get(RpgCapture::Get().attack);

		float damage = attack - defend;
		auto e = registry.create();
		registry.emplace<ModifierAttribute>(e, target, GameplayAttributeType::Hp, GameplayEffect::ModifierOp::Add, -damage);
	}
private:
};