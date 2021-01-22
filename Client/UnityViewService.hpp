#pragma once

#include "Framework/Game/ViewService.hpp"

class UnityViewService : public ViewService
{
public:
	
public:
	std::unique_ptr<IViewImpl> Create(std::string_view asset) const override;
	void Destory(const std::unique_ptr<IViewImpl>& view) const override;

	void UpdatePosition(const IViewImpl* view,float x,float y,float z) const;
	void UpdateForward(const IViewImpl* view, float x, float y, float z) const;
	void MovePosition(const IViewImpl* view, float x, float y, float z) const;
	void MoveForward(const IViewImpl* view, float x, float y, float z) const;
	void PlayAnimation(const IViewImpl* view, std::string_view name) const;
private:
};