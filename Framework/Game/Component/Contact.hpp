#pragma once

#include <vector>

#include "3rdparty/include/entt/entt.hpp"


struct Contact
{
	entt::entity entity;
	// todoһЩ��ײ��Ϣ,������ײ��֮��
};

struct ContactList
{
	std::vector<Contact> value;

	ContactList()
	{
	}
};