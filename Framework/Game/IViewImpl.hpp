#pragma once

#include "Framework/Game/Math.hpp"

class IViewImpl
{
public:
	IViewImpl() {}
	virtual ~IViewImpl() = 0 {}
	
	virtual void UpdatePosition(const vec3& pos) = 0;
	virtual void UpdateForward(const vec3& forward) = 0;
	virtual void MovePosition(const vec3& pos) = 0;
	virtual void MoveForward(const vec3& forward) = 0;
	virtual void PlayAnimation(const std::string& name) = 0;
};