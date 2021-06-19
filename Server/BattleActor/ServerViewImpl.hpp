#pragma once

#include "Framework/Game/IViewImpl.hpp"

class ServerViewImpl : public IViewImpl
{
public:
	ServerViewImpl();
	~ServerViewImpl();

	void UpdatePosition(const vec3& pos) override;
	void UpdateRotation(const quat& rotation) override;
	void MovePosition(const vec3& pos) override;
	void MoveRotation(const quat& rotation) override;
	void PlayAnimation(const std::string& name) override;
	void PlayAnimation(const std::string& name, float noramlized_time) override;

	int GetHandle() const { return handle_; }
private:
	int handle_;
};
