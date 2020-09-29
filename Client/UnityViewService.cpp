#include "UnityViewService.hpp"
#include "UnityViewImpl.hpp"
#include "Client/UnityBridge.hpp"

std::unique_ptr<IViewImpl> UnityViewService::Create(std::string_view asset)
{
	return std::make_unique<UnityViewImpl>(UnityBridge::Get().CallUnity<uint32_t>("CreateView",asset.data()));
}

void UnityViewService::Destory(const std::unique_ptr<IViewImpl>& view)
{
	if (!view)
	{
		return;
	}

	auto unity_view = dynamic_cast<UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("DestoryView", unity_view.GetHandle());
}

void UnityViewService::UpdatePosition(IViewImpl* view)
{
	if (!view)
	{
		return;
	}

	auto unity_view = dynamic_cast<UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("UpdatePosition", unity_view.GetHandle());
}

void UnityViewService::UpdateForward(IViewImpl* view)
{
	if (!view)
	{
		return;
	}

	auto unity_view = dynamic_cast<UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("UpdateForward", unity_view.GetHandle());
}