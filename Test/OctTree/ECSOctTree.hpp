#pragma once

#include "entt/entt.hpp"
#include "Primitive.hpp"
#include "Collision.hpp"

struct ListNode
{
	entt::entity prve{ entt::null };
	entt::entity next{ entt::null };

	entt::entity object{ entt::null };
};

struct List
{
	entt::entity head{ entt::null };
	entt::entity tail{ entt::null };
};

struct OctTreeNode 
{
	AABB box;
	entt::entity children[8];
	std::vector<entt::entity> objects;
	OctTreeNode(const AABB& aabb)
		:box(aabb)
	{
		for (int i = 0; i < 8; ++i)
		{
			children[i] = entt::null;
		}
	}
};

void InsertList(entt::registry& registry, entt::entity e,entt::entity object)
{
	if (auto list = registry.try_get<List>(e))
	{
		if (list->tail != entt::null)
		{
			auto& tail = registry.get<ListNode>(list->tail);

			auto next = registry.create();
			tail.next = next;
			list->tail = next;

			registry.emplace<ListNode>(next, list->tail, entt::null, object);
		}
		else
		{
			auto next = registry.create();
			list->head = next;
			list->tail = next;
			registry.emplace<ListNode>(next, entt::null, entt::null, object);
		}
	}

	
}

void BuildTree(entt::registry& registry, entt::entity e,const vec3& c, const vec3& r, uint32_t depth)
{
	if (depth <= 0) return;
	auto& node = registry.emplace_or_replace<OctTreeNode>(e, AABB(c, r));

	if (depth - 1 > 0)
	{
		vec3 offset(fixed16(0));
		for (int i = 0; i < 8; ++i)
		{
			offset.x = (i & 1) ? r.x : -r.x;
			offset.y = (i & 2) ? r.y : -r.y;
			offset.z = (i & 4) ? r.z : -r.z;
			offset *= fixed16(0.5f);


			auto child = registry.create();
			BuildTree(registry, child, c + offset, r * fixed16(0.5f), depth - 1);
			auto& node = registry.get<OctTreeNode>(e);
			node.children[i] = child;
		}
	}
}

void Insert(entt::registry& registry, entt::entity e,entt::entity object)
{
	auto object_box = registry.try_get<AABB>(object);
	if (!object_box) { return; }

	auto& node = registry.get<OctTreeNode>(e);
	uint32_t collision_num = 0;
	uint32_t index = 0;
	for (int i = 0; i < 8; ++i)
	{
		auto child = node.children[i];
		if (child != entt::null)
		{
			auto& child_node = registry.get<OctTreeNode>(child);
			if (TestCollision(child_node.box, *object_box))
			{
				index = i;
				++collision_num;
				if (collision_num > 1)
				{
					break;
				}
			}
		}
	}

	if (collision_num <= 0 || collision_num > 1)
	{
		// 1.没有碰撞,在整个八叉树外
		// 2.碰撞多个子树
		// 3.到达合适的层
		node.objects.push_back(object);
	}
	else
	{
		Insert(registry, node.children[index], object);
	}
}

void GetCollsions(entt::registry& registry, entt::entity tree, const AABB& box, std::vector<entt::entity>& collsions)
{
	auto& node = registry.get<OctTreeNode>(tree);
	if (TestCollision(node.box, box))
	{
		auto view = registry.view<AABB>();
		for (auto& object : node.objects)
		{
			auto& object_box = view.get<AABB>(object);
			if (&object_box != &box)
			{
				if (TestCollision(object_box, box))
				{
					collsions.emplace_back(object);
				}
			}
		}

		for (int i = 0; i < 8; ++i)
		{
			if (node.children[i] != entt::null)
			{
				GetCollsions(registry, node.children[i], box, collsions);
			}
		}
	}
}

void GetCollsions(entt::registry& registry, entt::entity tree, entt::entity e, std::vector<entt::entity>& collsions)
{
	if (!registry.has<OctTreeNode>(tree)) { return; }

	auto box = registry.try_get<AABB>(e);
	if (!box) { return; }

	GetCollsions(registry, tree, *box, collsions);

}