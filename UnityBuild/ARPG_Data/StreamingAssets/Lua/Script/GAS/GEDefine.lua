GEDefine = {}

GEDefine.DurationPolicy = 
{
    Instant = 1,    --(即刻)对Attribute中BaseValue立即进行的永久性修改. 其不会应用GameplayTag, 哪怕是一帧.
    Duration = 2,   --(持续)对Attribute中CurrentValue的临时修改和当GameplayEffect过期或手动移除时, 应用将要被移除的GameplayTag.持续时间在GE中配置
    Infinite = 3,   --(无限)对Attribute中CurrentValue的临时修改和当GameplayEffect移除时, 应用将要被移除的GameplayTag. 该类型自身永不过期且必须由某个Ability或ASC手动移除.
}

GEDefine.ModifierOp = 
{
    Add = 1,        --将Modifier指定的Attribute加上计算结果. 使用负数以实现减法操作.
    Multiply = 2,   --将Modifier指定的Attribute乘以计算结果.
    Divide = 3,     --将Modifier指定的Attribute除以计算结果.
    Override = 4,   --使用计算结果覆盖Modifier指定的Attribute.
}

GEDefine.ModifierMagnitudeCalculationType = 
{
    ScalableFloat = 1,
    AttributeBased = 2,
    CustomCalculationClass = 3, --自定义计算方式
}

GEDefine.StackingType = --GE的叠加方式
{
    None = 1,               --不会叠加，如果多次释放则每个Effect相当于单个Effect
    AggregateBySource = 2,  --在自身叠加
    AggregateByTarget = 3,  --在目标叠加
}

GEDefine.StatckExpirationPolicy = 
{
    ClearEntireStack = 1,                       --Duration结束时清除所有层数
    RemoveSingleStackAndRefreshDuration = 2,    --Duration结束时减少一层，然后重新经历一个Duration，一直持续到层数减为0
    -- 调用函数处理
}

GEDefine.AbilityRemovalPolicy = 
{
    CancelAbilityImmediately = 1,       --当Effect结束时，所绑定的Ability也会解绑，并且当绑定的Ability结束时会自动调用EndAbility
    --RemoveAbilityOnEnd = 2,           --当Effect结束时，所绑定的Ability也会解绑，当绑定的Ability结束时不会自动调用EndAbility
    DoNothing = 3,
}

GEDefine.PeriodicInhibitionPolicy = 
{
    NeverReset = 1,                     --是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从被阻止周期继续进行
    ResetPeriod = 2,                    --意思是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从0开始计算周期，但在0时刻不计算Attribute
    ExecuteAndResetPeriod = 3,          --意思是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从0开始计算周期，且在0时刻计算Attribute
}