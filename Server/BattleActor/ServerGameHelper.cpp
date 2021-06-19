#include "ServerGameHelper.hpp"

ServerGameHelper::~ServerGameHelper()
{

}

bool ServerGameHelper::IsLocalPlayer(uint32_t id) const
{
	return true;
}

bool ServerGameHelper::IsRemotePlayer(uint32_t id) const
{
	return false;
}
