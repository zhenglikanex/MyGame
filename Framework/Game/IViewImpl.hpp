#pragma once

#include "Framework/Game/Math.hpp"

class IViewImpl
{
public:
	IViewImpl() {}
	virtual ~IViewImpl() = 0 {}
	
	virtual void UpdatePosition(const vec3& pos) = 0;
	virtual void UpdateRotation(const quat& rotation) = 0;
	virtual void MovePosition(const vec3& pos) = 0;
	virtual void MoveRotation(const quat& rotation) = 0;
	virtual void PlayAnimation(const std::string& name) = 0;
	virtual void PlayAnimation(const std::string& name, float noramlized_time) = 0;
};