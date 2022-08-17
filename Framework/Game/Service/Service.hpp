#pragma once

class Service
{
public:
	Service() = default;
	Service(const Service&) = delete;
	Service(Service&&) = default;
	Service& operator=(const Service&) = delete;
	Service& operator=(Service&&) = default;

	virtual ~Service() = 0 {}

	//virtual bool Initialize() = 0;
	//virtual void Finalize() = 0;
private:
};