#pragma once

#include <memory>
#include <string>
#include "Framework/Game/Service.hpp"

class IViewImpl;

class ViewService : public Service
{
public:
	virtual ~ViewService() = 0 {}

	virtual std::unique_ptr<IViewImpl> Create(std::string_view asset) = 0;
	virtual void Destory(const std::unique_ptr<IViewImpl>& view) = 0;
private:
};