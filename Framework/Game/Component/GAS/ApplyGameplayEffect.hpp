#pragma once

#include <vector>
#include <string>

#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

#include "3rdparty/include/entt/entt.hpp"

//得保证在当前在处理完
//不然需要在同步这个数据
//因为在回滚的时候这个需要被恢复
//如果需要回滚这个数据的话key应该使用Local
struct ApplyGameplayEffect
{
	std::unordered_map<entt::entity, std::vector<GameplayEffectSpec>> value;
};
