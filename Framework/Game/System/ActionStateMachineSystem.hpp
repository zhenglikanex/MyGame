#pragma once

#include <algorithm>

#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/Commond.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/ActionState.hpp"

struct ActionStateMachineSystem : public System
{
	ActionStateMachineSystem(entt::registry& _registry) : System(_registry) { }
	~ActionStateMachineSystem() {}

	bool Initialize() override
	{

		return true;
	}

	void Update(fixed16 dt) override
	{
		
	}

	void ProcessIdleState()
	{

	}

	void ProecessMovementState()
	{

	}

	void ProecessAttackState()
	{

	}

	void ProecessHurtState()
	{

	}

	void ProcessDethState()
	{

	}

	void Finalize() override
	{

	}
};
