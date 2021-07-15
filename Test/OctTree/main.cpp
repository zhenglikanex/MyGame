//#define CATCH_CONFIG_MAIN
//
//#include <iostream>
//#include <random>
//#include <chrono>
//#include <vector>
//#include "Collision.hpp"
//#include "OctTree.hpp"
//#include "ECSOctTree.hpp"
//
//#include "catch2/catch.hpp"
//
//class Timer
//{
//public:
//	Timer()
//	{
//		start_ = std::chrono::system_clock::now();
//	}
//
//	double Elapsed()
//	{
//		return std::chrono::duration<double>(std::chrono::system_clock::now() - start_).count();
//	}
//private:
//	std::chrono::system_clock::time_point start_;
//};
//
//class AutoTimer
//{
//public:
//	AutoTimer() = default;
//	AutoTimer(const std::string& _name) : name(_name) { }
//	~AutoTimer() {
//		std::cout << name << ":" << timer.Elapsed() * 1000 << " ms" << std::endl;
//	}
//
//	Timer timer;
//	std::string name;
//};
//
//
//struct Test 
//{
//
//};
//
//TEST_CASE_METHOD(Test, "Collision")
//{
//	AABB box{ vec3(fixed16(10.0f)),vec3(fixed16(1.0f)) };
//	AABB box2{ vec3(fixed16(25.0f)),vec3(fixed16(25.0f)) };
//	REQUIRE(TestCollision(box, box2));
//}
//
//TEST_CASE_METHOD(Test, "Sphere")
//{
//	std::vector<Circular> circulars;
//	for (auto i = 0; i < 500; ++i)
//	{
//		circulars.push_back(Circular{ vec2(1.3,1.5),4.5 });
//	}
//	
//	{
//		int cnt = 0;
//		auto a = Circular{ vec2(1.3,1.5),4.5 };
//		AutoTimer t("TestCollision");
//		for (int i = 0; i < 500; ++i)
//		{
//			for (int j = 0; j < 500; ++j)
//			{
//				if (TestCollision(a, a))
//				{
//					++cnt;
//				}
//			}
//		}
//	}
//	
//	{
//		int cnt = 0;
//		AutoTimer t("circulars");
//		for (auto& a : circulars)
//		{
//			for (auto& b : circulars)
//			{
//				if(TestCollision(a, b))
//				{
//					++cnt;
//				}
//			}
//		}
//		std::cout << cnt << std::endl;
//	}
//}
//
//TEST_CASE_METHOD(Test, "OctTree")
//{
//	std::default_random_engine e;
//	e.seed(std::chrono::system_clock::now().time_since_epoch().count());
//	std::uniform_real_distribution<float>c(0, 1000);
//	std::uniform_real_distribution<float>r(0, 10);
//	std::vector<AABB> aabbs;
//	aabbs.reserve(10000);
//	std::vector<Capsule> collisions;
//	OctTree tree(vec3(fixed16(0)), vec3(fixed16(1000)), 5);
//
//	entt::registry registry;
//	auto te = registry.create();
//	BuildTree(registry, te, vec3(fixed16(0)), vec3(fixed16(1000)), 7);
//
//	for (int i = 0; i < 10000; ++i)
//	{
//		AABB obb = { vec3(10),vec3(20) };
//		aabbs.emplace_back(obb);
//	}
//
//	//{
//	//	AutoTimer t;
//	//	for (int i = 0; i < 1000; ++i)
//	//	{
//	//		tree.Insert(aabbs[i]);
//	//	}
//	//}
//
//	//{
//	//	AutoTimer t("ecs tree");
//	//	for (int i = 0; i < 1000; ++i)
//	//	{
//	//		auto e = registry.create();
//	//		registry.emplace<AABB>(e, aabbs[i]);
//	//		Insert(registry, te, e);
//	//	}
//	//}
//
//
//	int cnt1 = 0, cnt2 = 0;
//	{
//		AutoTimer t("obb");
//		for (int i = 0; i < 10000; ++i)
//		{
//			for (int j = 0; j < aabbs.size(); ++j)
//			{
//				if (i != j)
//				{
//					if (TestCollision(aabbs[i], aabbs[j])) ++cnt1;
//				}
//			}
//		}
//	}
//
//	//std::vector<const AABB*> objects;
//	//{
//
//	//	objects.reserve(100000);
//	//	AutoTimer t("oct");
//	//	for (int i = 0; i < 1000; ++i)
//	//	{
//	//		tree.GetCollisionObject(aabbs[i], objects);
//	//		//cnt2 += objects.size();
//	//	}
//	//}
//
//	//std::vector<entt::entity> entites;
//	//{
//	//	objects.reserve(100000);
//	//	AutoTimer t("ecs");
//	//	auto view = registry.view<AABB>();
//	//	for (auto e : view)
//	//	{
//	//		 GetCollsions(registry, te, e, entites);
//	//	}
//	//}
//
//	//REQUIRE(cnt1 == objects.size());
//	//REQUIRE(cnt1 == entites.size());
//}
//
//
//TEST_CASE_METHOD(Test, "entt list")
//{
//	entt::registry registry;
//
//	auto list = registry.create();
//	auto& comp = registry.emplace<List>(list);
//	std::list<int*> stdlist;
//	std::list<ListNode*> list_nodes;
//	std::vector<int*> vec;
//	for (int i = 0; i < 5000; ++i)
//	{
//		auto e = registry.create();
//		registry.emplace<int>(e, i);
//		InsertList(registry, list, e);
//		stdlist.push_back(new int(i));
//		vec.push_back(new int(i));
//	}
//
//	int cnt = 0;
//	{
//		auto view = registry.view<ListNode>();
//		auto int_view = registry.view<int>();
//		AutoTimer t("entt list");
//		for (int i = 0; i < 10; ++i)
//		{
//			auto current = comp.head;
//			while (current != entt::null)
//			{
//				auto& node = view.get(current);
//				//std::cout << (uint32_t)current << " " << registry.get<int>(node.object) << std::endl;
//				cnt += int_view.get<int>(node.object);
//				
//				current = node.next;
//			}
//		}
//	}
//	std::cout << cnt << std::endl;
//	cnt = 0;
//	{
//		AutoTimer t("list");
//		for (int i = 0; i < 10; ++i)
//		{
//			for (auto& o : stdlist)
//			{
//				//REQUIRE(o == cnt);
//				cnt += *o;
//			}
//		}
//	}
//	std::cout << cnt << std::endl;
//}
//
//TEST_CASE_METHOD(Test, "entt otc")
//{
//	entt::registry registry;
//	auto e = registry.create();
//	BuildTree(registry, e, vec3(fixed16(0)), vec3(fixed16(10)), 2);
//
//	auto o = registry.create();
//	registry.emplace<AABB>(o,vec3(fixed16(1)),vec3(fixed16(5)));
//	auto o2 = registry.create();
//	registry.emplace<AABB>(o2, vec3(fixed16(1)), vec3(fixed16(5)));
//
//	Insert(registry, e, o);
//	Insert(registry, e, o2);
//
//
//	system("pause");
//}

int main()
{
	return 0;
}