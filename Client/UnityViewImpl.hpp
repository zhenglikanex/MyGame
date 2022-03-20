#pragma once

#include "Framework/Game/IViewImpl.hpp"

class UnityViewService;

class UnityViewImpl : public IViewImpl
{
public:
	UnityViewImpl(UnityViewService* service, int handle);
	~UnityViewImpl();

	void UpdatePosition(const vec3& pos) override;
	void UpdateRotation(const quat& rotation) override;
	void MovePosition(const vec3& pos) override;
	void MoveRotation(const quat& rotation) override;
	void PlayAnimation(const std::string& name) override;
	void PlayAnimation(const std::string& name, float noramlized_time) override;
	void ApplyCue(std::string_view name) override;
	void ApplyCue(const std::shared_ptr<IViewImpl>& view) override;

	int GetHandle() const { return handle_; }
private:
	UnityViewService* service_;
	int handle_;
};
