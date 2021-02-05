#pragma once

#include <string>
#include <string_view>

#include "Framework/Game/Json.hpp"

#include "Framework/Game/Component/BoundingBox.hpp"

struct ActorInfo
{
	std::string model_asset;
	std::string anim_asset;
	BoundingBox body;
	BoundingBox weapon;

	ActorInfo(std::string_view _model_asset, std::string_view _anim_asset,const BoundingBox& _body,const BoundingBox& _weapon)
		: model_asset(_model_asset)
		, anim_asset(_anim_asset)
		, body(_body)
		, weapon(_weapon)
	{

	}
};