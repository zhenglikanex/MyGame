#pragma once

#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"

#include "Framework/Game/Component/SkillGraphAsset.hpp"
#include "Framework/Game/Component/SkillState.hpp"

#include "Framework/Game/System.hpp"



struct CreateSkillGraphSystem : public System
{
	const std::string path = "Config/SkillGraph/";
	std::unordered_map<std::string, SkillGraphInfo> skill_graph_infos;

	CreateSkillGraphSystem(entt::registry& _registry) : System(_registry) { }
	~CreateSkillGraphSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto view = registry.view<SkillGraphAsset>(entt::exclude<SkillGraph>);
		for (auto e : view)
		{
			const auto& asset = view.get<SkillGraphAsset>(e);
			auto& skill_graph_info = GetSkillGraphInfo(asset.value);
			registry.emplace<SkillGraph>(e, &skill_graph_info);
		}
	}

	void Finalize() override
	{

	}

	SkillGraphInfo& GetSkillGraphInfo(const std::string& name)
	{
		auto iter = skill_graph_infos.find(name);
		if (iter != skill_graph_infos.end())
		{
			return iter->second;
		}
		else
		{
			auto& locator = registry.ctx<Locator>();
			auto content = locator.Ref<FileService>().OpenFile(path + name);

			assert(content != "" && "not skill graph info!");

			nlohmann::json j = nlohmann::json::parse(content);
			SkillGraphInfo info = j.get<SkillGraphInfo>();
			auto ret = skill_graph_infos.emplace(name, std::move(info));

			return ret.first->second;
		}
	}
};
