#pragma once

#include "Kanex.hpp"

struct Destory
{
	bool verify;
	
	Destory()
		:verify(false)
	{

	}
};

NON_MEMEBER_BAR(Destory, obj.verify);