#pragma once

#include "Framework/Game/Collision.hpp"

#include "Framework/Game/Component/Contact.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/Matrix4x4.hpp"

#include "Framework/Game/System.hpp"

struct CollisionSystem : public ObserverSystem
{
	entt::observer* mover;

	CollisionSystem(entt::registry& _registry)
		: ObserverSystem(_registry)
		, mover(nullptr)
	{

	}

	~CollisionSystem() {}

	bool Initialize() override
	{
		mover = CreateObserver(entt::collector.group<Matrix4x4, Collider>().update<Matrix4x4>().where<Collider>());
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		CreateContactList();
		ClearContactList();
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

	void ClearContactList()
	{
		auto view = registry.view<ContactList>();
		for (auto e : view)
		{
			auto& contact_list = view.get<ContactList>(e);
			contact_list.value.clear();
		}
	}

	void UpdateCollision(fixed16 dt)
	{
		//todo:后面用八叉树替换,暂时不管重复碰撞
		auto view = registry.view<Matrix4x4,Collider>();

		for (auto it1 = mover->begin(); it1 != mover->end(); ++it1)
		{
			auto e1 = *it1;
			const auto& transform1 = view.get<Matrix4x4>(e1);
			const auto& collider1 = view.get<Collider>(e1);
			
			ContactList* contact_list = registry.valid(collider1.owner) ? registry.try_get<ContactList>(collider1.owner) : nullptr;

			for (auto it2 = view.begin(); it2 != view.end(); ++it2)
			{
				auto e2 = *it2;
				const auto& transform2 = view.get<Matrix4x4>(e2);
				const auto& collider2 = view.get<Collider>(e2);

				auto type1 = (size_t)collider1.geometry.type();
				auto type2 = (size_t)collider2.geometry.type();
				auto func = g_TestGeometryFuncTable[type1][type2] != nullptr ? g_TestGeometryFuncTable[type1][type2] : g_TestGeometryFuncTable[type2][type1];
				
				if (e1 != e2 && func && func(collider1.geometry,collider2.geometry,transform1.value,transform2.value))
				{
					INFO("collision");

					if (contact_list)
					{
						contact_list->value.emplace_back(Contact{ collider2.owner });
					}
				}
			}
		}
	}
	

	void Finalize() override
	{

	}

	void Connect() override
	{
		mover->connect(registry,entt::collector.group<Matrix4x4, Collider>().update<Matrix4x4>().where<Collider>());
	}

	void Disconnect() override
	{
		mover->disconnect();
	}
};
