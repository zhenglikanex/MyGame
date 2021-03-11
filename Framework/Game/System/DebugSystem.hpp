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

			auto mat = glm::mat4_cast(transform.rotation);
			mat[3] = vec4(transform.position, fixed16(1));

			if (box.type == BoundingBoxType::kAABB)
			{
				g_debug_service->DrawCube(transform.position + box.aabb.c, quat(vec3(0, 0, 0)), box.aabb.r * fixed16(2));
			}
			else if(box.type == BoundingBoxType::kOBB)
			{
				mat4 obb_mat(vec4(box.obb.u[0], 0), vec4(box.obb.u[0], 0), vec4(box.obb.u[0], 0), vec4(0, 0, 0, 1));
				mat *= obb_mat;
				vec3 position(mat[3]);
				quat rotation(mat);

				g_debug_service->DrawCube(position, rotation, box.obb.e * fixed16(2));
			}
			else if (box.type == BoundingBoxType::kCapsule)
			{
				auto a = (mat * glm::translate(glm::identity<mat4>(), box.capsule.a))[3];
				auto b = (mat * glm::translate(glm::identity<mat4>(), box.capsule.b))[3];
				auto position = a + (b - a) * fixed16(0.5);

				g_debug_service->DrawCapsule(position, a, b, box.capsule.r);
			}
			else if (box.type == BoundingBoxType::kSphere)
			{
				g_debug_service->DrawSphere(transform.position + box.sphere.c, box.sphere.r);
			}
		}
	}

	void Finalize() override
	{
	}
};