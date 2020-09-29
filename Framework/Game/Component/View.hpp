#pragma once

#include <memory>

#include "Framework/Game/IViewImpl.hpp"

struct View
{
	std::unique_ptr<IViewImpl> value;
};