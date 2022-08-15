#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/InputService.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/MoveCommand.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"

struct InputSystem : System
{
public:
	uint32_t input_frame;
	kanex::Buffer buffer;
	entt::entity player;

	InputSystem(Registry& _registry) 
		: System(_registry)
		, input_frame(0)
		, buffer(0)
		, player(entt::null)
	{
	}

	~InputSystem() {}

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
			command.frame = game_state.run_frame;

			BuildCommand(command, input);
			SendCommand(command);
		}
	}

	void BuildCommand(Command& command,const Input& input) const
	{
		BuildMoveCommand(command,input);
		BuildAbilityCommand(command,input);
	}

	void BuildMoveCommand(Command& command, const Input& input) const
	{
		if (input.x_axis > 0 || input.y_axis > 0)
		{
			command.move_command.value.emplace_back(registry.get<Remote>(player), input.x_axis, input.y_axis);
		}
	}

	void BuildAbilityCommand(Command& command, const Input& input) const
	{
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

	void SendCommand(const Command& command)
	{
		buffer.Clear();
		kanex::BinaryStream stream(buffer);
		kanex::BinaryOutputArchive ar(stream);

		ar(command);

		std::vector<uint8_t> data(buffer.Size());
		buffer.Read(data.data(), data.size());

		auto& network_service = registry.ctx<Locator>().Ref<NetworkService>();
		network_service.Send("input_command", std::move(data));
	}

	void Finalize() override
	{

	}
};