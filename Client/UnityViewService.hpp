#pragma once

#include "Framework/Game/ViewService.hpp"

class UnityViewService : public ViewService
{
public:
	
public:
	std::unique_ptr<IViewImpl> Create(std::string_view asset) const override;
	void Destory(const std::unique_ptr<IViewImpl>& view) const override;

	void UpdatePosition(const IViewImpl* view,float x,float y,float z) const;
	void UpdateRotation(const IViewImpl* view, float x, float y, float z, float w) const;
	void MovePosition(const IViewImpl* view, float x, float y, float z) const;
	void MoveRotation(const IViewImpl* view, float x, float y, float z, float w) const;
	void PlayAnimation(const IViewImpl* view, std::string_view name) const;
	void PlayAnimation(const IViewImpl* view, std::string_view name,float normalized_time) const;
private:
};