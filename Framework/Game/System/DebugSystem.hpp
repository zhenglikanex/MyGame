#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/BoundingBox.hpp"
#include "Framework/Game/Component/Transform.hpp"

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
		DrawBoundingBox();
	}

	void DrawBoundingBox()
	{
		auto view = registry.view<Transform,BoundingBox>();
		for (auto e : view)
		{
			const auto& transform = view.get<Transform>(e);
			const auto& box = view.get<BoundingBox>(e);
			if (box.type == BoundingBoxType::kAABB)
			{
				g_debug_service->DrawCube(box.aabb.c, quat(fixed16(1), fixed16(0), fixed16(0), fixed16(0)), box.aabb.r * fixed16(2));
			}
			else if(box.type == BoundingBoxType::kOBB)
			{
				mat3 mat(box.obb.u[0], box.obb.u[1], box.obb.u[2]);
				auto qut = quat(transform.mat);
				auto pos = box.obb.c * glm::inverse(qut);
				g_debug_service->DrawCube(transform.position + pos, quat(transform.mat), box.obb.e * fixed16(2));
			}
			else if (box.type == BoundingBoxType::kCapsule)
			{
				g_debug_service->DrawCapsule(transform.position, quat(fixed16(1), fixed16(0), fixed16(0), fixed16(0)), glm::distance(box.capsule.b, box.capsule.a), box.capsule.r);
			}
			else if (box.type == BoundingBoxType::kSphere)
			{
				g_debug_service->DrawSphere(box.sphere.c, box.sphere.r);
			}
		}
	}

	void Finalize() override
	{
	}
};