#include "Game.hpp"
#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Commond.hpp"
#include "Framework/Game/Component/CommondList.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/System/SyncSystem.hpp"

Game::Game(Locator&& locator,GameMode mode)	
{
	registry_.set<GameMode>(mode);
	registry_.set<Locator>(std::forward<Locator>(locator));
	registry_.set<GameState>();
	registry_.set<CommondGroup>();

	systems_.emplace_back(std::make_unique<SyncSystem>());
}

Game::~Game()
{

}

bool Game::Initialize()
{
	for (auto& system : systems_)
	{
		if (!system->Initialize())
		{
			return false;
		}
	}

	return true;
}

void Game::Update(float dt)
{
	if (registry_.ctx<GameMode>() == GameMode::kClinet)
	{
		CheckPredictFrame();
	}

	auto& game_state = registry_.ctx<GameState>();

	uint32_t mill_dt = dt * 1000;
	game_state.run_time += mill_dt;
	
	while (game_state.run_time > game_state.run_frame * kFrameRate + kFrameRate)
	{
		bool predict = false;

		if (registry_.ctx<GameMode>() == GameMode::kServer)
		{
			//生成没有收到当前帧的玩家commond
			SetupCommonds(game_state.run_frame);
		}
		registry_.set<CommondGroup>(GetCommondGroup(game_state.run_frame));
		if (registry_.ctx<GameMode>() == GameMode::kClinet)
		{
			if (registry_.ctx<CommondGroup>().commonds.size() < registry_.view<Player>().size() && game_state.run_frame - game_state.real_frame < kMaxPredictFrame)
			{
				auto commond_group = PredictCommondGroup(game_state.run_frame);
				registry_.set<CommondGroup>(commond_group);
				predict_commond_groups_.emplace(game_state.run_frame, std::move(commond_group));
				predict = true;
			}
		}
		
		if (registry_.ctx<CommondGroup>().commonds.size() == registry_.view<Player>().size())
		{
			for (auto& system : systems_)
			{
				system->Update(fixed16(0.33));
			}

			if (predict)
			{
				++registry_.ctx<GameState>().run_frame;
			}
			else
			{
				++registry_.ctx<GameState>().real_frame;
				++registry_.ctx<GameState>().run_frame;
			}

			if (registry_.ctx<GameMode>() == GameMode::kClinet && snapshots_.size() > kMaxPredictFrame)
			{
				snapshots_.erase(snapshots_.begin());
			}
			
			SaveSnapshot();
		}
	}
}

void Game::Finalize()
{
	for (auto iter = systems_.rbegin(); iter != systems_.rend(); ++iter)
	{
		auto& system = *iter;
		system->Finalize();
	}
}

void Game::InputCommond(uint32_t id,Commond&& commond)
{
	auto iter = commonds_map_.find(id);
	if(iter != commonds_map_.end())
	{
		iter->second.emplace_back(std::move(commond));
	}
	else
	{
		std::vector<Commond> commonds;
		commonds.emplace_back(std::move(commond));
		commonds_map_.emplace(id, commonds);
	}
}

void Game::SetupCommonds(uint32_t frame)
{
	if (registry_.ctx<GameMode>() == GameMode::kServer)
	{
		for (auto& it : commonds_map_)
		{
			if (it.second.size() == 0)
			{
				it.second.emplace_back(Commond());
			}
			else if (it.second.size() <= frame)
			{
				it.second.push_back(it.second.back());
			}
		}
	}
}

CommondGroup Game::GetCommondGroup(uint32_t frame)
{
	CommondGroup commonds;
	for(auto& it : commonds_map_)
	{
		if (it.second.size() > frame) {
			commonds.commonds.emplace(it.first, it.second[frame]);
		}
	}

	return commonds;
}

std::vector<CommondGroup> Game::GetAllCommondGroups()
{
	std::vector<CommondGroup> commond_groups;
	for (uint32_t frame = 0;frame <= registry_.ctx<GameState>().real_frame; ++frame)
	{
		commond_groups.emplace_back(GetCommondGroup(frame));
	}
	return commond_groups;
}

CommondGroup Game::PredictCommondGroup(uint32_t frame)
{
	CommondGroup commond_group = GetCommondGroup(frame - 1);
	//todo: 填充自己的上一帧的输入
	return commond_group;
}

void Game::CheckPredictFrame()
{
	auto game_state = registry_.ctx<GameState>();
	for (uint32_t frame = game_state.real_frame; frame < game_state.run_frame; ++frame)
	{
		auto commond_group = GetCommondGroup(frame);
		if (commond_group.commonds.size() < registry_.view<Player>().size())
		{
			return;
		}

		auto& predict_commonds = predict_commond_groups_.find(frame)->second;
		if (commond_group == predict_commonds)
		{
			predict_commond_groups_.erase(game_state.real_frame);
			snapshots_.erase(game_state.real_frame);
			++game_state.real_frame;
		}
		else
		{
			auto& snapshot = snapshots_.find(frame);
			//registry_.reset(snapshot);	// todo恢复快照
			game_state.run_frame = frame;

			// 清除预测的数据
			predict_commond_groups_.clear();
			snapshots_.clear();
			return;
		}
	}

	return;
}

void Game::SaveSnapshot()
{

}