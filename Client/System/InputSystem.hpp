#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/PlayerInput.hpp"
#include "Framework/Game/Service/InputService.hpp"
#include "Framework/Game/Service/NetworkService.hpp"

#include "Framework/Game/Component/Command.hpp"

#include "Framework/Game/System.hpp"


struct InputSystem : System
{
public:
	uint32_t input_frame;
	kanex::Buffer buffer;
	entt::entity player;

	explicit InputSystem(Registry& _registry) 
		: System(_registry)
		, input_frame(0)
		, buffer(0)
		, player(entt::null)
	{
	}

	bool Initialize() override
	{
		return true;
	}

	void Update(float dt) override
	{
		auto& game_state = registry.ctx<GameState>();
		while (input_frame <= game_state.real_frame)
		{
			++input_frame;

			// todo
			if (player == entt::null || !registry.valid(player))
			{
				auto view = registry.view<Local, Remote, Player>();
				for (auto e : view)
				{
					player = e;
				}
			}

			if (!registry.valid(player))
			{
				return;
			}

			auto& locator = registry.ctx<Locator>();
			auto& input_service = locator.Ref<InputService>();
			Input input = input_service.GetInput();

			auto& command_state = registry.ctx_or_set<CommandState>();
			Command& command = command_state.value[game_state.run_frame % command_state.value.size()];
			command.move_command.value.clear();
			command.gameplay_ability_command.value.clear();

			BuildCommand(command, input);

			command.frame = game_state.run_frame;
			SendInput(registry.get<Player>(player).id,game_state,command);
		}
	}

	void BuildCommand(Command& command,const Input& input) const
	{
		BuildMoveCommand(command,input);
		BuildAbilityCommand(command,input);
	}

	void BuildMoveCommand(Command& command, const Input& input) const
	{
		if (input.x_axis != 0 || input.y_axis != 0)
		{
			command.move_command.value.emplace_back(registry.get<Remote>(player), input.x_axis, input.y_axis);
		}
	}

	void BuildAbilityCommand(Command& command, const Input& input) const
	{
		if(input.abilities.empty())
		{
			return;
		}

		auto source = registry.get<Remote>(player);
		std::unordered_map<std::string,GameplayAbilityCommandSpec>& ability_command_specs = command.gameplay_ability_command.value[source];
		for (auto& [ability,input_key_status] : input.abilities)
		{
			if(ability_command_specs.find(ability) == ability_command_specs.end())
			{
				ability_command_specs.emplace(ability,GameplayAbilityCommandSpec(ability, source, Remote::null, input_key_status == InputKeyStatus::kDown));
			}
		}
	}

	void SendInput(uint32_t id, const GameState& game_state, const Command& command)
	{
		buffer.Clear();
		kanex::BinaryStream stream(buffer);
		kanex::BinaryOutputArchive ar(stream);

		PlayerInput input;
		input.id = id;
		input.game_state = game_state;
		input.command = command;

		ar(input);

		std::vector<uint8_t> data(buffer.Size());
		buffer.Read(data.data(), data.size());

		auto& network_service = registry.ctx<Locator>().Ref<NetworkService>();
		network_service.Send("player_input", std::move(data));
	}

	void Finalize() override
	{

	}
};