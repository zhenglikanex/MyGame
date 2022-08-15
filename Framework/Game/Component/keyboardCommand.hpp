#pragma once

#include <unordered_map>
#include <string>

#include "Framework/Game/Component/Remote.hpp"

enum class KeyStatus
{
	kDown,
	kUp
};

struct Keyboard
{
	std::unordered_map<std::string, KeyStatus> value;
};

NON_MEMEBER_BAR(Keyboard, obj.value);

struct KeyboardCommand
{
	std::unordered_map<Remote, Keyboard> value;
};

NON_MEMEBER_BAR(KeyboardCommand, obj.value);