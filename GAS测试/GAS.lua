local GameplayEffectDefine = require "GameplayEffectDefine"
--#region

function Clone(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs(object) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end

function string.split(str, delimiter)
    str = tostring(str)
    delimiter = tostring(delimiter)
    if (delimiter=='') then return false end
    local pos,arr = 0, {}
    -- for each divider found
    for st,sp in function() return string.find(str, delimiter, pos, true) end do
        table.insert(arr, string.sub(str, pos, st - 1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(str, pos))
    return arr
end

-- 判断tag2是否等于tag1或者被tag1包含
function IsContainTag(tag1,tag2)
    local hierarchys1 = string.split(tag1,".")
    local hierarchys2 = string.split(tag2,".")

    for i = 1,# hierarchys1 do
        if hierarchys1[i] ~= hierarchys2[i] then
            return false
        end
    end

    return true
end

function GetAbilitySystemComponentView()
end

function GetGameplayEffectList()
    return 
    {
        {
            spec = instance,
            stack_count = 1,
            duration = 0,   -- GE已经持续的时间
            period = 0, --  GE周期时间,每个周期归零
        },
        {
            spec = instance,
            stack_count = 1,
            duration = 0,   -- GE已经持续的时间
            period = 0, --  GE周期时间,每个周期归零
        },
    }
     
end

function GetGameplayEffectApplyList()
    return { "name1","name2","name3" }
end

function GetGameplayAbilitySpec()

end
-- 
function GetTagsComponent(e)
    return { self = {} , ge = { {}, } }    
end

function GetImmunityInfo(e)
    -- todo优化免疫的判断方式
    local immunity = { require_tags = {} ,ignore_tags = {} }
    local ge_list = GetGameplayEffectList(e)
    for _,ge in pairs(ge_list) do
        for _, tag in pairs(ge.spec.immunity.granted_application_immunity_tags.require_tags) do
            table.insert(immunity.require_tags,tag)
        end

        for _,tag in pairs(ge.spec.immunity.granted_application_immunity_tags.ignore_tags) do
           table.insert(immunity.ignore_tags,tag)
        end
    end

    return immunity
end

function CheckApplication(e,spec)
    if spec.application.chance_to_apply_to_target == 0 then
        return false
    end

    if spec.application.chance_to_apply_to_target == 1 then
        return true
    end

    -- todo,需要使用自己的随机数库,提供回滚功能
    local num = math.random()
    return spec.application.chance_to_apply_to_target >= num
end

function CheckImmunity(e,spec)
    -- 判断方式需要优化,考虑map查表的方式
    local immunity = GetImmunityInfo(e)

    -- 如果要应用的GE的gameplay_effect_asset_tags字段含有这个
    for _,require_tag in pairs(immunity.require_tags) do
        local found = false
        for _,tag in pairs(spec.tag.gameplay_effect_asset_tags) do
            if IsContainTag(require_tag,tag) then
                found = true
                break
            end
        end

        if not found then
            return false
        end
    end

    -- 如果要应用的GE的gameplay_effect_asset_tags字段不含有ignore_tags中的字段,
    for _,ignore_tag in pairs(immunity.ignore_tags) do
        for _,tag in pairs(spec.tag.gameplay_effect_asset_tags) do
            if IsContainTag(ignore_tag,tag) then
                return false
            end
        end
    end

    return true
end

function CheckApplicationTagRequirements(e,spec)
    local comp = GetTagsComponent(e)
    local tags = {}
    for _,tag in pairs(comp.self) do
        table.insert(tags,tag)
    end

    for _,ge in pairs(comp.ge) do
        for _,tag in pairs(ge) do
            table.insert(tags,tag)
        end
    end

    -- 如果被施加的目标不包含application_tag_requirements中的tag,则不能应用
    for _,require_tag in pairs(spec.tag.application_tag_requirements) do
        local found = false
        for _,tag in pairs(tags) do
            if IsContainTag(require_tag,tag) then
                found = true
                break
            end
        end

        if not found then
            return false
        end
    end

    return true
end

function ApplyGameplayEffect(e,ge)
    local ge = require(string.format("GameplayEffect/%s",ge))

    local spec = ge.New()
    if not CheckApplicationTagRequirements(e,spec) or not CheckImmunity(e,spec) or not CheckApplication(e,spec) then
        return
    end    
end

function DoApplyGameplayEffect(e,spec)
    local applied = true

    local modifiers = GetModifiers(e)
    for _,modifier in pairs(spec.gameplay_effect.modifiers) do
        table.insert(modifiers,modifier)
    end

    local ge_list = GetGameplayEffectList(e)
    local tags = GetTagsComponent(e)
    if spec.gameplay_effect.duration_poilcy == GameplayEffectDefine.DurationPolicy.Duration or
        spec.gameplay_effect.duration_poilcy == GameplayEffectDefine.DurationPolicy.Infinite then

        if spec.stacking.stacking_type == GameplayEffectDefine.StackingType.None then
            table.insert( ge_list, { spec = spec,duration = 0,period = 0,stack_count = 1 } )
        else
            local found = false
            for _,ge in pairs(ge_list) do
                if ge.spec.name == spec.name then
                    if ge.stack_count < spec.stacking.stacking_limit_count then
                        ge.stack_count = ge.stack_count + 1
                    else
                        applied = false

                        --处理overflow相关
                        for _,overflow_effect in pairs(spec.overflow.overflow_effects) do
                            -- todo 注意这里正在遍历ge_apply_list,往里面插数据会有问题,要另外处理
                            local ge_apply_list = GetGameplayEffectApplyList(e)
                            table.insert(ge_apply_list,overflow_effect)
                        end
                    end

                    local reset_duration = spec.stacking.stack_duration_refresh_policy and (spec.overflow.deny_overflow_application ~= true or ge.stack_count <= spec.stacking.stacking_limit_count)
                    local reset_period = spec.stacking.statck_period_reset_policy and (spec.overflow.deny_overflow_application ~= true or ge.stack_count <= spec.stacking.stacking_limit_count)
                    
                    if reset_duration then
                        ge.duration = 0
                    end

                    if reset_period then
                        ge.period = 0
                    end

                    found = true
                end
            end

            if not found then
                table.insert( ge_list, { spec = spec,duration = 0,period = 0,stack_count = 1 } )
            end
        end

        if applied then
            if not tags.ge[spec.name] then
                tags.ge[spec.name] = Clone(spec.tag.granted_tags)
            end

            --todo 添加技能到带添加列表,等待后面的update添加
            --

            
        end
    end
end

function UpdateApplyGE()
    local view = GetAbilitySystemComponentView()
    for _,e in pairs(e) do
        local ge_list = GetGameplayEffectApplyList(e)
        for _,ge in pairs(ge_list) do
            ApplyGameplayEffect(e,ge)
        end
    end
end

function UpdateModifier()
    local view = GetView()
    for _,e in pairs(view) do
        local modifiers = {}
        for _,modifier in pairs(modifiers) do
            
        end
    end
end

function DoMofifier(e,modifier)
    --
end