#pragma once

#include "Framework/Game/Math.hpp"

class IViewImpl
{
public:
	IViewImpl() {}
	virtual ~IViewImpl() = 0 {}
	
	virtual void SetPosition(const vec3& pos) = 0;
	virtual void SetForward(const vec3& forward) = 0;
};