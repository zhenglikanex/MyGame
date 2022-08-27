local HeroBaseGE = 
{
    period = {
        --period = 3,
        --duration = 12,
        --periodic_inhibition_policy = GEDefine.PeriodicInhibitionPolicy.NeverReset
    },

    application = {
        chance_to_apply_to_target = 100
    },
    --设置的是GE应用状态时的免疫效果
    immunity = {
        granted_application_immunity_tags = {
            -- 免疫带有下列标签的GE
            require_tags = {
                
            },
            -- 免疫没有下列标签的GE
            ignore_tags = {
                
            }
        }
    },
    --当所Apply的Effect的Stack超过了StackLimitCount，便会调用这个模块
    overflow = {    
        overflow_effects = {    --超过StackLimitCount数量的Effect被Apply时将会调用该OverflowEffects
        },
        deny_overflow_application = true, --对应于StackDurationRefreshPolicy，如果为True则多余的Apply不会刷新Duration
        clear_stack_on_overflow = true, --当DenyOverflowApplication为True是才有效，当Overflow时是否直接删除所有层数
    },
    --用于Effect在提前结束还是正常结束时使用
    expiration = {
        --考虑怎么实现，要做些什么
        --PrematureExpirationEffectClasses：Duration还未结束时该Effect便被强制结束，则调用PrematureExpirationEffectClasses
        --RoutineExpirationEffectClasses：Duration正常结束时，调用该RoutineExpirationEffectClasses
    },
    stacking = { -- 处理叠加相关
        stacking_type = GEDefine.StackingType.None,
        stack_limit_count = 1,
        stack_duration_refresh_policy = true, --每次ApplyEffect时是否刷新Effect的持续时间，即使层数不增加也会刷新Duration
        statck_period_reset_policy = true,--每次ApplyEffect时是否刷新周期时间（Period），即使层数不增加也会刷新Period
        statck_expiration_policy =  GEDefine.StatckExpirationPolicy.RemoveSingleStackAndRefreshDuration, --当Effect的Duration结束时,层数的结束方式
    },
    gameplay_effect = {
        duration_policy = GEDefine.DurationPolicy.Instant,
        modifiers = {   --进行一些简单的数值修改

        },
        executions = {  -- 可以进行复杂的修改,可以捕获source和target的属性
            {
                class_name = "RpgExecutionCalculation",
            }
        },
    },
    
    --当该Effect被Apply时，授予目标技能
    granted_abilities = {
        {
        }
    },
    --在应用Effect时的表现形式，如加血时身上冒出绿色的回复数字
    display = {
        cues = {
        }
    },
    tag = {
        gameplay_effect_asset_tags = --GameplayEffect拥有的标签, 它们自身没有任何功能且只用于描述GameplayEffect
        {
            
        },
        granted_tags = { --存于GameplayEffect中且又用于GameplayEffect所应用ASC的标签. 当GameplayEffect移除时它们也会从ASC中移除. 该标签只作用于持续(Duration)和无限(Infinite)GameplayEffect(待考虑是否只作用这两个类型).
            
        },
        ongoing_tag_requirements = { --在GameplayEffect应用期间，持续监测是否满足这些tag,(),提供关闭和重新开启的功能。 该标签只作用于持续(Duration)和无限(Infinite)GameplayEffect.
            require_tags = {

            },
            ignore_tags = {

            }
        },
        application_tag_requirements = { --某个GameplayEffect是否可以应用到该目标的标签, 如果不满足这些需求, 那么GameplayEffect就不可应用.
            require_tags = {

            },
            ignore_tags = {
                --"Test.Ignore"
            }
        },
        remove_gameplay_effects_with_tags = --若施加成功，移除满足条件的所有GE
        {
            
        },
    }
}

return HeroBaseGE