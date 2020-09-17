#pragma once

#include "Framework/Game/Service.hpp"

class NetworkService : public Service
{
public:
	virtual void Send(uint8_t* data,uint32_t len) = 0;
};