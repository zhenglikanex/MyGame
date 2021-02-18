#pragma once

#include "Framework/Game/IViewImpl.hpp"

class UnityViewImpl : public IViewImpl
{
public:
	UnityViewImpl(int handle) :handle_(handle) {}
	~UnityViewImpl()
	{

	}

	void UpdatePosition(const vec3& pos) override;
	void UpdateForward(const vec3& forward) override;
	void MovePosition(const vec3& pos) override;
	void MoveForward(const vec3& forward) override;
	void PlayAnimation(const std::string& name) override;
	void PlayAnimation(const std::string& name, float noramlized_time) override;

	int GetHandle() const { return handle_; }
private:
	int handle_;
};
