#pragma once

#include <Windows.h>
#include <DirectXTK/Keyboard.h>

#include "../base/Math.h"

namespace Input
{
	using DirectX::Keyboard;

	using KeyCode = Keyboard::Keys;

	enum class MouseButton
	{
		Left = 0,
		Right = 1,
		Middle = 2
	};

	struct MouseState
	{
		bool isPressed = false;
		bool isReleased = false;
		bool isHeld = false;
		bool isUp = false;
	};

	enum class PadButton
	{
		A,B,X,Y,
		Left, Right, Up, Down,
		LeftShoulder, RightShoulder,
	};

	struct PadButtonState
	{
		bool isPressed = false;
		bool isReleased = false;
		bool isHeld = false;
		bool isUp = false;
	};

	void Initialize(HWND hWnd);

	void Update();

	//Returns true while the user holds down the key identified by name.
	bool GetKey(KeyCode key);

	bool GetKeyDown(KeyCode key);

	bool GetKeyUp(KeyCode key);

	MouseState GetMouseState(MouseButton button);

	// これはスクリーンポジションなのかな...
	tama::Int2 GetMousePosition();

	namespace XInput
	{
		PadButtonState GetPadButton(uint32_t index, PadButton button);
	}

	namespace DirectInput
	{
		enum class Button
		{
			Z, X, C, V, L2, R2, L1, R1, Start, Select,
			Up, Down, Left, Right,
		};

		bool IsAnyGamepadActive();

		bool GetButtonDown(unsigned player, Button button);
	}

	bool hoge();
}
