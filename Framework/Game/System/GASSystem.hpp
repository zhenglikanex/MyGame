#pragma once

#include "Framework/Game/System.hpp"
#include "Framework/Game/System/GAS/ApplyGameplayEffectSystem.hpp"

struct GASSystem : public System
{
	std::vector<std::unique_ptr<System>> systems;

	GASSystem(entt::registry& _registry) : System(_registry) { }
	~GASSystem() {}

	bool Initialize() override
	{
		systems.push_back(std::make_unique<ApplyGameplayEffectSystem>(registry));

		for (auto& system : systems)
		{
			if (!system->Initialize())
			{
				return false;
			}
		}

		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		for (auto& system : systems)
		{
			system->FixedUpdate(dt);
		}
	}

	void Finalize() override
	{
		for (auto& system : systems)
		{
			system->Finalize();
		}

		systems.clear();
	}
};
