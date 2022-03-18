#pragma once

#include "Framework/Game/Registry.hpp"
#include "Framework/Game/Math.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Player.hpp"

namespace actor_utiltiy
{
	inline entt::entity CreatePlayer(Registry& reg,uint32_t id, std::string_view actor_asset,const vec3& position = vec3(0,0,0),const quat& rotation = glm::identity<quat>())
	{
		auto e = reg.create_predict(id);
		reg.emplace<ActorAsset>(e, actor_asset); // 通过createactoresystem创建具体actor信息
		reg.emplace<Player>(e,id);
		reg.emplace<Transform>(e, position, rotation);

		return e;
	}

	inline entt::entity CreateMonster(Registry& reg, uint32_t id, const std::string& actore_asset, const vec3& position = vec3(0, 0, 0), const quat& rotation = glm::identity<quat>())
	{
		
	}
};