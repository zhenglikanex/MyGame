#pragma once

#include "Framework/Game/Collision.hpp"

#include "Framework/Game/Component/Contact.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/Matrix4x4.hpp"

#include "Framework/Game/System.hpp"

struct CollisionSystem : public System
{
	entt::observer mover{ registry, entt::collector.group<Matrix4x4,Collider>().update<Matrix4x4>() };

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

	void CreateContactList()
	{
		auto view = registry.view<ColliderInfo>(entt::exclude<ContactList>);
		for (auto e : view)
		{
			registry.emplace<ContactList>(e);
		}
	}

	void UpdateCollision(fixed16 dt)
	{
		//todo:后面用八叉树替换,暂时不管重复碰撞
		auto view = registry.view<Matrix4x4,Collider>();

		for (auto it1 = mover.begin(); it1 != mover.end(); ++it1)
		{
			auto e1 = *it1;
			const auto& transform1 = view.get<Matrix4x4>(e1);
			const auto& collider1 = view.get<Collider>(e1);
			
			ContactList* contact_list = registry.valid(collider1.owner) ? registry.try_get<ContactList>(collider1.owner) : nullptr;

			for (auto it2 = view.begin(); it2 != view.end(); ++it2)
			{
				auto e2 = *it2;
				const auto& transform2 = view.get<Matrix4x4>(e1);
				const auto& collider2 = view.get<Collider>(e2);

				auto type1 = (size_t)collider1.geometry.type();
				auto type2 = (size_t)collider2.geometry.type();
				auto func = g_GeometryTestFuncTable[type1][type2] != 0 ? g_GeometryTestFuncTable[type1][type2] : g_GeometryTestFuncTable[type2][type1];

				if (e1 != e2 && func(collider1.geometry,collider2.geometry,transform1.value,transform2.value))
				{
					contact_list->value.emplace_back(Contact{ e2 });
				}
			}
		}
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

	void Finalize() override
	{

	}
};
