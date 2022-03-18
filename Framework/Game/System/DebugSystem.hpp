#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Collider.hpp"

#include "Framework/Game/System.hpp"

struct DebugSystem : public System
{
	DebugSystem(Registry& _registry) : System(_registry) { }
	~DebugSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(float dt) override
	{
		DrawCollider();
	}

	void DrawCollider()
	{
		auto view = registry.view<Transform,Collider,PhysicsActor>();
		for (auto e : view)
		{
			auto& [transform,collider] = view.get<Transform,Collider>(e);

			auto m = GetMatrix4x4(transform) * collider.transform;
			if (collider.geometry.type() == GeometryType::kBox)
			{
				quat q = glm::quat_cast(m);
				g_debug_service->DrawCube(m[3], q, collider.geometry.box().e * 2.0f);
			}
			else if(collider.geometry.type() == GeometryType::kCapsule)
			{
				quat q = glm::quat_cast(m);
				g_debug_service->DrawCapsule(m[3], q, collider.geometry.capsule().h, collider.geometry.capsule().r);
			}
			else if (collider.geometry.type() == GeometryType::kSphere)
			{
				g_debug_service->DrawSphere(m[3], collider.geometry.sphere().r);
			}
		}
	}

	void Finalize() override
	{
	}
};