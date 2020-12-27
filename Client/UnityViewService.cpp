#include "UnityViewService.hpp"
#include "UnityViewImpl.hpp"
#include "Client/UnityBridge.hpp"

std::unique_ptr<IViewImpl> UnityViewService::Create(std::string_view asset) const
{
	return std::make_unique<UnityViewImpl>(UnityBridge::Get().CallUnity<uint32_t>("CreateView", asset.data()));
}

void UnityViewService::Destory(const std::unique_ptr<IViewImpl>& view) const
{
	if (!view)
	{
		return;
	}

	auto unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("DestoryView", unity_view.GetHandle());
}

void UnityViewService::UpdatePosition(const IViewImpl* view, float x, float y, float z) const
{
	if (!view)
	{
		return;
	}

	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("UpdatePosition", unity_view.GetHandle(),x,y,z);
}

void UnityViewService::UpdateForward(const IViewImpl* view, float x, float y, float z) const
{
	if (!view)
	{
		return;
	}

	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("UpdateForward", unity_view.GetHandle(),x,y,z);
}

void UnityViewService::MovePosition(const IViewImpl* view, float x, float y, float z) const
{
	if (!view)
	{
		return;
	}

	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("MovePosition", unity_view.GetHandle(),x,y,z);
}

void UnityViewService::MoveForward(const IViewImpl* view, float x, float y, float z) const
{
	if (!view)
	{
		return;
	}

	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("MoveForward", unity_view.GetHandle(),x,y,z);
}