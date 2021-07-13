#pragma once

#include <vector>

#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"


struct Contact
{
	entt::entity entity;
	Contact()
		: entity(entt::null)
	{

	}
	Contact(entt::entity _entity)
		: entity(_entity)
	{

	}
	// todoһЩ��ײ��Ϣ,������ײ��֮��
};

struct ContactList
{
	std::vector<Contact> value;

	ContactList()
	{
	}
};

NON_MEMEBER_BAR(Contact, obj.entity);
NON_MEMEBER_BAR(ContactList, obj.value);