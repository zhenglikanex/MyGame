#pragma once

#include "Framework/Game/ViewService.hpp"

class UnityViewService : public ViewService
{
public:
	
public:
	std::unique_ptr<IViewImpl> Create(std::string_view asset) override;
	void Destory(const std::unique_ptr<IViewImpl>& view) override;

	void UpdatePosition(IViewImpl* view);
	void UpdateForward(IViewImpl* view);
private:
};