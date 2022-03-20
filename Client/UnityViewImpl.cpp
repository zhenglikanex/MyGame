#include "UnityViewImpl.hpp"

#include "Client/UnityViewService.hpp"

UnityViewImpl::UnityViewImpl(UnityViewService* service, int handle)
	: IViewImpl()
	, service_(service)
	, handle_(handle)
{
}

UnityViewImpl::~UnityViewImpl()
{
	service_->Destory(this);
}

void UnityViewImpl::UpdatePosition(const vec3& pos)
{
	service_->UpdatePosition(this, pos.x, pos.y, pos.z);
}

void UnityViewImpl::UpdateRotation(const quat& rotation)
{
	service_->UpdateRotation(this, rotation.x, rotation.y, rotation.z, rotation.w);
}

void UnityViewImpl::MovePosition(const vec3& pos)
{
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;
	service_->MovePosition(this, x, y, z);
}

void UnityViewImpl::MoveRotation(const quat& rotation)
{
	service_->MoveRotation(this, rotation.x, rotation.y, rotation.z, rotation.w);
}

void UnityViewImpl::PlayAnimation(const std::string& name)
{
	service_->PlayAnimation(this, name);
}

void UnityViewImpl::PlayAnimation(const std::string& name, float normalized_time)
{
	service_->PlayAnimation(this, name, normalized_time);
}

void UnityViewImpl::ApplyCue(std::string_view name)
{
	service_->ApplyCue(this, name);
}

void UnityViewImpl::ApplyCue(const std::shared_ptr<IViewImpl>& view)
{
}
