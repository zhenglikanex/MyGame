#pragma once

#include "Framework/Game/Collision.hpp"

#include "Framework/Game/Component/BoundingBox.hpp"
#include "Framework/Game/Component/Contact.hpp"

#include "Framework/Game/System.hpp"

struct CollisionSystem : public System
{
	CollisionSystem(entt::registry& _registry) : System(_registry) { }
	~CollisionSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		CreateContactList();
		UpdateCollision(dt);
	}

	void LateUpdate(fixed16 dt)
	{
		auto view = registry.view<ContactList>();
		for (auto e : view)
		{
			auto& contact_list = view.get<ContactList>(e);
			contact_list.value.clear();
		}
	}

	void CreateContactList()
	{
		auto view = registry.view<BoundingBox>(entt::exclude<ContactList>);
		for (auto e : view)
		{
			registry.emplace<ContactList>(e);
		}
	}

	void UpdateCollision(fixed16 dt)
	{
		//todo:后面用八叉树替换,暂时不管重复碰撞
		auto view = registry.view<BoundingBox,ContactList>();
		
		for (auto it1 = view.begin(); it1 != view.end(); ++it1)
		{
			auto e1 = *it1;
			const auto& box1 = view.get<BoundingBox>(e1);
			auto& contact_list = view.get<ContactList>(e1);
			for (auto it2 = view.begin(); it2 != view.end(); ++it2)
			{
				auto e2 = *it2;
				const auto& box2 = view.get<BoundingBox>(e2);
				if (e1 != e2 && TestCollision(box1, box2))
				{
					contact_list.value.emplace_back(Contact{ e2 });
				}
			}
		}
	}
	
	int TestCollision(const BoundingBox& box1, const BoundingBox& box2)
	{
		if (box1.type == BoundingBoxType::kAABB)
		{
			return TestCollision(box1.aabb, box2);
		}
		else if (box1.type == BoundingBoxType::kOBB)
		{
			return TestCollision(box1.obb, box2);
		}
		else if (box1.type == BoundingBoxType::kCapsule)
		{
			return TestCollision(box1.capsule, box2);
		}
		else if (box1.type == BoundingBoxType::kSphere)
		{
			return TestCollision(box1.sphere, box2);
		}

		return 0;
	}

	template<class T>
	int TestCollision(const T& primitive,const BoundingBox& box)
	{
		/*if (box.type == BoundingBoxType::kAABB)
		{
			return ::TestCollision(primitive, box.aabb);
		}
		else if (box.type == BoundingBoxType::kOBB)
		{
			return ::TestCollision(primitive, box.obb);
		}
		else if (box.type == BoundingBoxType::kCapsule)
		{
			return ::TestCollision(primitive, box.capsule);
		}
		else if (box.type == BoundingBoxType::kSphere)
		{
			return ::TestCollision(primitive, box.sphere);
		}*/

		return 0;
	}

	void Finalize() override
	{

	}
};
