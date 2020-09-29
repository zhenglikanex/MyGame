#pragma once

#include "Framework/Game/IViewImpl.hpp"

class UnityViewImpl : public IViewImpl
{
public:
	UnityViewImpl(int handle) :handle_(handle) {}
	~UnityViewImpl()
	{

	}

	void SetPosition(const vec3& pos) override
	{
		
	}

	void SetForward(const vec3& forward) override
	{

	}
	
	int GetHandle() const { return handle_; }
private:
	int handle_;
};
