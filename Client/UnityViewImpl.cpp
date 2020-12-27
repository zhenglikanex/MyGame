#include "UnityViewImpl.hpp"

#include "Framework/Game/Game.hpp"
#include "Client/UnityViewService.hpp"

void UnityViewImpl::UpdatePosition(const vec3& pos)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(*locator.Get<ViewService>());
		service.UpdatePosition(this, static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z));
	}
}

void UnityViewImpl::UpdateForward(const vec3& forward)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(*locator.Get<ViewService>());
		service.UpdatePosition(this, static_cast<float>(forward.x), static_cast<float>(forward.y), static_cast<float>(forward.z));
	}
}

void UnityViewImpl::MovePosition(const vec3& pos)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(*locator.Get<ViewService>());
		service.UpdatePosition(this, static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z));
	}
}

void UnityViewImpl::MoveForward(const vec3& forward)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(*locator.Get<ViewService>());
		service.UpdatePosition(this, static_cast<float>(forward.x), static_cast<float>(forward.y), static_cast<float>(forward.z));
	}
}