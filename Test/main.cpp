

#include "3rdparty/include/catch2/catch.hpp"
#include <chrono>
#include <any>
#include <iostream>
#include "Framework/Game/Math.hpp"
#include <variant>
#include <iostream>
#include <bitset>

#include <thread>

#include "Framework/Game/System/CreateActorSystem.hpp"
#include "Server/BattleActor/System/ServerSyncSystem.hpp"
#include "Framework/Game/GAS/GameplayTagDefine.hpp"
#include "Framework/Game/Registry.hpp"


class Timer
{
public:
	Timer()
	{
		start_ = std::chrono::system_clock::now();
	}

	uint32_t Elapsed()
	{
		return duration_cast<milliseconds>(system_clock::now() - start_).count();
	}
private:
	std::chrono::system_clock::time_point start_;
};

class AutoTimer
{
public:
	AutoTimer() = default;
	AutoTimer(const std::string& _name) : name(_name) { }
	~AutoTimer() {
		std::cout << name << ":" << timer.Elapsed() << " ms" << std::endl;
	}

	Timer timer;
	std::string name;
};

using namespace std::chrono;

struct Serialize1
{
	
};


template<class Component>
int Func1(uint32_t i)
{
	std::cout << i << std::endl;
	return 1;
}

int main()
{
	using namespace  std::chrono;

	int num = 20;
	Registry registry(GameMode::kAuthority, {});
	registry.set<GameState>();
	for (auto i = 0; i < num; ++i)
	{
		auto e = registry.create();
		registry.emplace<Remote>(e, e);
		registry.emplace<Local>(e, i, 0);
		registry.emplace<Transform>(e);
		registry.emplace<AnimationClip>(e);
		registry.emplace<GameplayAttributeSet>(e);
		//spec.effect = "1213131231444";

	}

	SerializationManager manager;

	for (int i = 0; i < 30; ++i)
	{
		auto start = system_clock::now();
		
		for (uint32_t i = 0; i <= num; i++)
		{
			manager.Clear();
			manager.Serialize(registry, { i,0 });

			std::vector<uint8_t> buffer(manager.buffer().Size());
			manager.buffer().Read(buffer.data(), buffer.size());
		}

		auto duration = system_clock::now() - start;

		std::cout << "ms" << duration_cast<milliseconds>(duration).count() << std::endl;
	}

	std::unordered_map<Local, GameplayEffectSpec> map;
	for (uint32_t i = 0; i < num; ++i)
	{
		GameplayEffectSpec spec;
		spec.effect = "1213131231444";
		map.emplace(Local{ 1,i }, spec);
	}

	
	auto start = system_clock::now();
	kanex::Buffer buffer(1000);
	kanex::BinaryStream stream(buffer);
	kanex::BinaryOutputArchive iar(stream);
	for (uint32_t i = 0; i <= num; i++)
	{
		buffer.Clear();
		iar(map);
		/*std::vector<uint8_t> buffer(manager.buffer().Size());
		manager.buffer().Read(buffer.data(), buffer.size());*/
	}

	auto duration = system_clock::now() - start;

	std::cout << "2  ms" << duration_cast<milliseconds>(duration).count() << std::endl;

	{

		AutoTimer timer("map");
		std::vector<GameplayEffectSpec> vec;
		for (uint32_t i = 0; i < num; ++i)
		{
			GameplayEffectSpec spec;
			spec.effect = "1213131231444";
			vec.emplace_back(spec);
		}


		kanex::Buffer buffer(1000);
		kanex::BinaryStream stream(buffer);
		kanex::BinaryOutputArchive iar(stream);
		for (uint32_t i = 0; i <= num; i++)
		{
			buffer.Clear();
			iar(vec);
			/*std::vector<uint8_t> buffer(manager.buffer().Size());
			manager.buffer().Read(buffer.data(), buffer.size());*/
		}

	}
}