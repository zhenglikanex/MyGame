#include "UnityInputService.hpp"

#include "Client/UnityBridge.hpp"

inline void from_json(const nlohmann::json& json, Input& input)
{
	json.at("xAxis").get_to(input.x_axis);
	json.at("yAxis").get_to(input.y_axis);
	json.at("abilities").get_to(input.abilities);
}

Input UnityInputService::GetInput() const
{
	return UnityBridge::Get().CallUnity<Input>("InputHandler");
}