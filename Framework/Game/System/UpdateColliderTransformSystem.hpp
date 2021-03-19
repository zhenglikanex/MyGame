#pragma once

#include "Framework/Game/Collision.hpp"

#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Matrix4x4.hpp"
#include "Framework/Game/Component/Collider.hpp"

#include "Framework/Game/System.hpp"

struct UpdateColliderTransformSystem : public System
{
	entt::observer mover{ registry, entt::collector.group<Transform,ColliderInfo>().update<Transform>() };

	UpdateColliderTransformSystem(entt::registry& _registry) : System(_registry) { }
	~UpdateColliderTransformSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto view = registry.view<const Transform, const ColliderInfo>();
		for (auto e : mover)
		{
			auto[transform, collider_info] = view.get<const Transform, const ColliderInfo>(e);

			if (registry.valid(collider_info.collider) && registry.has<Collider>(collider_info.collider))
			{
				mat4 mat = glm::mat4_cast(transform.rotation);
				mat[3] = vec4(transform.position,1);
				registry.emplace_or_replace<Matrix4x4>(collider_info.collider, mat * collider_info.transform);
			}
		}
	}

	void LateUpdate(fixed16 dt)
	{
	}

	void Finalize() override
	{

	}
};
