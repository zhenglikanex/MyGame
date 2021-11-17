#pragma once

#include "Framework/Game/GAS/GameplayTag.hpp"

struct GameplayEffect
{	
	struct RequireAndIgnoreTags
	{
		std::vector<GameplayTag> require_tags;
		std::vector<GameplayTag> ignore_tags;
	};

	enum class PeriodicInhibitionPolicy : uint8_t
	{
		NeverReset = 1,                     //是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从被阻止周期继续进行
		ResetPeriod = 2,                    //意思是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从0开始计算周期，但在0时刻不计算Attribute
		ExecuteAndResetPeriod = 3,          //意思是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从0开始计算周期，且在0时刻计算Attribute
	};

	struct Period
	{
		float period = 0;
		float duration = 0;
		PeriodicInhibitionPolicy periodic_inhibition_policy = PeriodicInhibitionPolicy::NeverReset;
	};

	Period period;

	struct Appliction
	{
		float chance_to_apply_to_target = 0;	 //--(0 - 1)范围0~1，0不会影响，1一定影响
		//ApplicationRequirements：可自定义较复杂的概率计算，类似于Executions与Modifiers
		//std::function<void(target,source)>
	};

	Appliction appliction;

	struct Immunity
	{
		RequireAndIgnoreTags granted_application_immunity_tags;
	};
	
	Immunity immunity;

	struct Overflow
	{
		std::vector<std::string> overflow_effects;
		bool deny_overflow_application = true;
		bool clear_stack_on_overflow = true;
	};

	Overflow overflow;

	/*
	--用于Effect在提前结束还是正常结束时使用
		expiration = {
			--考虑怎么实现，要做些什么
			--PrematureExpirationEffectClasses：Duration还未结束时该Effect便被强制结束，则调用PrematureExpirationEffectClasses
			--RoutineExpirationEffectClasses：Duration正常结束时，调用该RoutineExpirationEffectClasses
	},
	*/

	enum class StackingType : uint8_t 
	{
		None = 1,				//不会叠加，如果多次释放则每个Effect相当于单个Effect
		AggregateBySource = 2,	//在自身叠加
		AggregateByTarget = 3,	//在目标叠加
	};

	enum class StatckExpirationPolicy : uint8_t
	{
		ClearEntireStack = 1,                       //Duration结束时清除所有层数
		RemoveSingleStackAndRefreshDuration = 2,    //Duration结束时减少一层，然后重新经历一个Duration，一直持续到层数减为0
		// 调用函数处理
	};

	struct Stacking
	{
		StackingType stacking_type = StackingType::None;
		uint8_t statck_limit_count = 0;
		bool stack_duration_refresh_policy = true;
		bool statck_period_reset_policy = true;
		StatckExpirationPolicy statck_expiration_policy = StatckExpirationPolicy::ClearEntireStack;
	};

	Stacking stacking;

	enum class DurationPolicy : uint8_t
	{
		Instant = 1,    //(即刻)对Attribute中BaseValue立即进行的永久性修改.其不会应用GameplayTag, 哪怕是一帧.
		Duration = 2,   //(持续)对Attribute中CurrentValue的临时修改和当GameplayEffect过期或手动移除时, 应用将要被移除的GameplayTag.持续时间在GE中配置
		Infinite = 3,	//(无限)对Attribute中CurrentValue的临时修改和当GameplayEffect移除时, 应用将要被移除的GameplayTag.该类型自身永不过期且必须由某个Ability或ASC手动移除.
	};


	enum class ModifierOp : uint8_t
	{
		Add = 1,		//将Modifier指定的Attribute加上计算结果.使用负数以实现减法操作.
		Multiply = 2,	//将Modifier指定的Attribute乘以计算结果.
		Divide = 3,		//将Modifier指定的Attribute除以计算结果.
		Override = 4,	//使用计算结果覆盖Modifier指定的Attribute.
	};

	enum class ModifierMagnitudeCalculationType : uint8_t
	{
		ScalableFloat = 1,
		AttributeBased = 2,
		CustomCalculationClass = 3, //自定义计算方式
	};

	struct ModifierMagnitude
	{
		ModifierMagnitudeCalculationType calculation_type = ModifierMagnitudeCalculationType::ScalableFloat;
		float value = 0;
	};

	struct Modifier
	{
		//todo
		std::string attribute;
		ModifierOp modifier_op = ModifierOp::Add;
		
		RequireAndIgnoreTags source_tags;
		RequireAndIgnoreTags target_tags;
	};

	struct Execution
	{
		//std::function<void()> func;
	};

	struct GameplayEffectModifier
	{
		DurationPolicy duration_policy = DurationPolicy::Instant;
		std::vector<Modifier> modifiers;
		std::vector<Execution> executions;
	};

	GameplayEffectModifier gameplay_effect;

	enum class AbilityRemovalPolicy : uint8_t
	{
		CancelAbilityImmediately = 1,       //当Effect结束时，所绑定的Ability也会解绑，并且当绑定的Ability结束时会自动调用EndAbility
		RemoveAbilityOnEnd = 2,				//当Effect结束时，所绑定的Ability也会解绑，当绑定的Ability结束时不会自动调用EndAbility
		DoNothing = 3,
	};

	struct GrantedAbility
	{
		std::string ability;
		uint32_t level;
		AbilityRemovalPolicy removal_policy = AbilityRemovalPolicy::DoNothing;
	};

	struct Display
	{
		std::vector<std::string> cues;
	};

	Display display;

	struct Tag
	{
		std::vector<GameplayTag> gameplay_effect_asset_tags;
	};

	Tag tag;
};