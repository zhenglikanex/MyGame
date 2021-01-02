#pragma once

class Service
{
public:
	Service() = default;
	Service(const Service&) = delete;
	const Service& operator=(const Service&) = delete;

	virtual ~Service() = 0 {}

	//virtual bool Initialize() = 0;
	//virtual void Finalize() = 0;
private:
};