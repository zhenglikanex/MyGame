#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/Matrix4x4.hpp"

#include "Framework/Game/System.hpp"

struct DebugSystem : public System
{
	DebugSystem(entt::registry& _registry) : System(_registry) { }
	~DebugSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		DrawCollider();
	}

	void DrawCollider()
	{
		auto view = registry.view<Matrix4x4,Collider>();
		for (auto e : view)
		{
			const auto& transform = view.get<Matrix4x4>(e);
			const auto& collider = view.get<Collider>(e);

			if (collider.geometry.type() == GeometryType::kBox)
			{
				quat q = glm::quat_cast(transform.value);
				g_debug_service->DrawCube(transform.value[3], q, collider.geometry.box().e * fixed16(2.0f));
			}
			else if(collider.geometry.type() == GeometryType::kCapsule)
			{
				
			}
			else if (collider.geometry.type() == GeometryType::kSphere)
			{
				g_debug_service->DrawSphere(transform.value[3], collider.geometry.sphere().r);
			}
		}
	}

	void Finalize() override
	{
	}
};