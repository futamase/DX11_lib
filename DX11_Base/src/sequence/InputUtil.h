#pragma once

#include "../System/Input.h"
#include <unordered_map>


class InputUtil
{
public:
	enum Mode { Keyboard, Gamepad };
	enum Button
	{
		Up, Down, Left, Right,
		One, Two, Three, Four,
		Start, Select,
	};

	static std::unordered_map<int, int> keyMap;

private:
	static Mode mode;

public:
	static void Init();
	static bool GetButtonDown(Button button);
};