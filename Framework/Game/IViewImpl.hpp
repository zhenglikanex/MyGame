#pragma once

#include <memory>
#include <string_view>

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
	virtual void ApplyCue(std::string_view name) = 0;
	virtual void ApplyCue(const std::shared_ptr<IViewImpl>& view) = 0;
};