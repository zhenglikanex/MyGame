#pragma once

#include "Framework/Game/GameHelper.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Command.hpp"

#include "Framework/Game/System.hpp"

struct RecvCommandSystem : public System
{
	RecvCommandSystem(Registry& _registry)
		: System(_registry)
	{ }
	~RecvCommandSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto& network_service = registry.ctx<Locator>().Ref<const NetworkService>();
		while (auto message = network_service.Recv("input_command"))
		{
			auto& data = *message;

			kanex::Buffer buffer(std::move(data));
			kanex::BinaryStream stream(buffer);
			kanex::BinaryInputArchive ar(stream);

			Command input_command;
			ar(input_command);

			auto& game_state = registry.ctx<GameState>();
			
			if (game_state.run_frame > input_command.frame)
			{
				continue;
			}

			auto& command_state = registry.ctx_or_set<CommandState>();
			auto& command = command_state.value[input_command.frame % command_state.value.size()];
			
			if (command.frame != input_command.frame)
			{
				command.frame = input_command.frame;
				command.move_command.value.clear();
				command.gameplay_ability_command.value.clear();
			}

			for (auto& spec : input_command.move_command.value)
			{
				command.move_command.value.push_back(spec);
			}

			for (auto& item : input_command.gameplay_ability_command.value)
			{
				if (input_command.gameplay_ability_command.value.find(item.first)
					!= input_command.gameplay_ability_command.value.end())
				{
					input_command.gameplay_ability_command.value.emplace(item.first,std::move(item.second));
					continue;
				}
				
				auto& gameplay_ability_command = input_command.gameplay_ability_command.value[item.first];
				for (auto& spec : item.second)
				{
					gameplay_ability_command.push_back(spec);
				}
			}
		}
	}

	void Finalize() override
	{

	}
};