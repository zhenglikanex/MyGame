function LoadGameplayEffect(name,ge)
    local template = require("GameplayEffect/".. name)
    CopyTable(template.period,ge.period)
    CopyTable(template.application,ge.application)
    for _,tag in pairs(template.immunity.granted_application_immunity_tags.require_tags) do
        ge.immunity.granted_application_immunity_tags.require_tags:Push((tag))
    end
    for _,tag in pairs(template.immunity.granted_application_immunity_tags.ignore_tags) do
        ge.immunity.granted_application_immunity_tags.ignore_tags:Push((tag))
    end

    for _,effect in pairs(ge.overflow.overflow_effects) do
        ge.overflow.overflow_effects:add(effect)
    end

    ge.overflow.deny_overflow_application = template.overflow.deny_overflow_application
    ge.overflow.clear_stack_on_overflow = template.overflow.clear_stack_on_overflow

    CopyTable(template.stacking,ge.stacking)
    
    ge.gameplay_effect.duration_policy = template.gameplay_effect.duration_policy
    for _,modifier in pairs(template.gameplay_effect.modifiers) do
        dump(modifier)
        local mod = GEModifier.new()
        mod.attribute = modifier.attribute
        mod.modifier_op = modifier.modifier_op
        CopyTable(modifier.modifier_magnitude,mod.modifier_magnitude)
        for _,tag in pairs(modifier.source_tags.require_tags) do
            mod.source_tags.require_tags:add((tag))
        end
        for _,tag in pairs(modifier.source_tags.ignore_tags) do
            mod.source_tags.ignore_tags:add((tag))
        end
        for _,tag in pairs(modifier.target_tags.require_tags) do
            mod.target_tags.require_tags:add((tag))
        end
        for _,tag in pairs(modifier.target_tags.ignore_tags) do
            mod.target_tags.ignore_tags:add((tag))
        end
        print("ge.gameplay_effect.modifiers",ge.gameplay_effect.modifiers)
        ge.gameplay_effect.modifiers:add( mod )
    end

    for _,execution in pairs(template.gameplay_effect.executions) do
        print(execution.class_name)
        local exec = GEExecution.new()
        exec.class_name = execution.class_name
        ge.gameplay_effect.executions:add( exec )
    end

    for _,ability in pairs(template.granted_abilities) do
        local ga = GEGrantedAbility.new()
        CopyTable(ability,ga)
        ge.granted_abilities:add(ga)
    end

    for _,tag in pairs(template.tag.gameplay_effect_asset_tags) do
        ge.tag.gameplay_effect_asset_tags:Push((tag))
    end
    for _,tag in pairs(template.tag.granted_tags) do
        ge.tag.granted_tags:Push((tag))
    end
    for _,tag in pairs(template.tag.ongoing_tag_requirements.require_tags) do
        ge.tag.ongoing_tag_requirements.require_tags:Push((tag))
    end
    for _,tag in pairs(template.tag.ongoing_tag_requirements.ignore_tags) do
        ge.tag.ongoing_tag_requirements.ignore_tags:Push((tag))
    end
    for _,tag in pairs(template.tag.application_tag_requirements.require_tags) do
        ge.tag.application_tag_requirements.require_tags:Push((tag))
    end
    for _,tag in pairs(template.tag.application_tag_requirements.ignore_tags) do
        ge.tag.application_tag_requirements.ignore_tags:Push((tag))
    end
    for _,tag in pairs(template.tag.remove_gameplay_effects_with_tags) do
        ge.tag.remove_gameplay_effects_with_tags:Push((tag))
    end
end

function LoadGameplayAbility(name,ga)
    local template = require("GameplayAbility/" .. name)

    ga.cooldown_gameplay_effect = template.cooldown_gameplay_effect
    ga.cost_gameplay_effect = template.cost_gameplay_effect
    
    print(ga.cost_gameplay_effect, template.cost_gameplay_effect)

    for _,tag in pairs(template.ability_tags) do
        ga.ability_tags:Push(tag)
    end

    for _,tag in pairs(template.cancel_ability) do
        ga.cancel_ability:Push(tag)
    end

    for _,tag in pairs(template.block_abilites_with_tag) do
        ga.block_abilites_with_tag:Push(tag)
    end

    for _,tag in pairs(template.activation_owned_tags) do
        ga.activation_owned_tags:Push(tag)
    end

    for _,tag in pairs(template.activation_required_tags) do
        ga.activation_required_tags:Push(tag)
    end

    for _,tag in pairs(template.activation_blocked_tags) do
        ga.activation_blocked_tags:Push(tag)
    end

    for _,tag in pairs(template.source_required_tags) do
        ga.source_required_tags:Push(tag)
    end

    for _,tag in pairs(template.source_blocked_tags) do
        ga.source_blocked_tags:Push(tag)
    end

    for _,tag in pairs(template.target_required_tags) do
        ga.target_required_tags:Push(tag)
    end

    for _,tag in pairs(template.target_blocked_tags) do
        ga.target_blocked_tags:Push(tag)
    end

    ga.cooldown_duration = template.cooldown_duration or 0
    if template.cooldown_tags then
        for _,tag in pairs(template.cooldown_tags) do
            ga.cooldown_tags:Push(tag)
        end
    end

    ga.script = require("GameplayAbility/" .. name.."Script")
end
