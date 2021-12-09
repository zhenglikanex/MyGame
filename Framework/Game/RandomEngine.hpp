#pragma once

#include <random>

class RandomEngine
{
public:
	RandomEngine(uint32_t seed)
		: seed_(seed)
		, engine_(seed)
	{

	}
	
	// todo
	void Rollback(uint32_t frame)
	{
		
	}

	uint32_t Random(uint32_t begin, uint32_t end)
	{
		std::uniform_int_distribution<uint32_t> dis(begin, end);
		return dis(engine_);
	}

	uint32_t seed() const { return seed_; }
private:
	uint32_t seed_;
	std::default_random_engine engine_;
};