#pragma once

#include "Framework/Game/Service/ViewService.hpp"

class ServerViewService : public ViewService
{
public:
	std::unique_ptr<IViewImpl> Create(std::string_view asset) const override;
	void Destroy(const IViewImpl* view) const override;
private:
};