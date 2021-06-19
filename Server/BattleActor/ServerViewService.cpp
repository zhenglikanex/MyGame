#include "ServerViewService.hpp"

#include "Server/BattleActor/ServerViewImpl.hpp"

std::unique_ptr<IViewImpl> ServerViewService::Create(std::string_view asset) const
{
	return std::make_unique<ServerViewImpl>();
}

void ServerViewService::Destory(const IViewImpl* view) const
{
	
}
