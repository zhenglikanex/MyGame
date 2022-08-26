#include "UnityViewService.hpp"
#include "UnityViewImpl.hpp"
#include "Client/UnityBridge.hpp"

std::unique_ptr<IViewImpl> UnityViewService::Create(std::string_view asset) const
{
	return std::make_unique<UnityViewImpl>(this,UnityBridge::Get().CallUnity<uint32_t>("CreateView", asset.data()));
}

void UnityViewService::Destroy(const IViewImpl* view) const
{
	if (!view)
	{
		return;
	}

	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
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

void UnityViewService::UpdateRotation(const IViewImpl* view, float x, float y, float z, float w) const
{
	if (!view)
	{
		return;
	}

	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("UpdateRotation", unity_view.GetHandle(),x,y,z,w);
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

void UnityViewService::MoveRotation(const IViewImpl* view, float x, float y, float z, float w) const
{
	if (!view)
	{
		return;
	}

	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("MoveRotation", unity_view.GetHandle(),x,y,z,w);
}

void UnityViewService::PlayAnimation(const IViewImpl* view,std::string_view name,bool is_timeline) const
{
	if (!view)
	{
		return;
	}
	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("PlayAnimation", unity_view.GetHandle(), name.data(),is_timeline);
}

void UnityViewService::PlayAnimation(const IViewImpl* view, std::string_view name, float normalized_time,bool is_timeline) const
{
	if (!view)
	{
		return;
	}
	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("PlayAnimationToTime", unity_view.GetHandle(), name.data(),normalized_time,is_timeline);
}

void UnityViewService::PlayTimeline(const IViewImpl* view, std::string_view name, uint32_t frame) const
{
	if(!view)
	{
		return;
	}

	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("PlayTimeline", unity_view.GetHandle(), name.data(),frame);
}

void UnityViewService::ApplyCue(const IViewImpl* view, std::string_view name) const
{
	if (!view)
	{
		return;
	}
	auto& unity_view = dynamic_cast<const UnityViewImpl&>(*view);
	UnityBridge::Get().CallUnity<void>("ApplyCue", unity_view.GetHandle(), name.data());
}
