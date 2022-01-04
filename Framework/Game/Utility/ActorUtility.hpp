#pragma once

#include <unordered_map>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Math.hpp"
#include "Framework/Game/GameHelper.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/SyncManager.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct ActorUtiltiy
{
	static entt::entity CreatePlayer(entt::registry& reg,uint32_t id, std::string_view actor_asset,const vec3& position = vec3(0,0,0),const quat& rotation = glm::identity<quat>())
	{
		auto e = reg.create();
		reg.emplace<ActorAsset>(e, actor_asset); // 通过createactoresystem创建具体actor信息
		reg.emplace<Local>(e, reg.ctx<SyncManager>().players[id].local_id_generator.value++);
		reg.emplace<Player>(e,id);
		reg.emplace<Transform>(e, position, rotation);
		reg.emplace<TransformState>(e);
		reg.emplace<GameplayAttributeSet>(e);
		reg.emplace<ModifierBaseAttributeSet>(e);
		reg.emplace<ModifierAttributeSet>(e);
		reg.emplace<GameplayState>(e);

		auto& helper = reg.ctx<Locator>().Ref<const GameHelper>();
		if (helper.IsAuthority())
		{
			reg.emplace<UniqueId>(e,e);
		}

		return e;
	}

	static entt::entity CreateMonster(entt::registry& reg, uint32_t id, const std::string& actore_asset, const vec3& position = vec3(0, 0, 0), const quat& rotation = glm::identity<quat>())
	{
		
	}
};