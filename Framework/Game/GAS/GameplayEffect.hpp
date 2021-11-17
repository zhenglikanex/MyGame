#pragma once

#include "Framework/Game/GAS/GameplayTag.hpp"

struct GameplayEffect
{	
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
		struct GrantedApplicationImmunityTags
		{
			std::vector<GameplayTag> require_tags;
			std::vector<GameplayTag> ignore_tags;
		};

		GrantedApplicationImmunityTags granted_application_immunity_tags;
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

	struct Stacking
	{
		
	};
};