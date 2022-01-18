#pragma once

#include "Framework/Game/Component/HpModify.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

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

	void Add(const GameplayAttributeCapture& capture)
	{
		entt::entity source;
		entt::entity target;
		if (capture.snapshot)
		{
			if (capture.owner == GameplayAttributeCaptureOwner::kTarget)
			{
				target_captures.emplace_back(capture.attribute, registry.get<GameplayAttributeSet>(target).value[(size_t)capture.attribute].current_value);
			}
			else
			{
				source_captures.emplace_back(capture.attribute, registry.get<GameplayAttributeSet>(source).value[(size_t)capture.attribute].current_value);
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

struct RpgExecutionCalculation : public GameplayEffectExecutionCalculation
{
	RpgExecutionCalculation(entt::registry& _registry)
		: GameplayEffectExecutionCalculation(_registry)
	{
		Add(RpgCapture::Get().attack);
		Add(RpgCapture::Get().defend);
	}

	void ExecutionCalculation(entt::registry& registry) override
	{
		entt::entity source = entt::null;
		entt::entity target = entt::null;

		float defend = Get(RpgCapture::Get().defend);
		float attack = Get(RpgCapture::Get().attack);

		// 这里直接减少生命值,并且显示伤害
		float damage = attack - defend;
		registry.patch<ModifierBaseAttributeSet>(target,[damage](ModifierBaseAttributeSet& mod_attr_set)
			{
				mod_attr_set.value[(size_t)GameplayAttributeType::Hp].emplace_back(ModifierAttribute{ GameplayEffect::ModifierOp::Add,-damage });
			});
	}
};