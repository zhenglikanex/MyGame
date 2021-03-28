#include "UnityViewImpl.hpp"

#include "Framework/Game/Game.hpp"
#include "Client/UnityViewService.hpp"

UnityViewImpl::~UnityViewImpl()
{
	if (g_game)
	{
		// ctx数据会先于locator
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(locator.Ref<ViewService>());
		service.Destory(this);
	}
}

void UnityViewImpl::UpdatePosition(const vec3& pos)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(locator.Ref<ViewService>());
		service.UpdatePosition(this, static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z));
	}
}

void UnityViewImpl::UpdateRotation(const quat& rotation)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(locator.Ref<ViewService>());
		service.UpdateRotation(this, static_cast<float>(rotation.x), static_cast<float>(rotation.y), static_cast<float>(rotation.z), static_cast<float>(rotation.w));
	}
}

void UnityViewImpl::MovePosition(const vec3& pos)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(locator.Ref<ViewService>());
		float x = static_cast<float>(pos.x);
		float y = static_cast<float>(pos.y);
		float z = static_cast<float>(pos.z);
		service.MovePosition(this, x, y, z);
	}
}

void UnityViewImpl::MoveRotation(const quat& rotation)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(locator.Ref<ViewService>());
		service.MoveRotation(this, static_cast<float>(rotation.x), static_cast<float>(rotation.y), static_cast<float>(rotation.z), static_cast<float>(rotation.w));
	}
}

void UnityViewImpl::PlayAnimation(const std::string& name)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(locator.Ref<ViewService>());
		service.PlayAnimation(this, name);
	}
}

void UnityViewImpl::PlayAnimation(const std::string& name, float normalized_time)
{
	if (g_game)
	{
		auto& locator = g_game->registry().ctx<Locator>();
		auto& service = dynamic_cast<const UnityViewService&>(locator.Ref<ViewService>());
		service.PlayAnimation(this, name,normalized_time);
	}
}