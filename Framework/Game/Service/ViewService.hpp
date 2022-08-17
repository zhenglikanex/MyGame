#pragma once

#include <memory>
#include <string>
#include "Framework/Game/Service/Service.hpp"

class IViewImpl;

class ViewService : public Service
{
public:
	virtual std::unique_ptr<IViewImpl> Create(std::string_view asset) const = 0;
	virtual void Destroy(const IViewImpl* view) const = 0;
private:
};