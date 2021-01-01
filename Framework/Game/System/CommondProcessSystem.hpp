#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/Commond.hpp"
#include "Framework/Game/Component/Player.hpp"

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
				// movement commond
				registry.emplace_or_replace<MovementCommond>(e,iter->second.x_axis, iter->second.y_axis);
			}
		}
	}

	void Finalize() override
	{

	}
};
