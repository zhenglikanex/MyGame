#include "UnityInputService.hpp"

#include "Client/UnityBridge.hpp"

void UnityInputService::InputHandler() const
{
	UnityBridge::Get().CallUnity<void>("InputHandler");
}