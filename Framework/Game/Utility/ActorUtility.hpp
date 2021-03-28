#pragma once

#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Math.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Player.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct ActorUtiltiy
{
	static entt::entity CreatePlayer(entt::registry& reg,uint32_t id, std::string_view actor_asset,const vec3& position = vec3(0,0,0),const quat& rotation = glm::identity<quat>())
	{
		auto e = reg.create();

		reg.emplace<Player>(e,id);
		reg.emplace<Transform>(e, position, rotation);

		// 通过createactoresystem创建具体actor信息
		reg.emplace<ActorAsset>(e,actor_asset);
		
		return e;
	}

	static entt::entity CreateMonster(entt::registry& reg, uint32_t id, const std::string& actore_asset, const vec3& position = vec3(0, 0, 0), const quat& rotation = glm::identity<quat>())
	{

	}
};