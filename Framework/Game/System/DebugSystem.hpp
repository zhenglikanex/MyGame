#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/ColliderInfo.hpp"

#include "Framework/Game/System.hpp"

struct DebugSystem : public System
{
	DebugSystem(entt::registry& _registry) : System(_registry) { }
	~DebugSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		DrawCollider();
	}

	void DrawCollider()
	{
		auto view = registry.view<Transform,Collider,PhysicsActor>();
		for (auto e : view)
		{
			auto& [transform,collider_info] = view.get<Transform,Collider>(e);

			auto m = GetMatrix4x4(transform) * collider_info.transform;
			if (collider_info.geometry.type() == GeometryType::kBox)
			{
				quat q = glm::quat_cast(m);
				g_debug_service->DrawCube(m[3], q, collider_info.geometry.box().e * 2.0f);
			}
			else if(collider_info.geometry.type() == GeometryType::kCapsule)
			{
				quat q = glm::quat_cast(m);
				g_debug_service->DrawCapsule(m[3], q, collider_info.geometry.capsule().h, collider_info.geometry.capsule().r);
			}
			else if (collider_info.geometry.type() == GeometryType::kSphere)
			{
				g_debug_service->DrawSphere(m[3], collider_info.geometry.sphere().r);
			}
		}
	}

	void Finalize() override
	{
	}
};