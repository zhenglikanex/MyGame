#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/Commond.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/LogService.hpp"
#include "Framework/game/Locator.hpp"

struct CommondProcessSystem : public System
{
	CommondProcessSystem(entt::registry& _registry) : System(_registry) { }
	~CommondProcessSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		const auto& commond_group = registry.ctx<CommondGroup>();
		auto view = registry.view<Player>();
		for (auto e : view)
		{
			const auto& player = view.get<Player>(e);
			auto iter = commond_group.value.find(player.id);
			if (iter != commond_group.value.end())
			{
				INFO("x_axis:{} y_axis:{}", static_cast<float>(iter->second.x_axis), static_cast<float>(iter->second.y_axis));
				// movement commond
				if (iter->second.x_axis != fixed16(0.0) || iter->second.y_axis != fixed16(0.0))
				{
					registry.emplace_or_replace<MovementCommond>(e, iter->second.x_axis, iter->second.y_axis);
				}
			}
		}
	}
	
	void Finalize() override
	{

	}
};
