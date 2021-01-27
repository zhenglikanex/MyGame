#pragma once

#include <vector>

#include "Primitive.hpp"
#include "Collision.hpp"

#include "Framework/Game/Math.hpp"

//节点序号
/// <summary>
/// 2,3
/// 6,7
/// ---------
/// 0,1
/// 4,5
/// </summary>
/// 
/// 到最大节点停下
/// 最多多少节点停下
/// 划分到每个节点只有一个
/// 
/*
class OctTree
{
private:
	struct Node
	{
		vec3 center;
		fixed16 half_width;
		Node* child[8];
		std::vector<const Object*> objects;

		Node(vec3 _center, fixed16 _half_width)
			:center(_center)
			, half_width(_half_width)
		{

		}
	};
public:
	OctTree(vec3 ceneter, fixed16 half_width, int stop_depth)
		:root(nullptr)
	{
		root = Build(ceneter, half_width, stop_depth);
	}

	void Insert(const Object& object)
	{
		for (int i = 0; i < 8; ++i)
		{
			
		}
	}
private:	
	Node* Build(vec3 center, fixed16 half_width, int stop_depth)
	{
		if (stop_depth < 0) { return nullptr; }

		Node* node = new Node(center,half_width);
		
		vec3 offset(fixed16(0));
		fixed16 step = half_width * fixed16(0.5f);
		for (int i = 0; i < 8; i++)
		{
			offset.x = ((i & 1) ? step : -step);	// 1、3、5、7或0,4,6,2
			offset.y = ((i & 2) ? step : -step);	// 2,3,6,7或0,1,4,5 
			offset.z = ((i & 4) ? step : -step);	// 4,5,6,7或0,1,2,3
			node->child[i] = Build(center + offset, step, stop_depth - 1);
		}
		return node;
	}

	//void _Inster(Node* tree, const Object& object)
	//{
	//	bool flag = false;
	//	for (int i = 0; i < 8; ++i)
	//	{
	//		if (tree->child[i])
	//		{
	//			// 判断是否在这个子树的范围
	//			if (fpm::abs(tree->center[0] - object.ceneter[0]) < object.radius + tree->half_width
	//				&& fpm::abs(tree->center[1] - object.ceneter[1]) < object.radius + tree->half_width
	//				&& fpm::abs(tree->center[2] - object.ceneter[2]) < object.radius + tree->half_width)
	//			{
	//				flag = true;
	//				_Inster(tree->child[i], object);
	//			}
	//		}
	//	}

	//	if (!flag)
	//	{
	//		tree->objects.push_back(&object);
	//	}
	//}

	void InsertObject(Node* tree, const Object* object)
	{
		int index = 0, straddle = 0;
		for (int i = 0; i < 3; i++)
		{
			fixed16 delta = object->ceneter[i] - tree->center[i];
			if (fpm::abs(delta) < tree->half_width + object->radius)
			{
				// 发生碰撞
				straddle = 1;
				break;
			}

			if (delta > fixed16(0.0f)) index |= (1 << i); //ZYX
		}
	}

	Node* root;
};
*/

struct OctTreeObject
{
	AABB box;
};

class OctTree
{
private:
	struct Node
	{
		AABB box;
		Node* children[8];
		std::vector<const AABB*> objects;
		Node(const AABB& _box)
			:box(_box)
		{ 
			memset(children, 0, sizeof(children));
		}
	};
public:
	OctTree(const vec3& c,const vec3& r, uint32_t max_depth)
		:tree_(nullptr)
		, max_depth_(max_depth)
	{
		tree_ = BuildTree(c, r,max_depth);
	}

	void Insert(const AABB& aabb)
	{
		Insert(tree_, aabb);
	}

	std::vector<const AABB*> GetCollisionObject(const AABB& aabb)
	{
		std::vector<const AABB*> objects;
		GetCollisionObject(tree_, aabb, objects);
		return objects;
	}

	void GetCollisionObject(const AABB& aabb, std::vector<const AABB*>& objects)
	{
		GetCollisionObject(tree_, aabb, objects);
	}

	uint32_t GetObjectCount()
	{
		uint32_t count = 0;
		GetObjectCount(tree_, count);
		return count;
	}
private:
	Node* BuildTree(const vec3& c, const vec3& r, uint32_t depth)
	{
		if (depth <= 0) return nullptr;

		Node* node = new Node(AABB(c,r));


		vec3 offset(fixed16(0));
		for (int i = 0; i < 8; ++i)
		{
			offset.x = (i & 1) ? r.x : -r.x;
			offset.y = (i & 2) ? r.y : -r.y;
			offset.z = (i & 4) ? r.z : -r.z;
			offset *= fixed16(0.5f);

			node->children[i] = BuildTree(c + offset, r * fixed16(0.5f), depth - 1);
		}
		
		return node;
	}
	void Insert(Node* tree, const AABB& aabb) 
	{
		uint32_t collision_num = 0;
		uint32_t index = 0;
		for (int i = 0; i < 8; ++i)
		{
			Node* child = tree->children[i];
			if (child && TestCollision(child->box, aabb))
			{
				index = i;
				++collision_num;
				if (collision_num >1)
				{
					break;
				}
			}
		}

		if (collision_num <= 0 || collision_num > 1)
		{
			// 1.没有碰撞,在整个八叉树外
			// 2.碰撞多个子树
			// 3.到达合适的层
			tree->objects.push_back(&aabb);
		}
		else
		{
			Insert(tree->children[index], aabb);
		}
	}

	void GetCollisionObject(const Node* tree,const AABB& aabb, std::vector<const AABB*>& objects)
	{
		if (TestCollision(tree->box, aabb))
		{
			for (auto object : tree->objects)
			{
				if (&aabb != object && TestCollision(aabb, *object))
				{
					objects.push_back(object);
				}
			}

			for (int i = 0; i < 8; ++i)
			{
				if (tree->children[i])
				{
					GetCollisionObject(tree->children[i], aabb, objects);
				}
			}
		}
	}

	void GetObjectCount(Node* tree, uint32_t& count)
	{
		count += tree->objects.size();
		for (int i = 0; i < 8; ++i)
		{
			if (tree->children[i])
			{
				GetObjectCount(tree->children[i],count);
			}
		}
	}
public:
	Node* tree_;
	uint32_t max_depth_;
};

