#pragma once

#include <algorithm>

#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Hit.hpp"

#include "Framework/Game/System.hpp"

struct HitTestSystem : public System
{
	HitTestSystem(entt::registry& _registry) : System(_registry) { }
	~HitTestSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto view = registry.view<Hit>();
		for (auto e : view)
		{
			auto& hit = view.get<Hit>(e);
			switch (hit.hit_type)
			{
			case HitType::kRay:
				
				break;
			case HitType::kOverlap:
				std::variant
				break;
			case HitType::kSweep:
				break;
			default:
				break;

			}
		}
	}

	void Finalize() override
	{

	}
};
