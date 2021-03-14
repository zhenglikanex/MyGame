#pragma once

#include <string>
#include <string_view>

#include "Framework/Game/Json.hpp"



struct ActorInfo
{
	std::string model_asset;
	std::string anim_asset;
	Collider body;
	Collider weapon;

	ActorInfo(std::string_view _model_asset, std::string_view _anim_asset,const Collider& _body,const Collider& _weapon)
		: model_asset(_model_asset)
		, anim_asset(_anim_asset)
		, body(_body)
		, weapon(_weapon)
	{

	}
};