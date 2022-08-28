#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/Service/DebugService.hpp"

#include "Framework/Game/Component/Collider.hpp"

#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/GAS/GameplayHitTestTask.hpp"

struct DebugSystem : public System
{
	explicit DebugSystem(Registry& _registry) : System(_registry) { }

	bool Initialize() override
	{
		return true;
	}

	void DrawHitCollider()
	{
		auto view = registry.view<GameplayHitTestTask>();
		for (auto e : view)
		{
			auto& task = registry.get<GameplayHitTestTask>(e);
			for (auto& gameplay_hit_test_spec : task.value)
			{
				auto& geometry = gameplay_hit_test_spec.geometry;
				auto m = GetMatrix4x4(gameplay_hit_test_spec.transform);
				if (geometry.type() == GeometryType::kBox)
				{
					quat q = glm::quat_cast(m);
					g_debug_service->DrawCube(m[3], q, geometry.box().e * 2.0f);
				}
				else if (geometry.type() == GeometryType::kCapsule)
				{
					quat q = glm::quat_cast(m);
					g_debug_service->DrawCapsule(m[3], q, geometry.capsule().h, geometry.capsule().r);
				}
				else if (geometry.type() == GeometryType::kSphere)
				{
					g_debug_service->DrawSphere(m[3], geometry.sphere().r);
				}
			}
		}
	}

	void FixedUpdate(float dt) override
	{
		DrawCollider();
		DrawHitCollider();
	}

	void DrawCollider()
	{
		g_debug_service->ClearDraw();
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
