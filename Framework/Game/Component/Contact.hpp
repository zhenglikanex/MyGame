#pragma once

#include <vector>

#include "3rdparty/include/entt/entt.hpp"


struct Contact
{
	entt::entity entity;
	// todo一些碰撞信息,比如碰撞点之类
};

struct ContactList
{
	std::vector<Contact> value;

	ContactList()
	{
	}
};