#define CATCH_CONFIG_MAIN

#include <random>
#include <chrono>
#include "Collision.hpp"
#include "OctTree.hpp"

#include "catch2/catch.hpp"

class Timer
{
public:
	Timer()
	{
		start_ = std::chrono::system_clock::now();
	}

	double Elapsed()
	{
		return std::chrono::duration<double>(std::chrono::system_clock::now() - start_).count();
	}
private:
	std::chrono::system_clock::time_point start_;
};

class AutoTimer
{
public:
	~AutoTimer() {
		std::cout << timer.Elapsed() * 1000 << " ms" << std::endl;
	}

	Timer timer;
};


struct Test 
{

};

TEST_CASE_METHOD(Test, "Collision")
{
	AABB box{ vec3(fixed16(10.0f)),vec3(fixed16(1.0f)) };
	AABB box2{ vec3(fixed16(25.0f)),vec3(fixed16(25.0f)) };
	REQUIRE(TestCollision(box, box2));
}

TEST_CASE_METHOD(Test, "OctTree")
{
	std::default_random_engine e;
	e.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float>c(0,1000);
	std::uniform_real_distribution<float>r(0, 10);
	std::vector<AABB> aabbs;
	aabbs.reserve(10000);
	std::vector<AABB> collisions;
	OctTree tree(vec3(fixed16(0)), vec3(fixed16(1000)),3);
	for (int i = 0; i < 250; ++i)
	{
		aabbs.emplace_back(vec3(fixed16(c(e))), vec3(fixed16(r(e))));
		
	}

	{
		AutoTimer t;
		for (int i = 0; i < 250; ++i)
		{
			tree.Insert(aabbs[i]);
		}
	}
	

	int cnt1 = 0 ,cnt2 = 0;
	{ 
		AutoTimer t;
		for (int i = 0; i < 250; ++i)
		{
			for (int j = 0; j < aabbs.size(); ++j)
			{
				if (i != j)
				{
					if (TestCollision(aabbs[i], aabbs[j])) ++cnt1;
				}
			}
		}
	}
	
	std::vector<const AABB*> objects;
	{
		
		objects.reserve(100000);
		AutoTimer t;
		for (int i = 0; i < 250; ++i)
		{
			tree.GetCollisionObject(aabbs[i], objects);
			//cnt2 += objects.size();
		}
	}

	REQUIRE(cnt1 == objects.size());
}

