#include <Windows.h>
#include "Input.h"
#include <DirectXTK/Mouse.h>
#include <DirectXTK/GamePad.h>
#include "DXInput.h"

#include <array>
#include <cassert>

namespace Input
{
	using namespace DirectX;

	static std::unique_ptr<Mouse> s_mouse;
	static Mouse::ButtonStateTracker s_mouseTracker;
	static std::unique_ptr<Keyboard> s_keyboard;
	static Keyboard::KeyboardStateTracker s_tracker;
	static std::unique_ptr<GamePad> s_gamepad;
	static std::unique_ptr<GamePad::ButtonStateTracker> s_padTracker;

	namespace DirectInput {
		void UpdateDirectInputJoystick();
	}

	void Initialize(HWND hWnd)
	{
		if (!s_mouse)
			s_mouse = std::make_unique<Mouse>();

		s_mouse->SetWindow(hWnd);

		if (!s_keyboard)
			s_keyboard = std::make_unique<Keyboard>();

		if (!s_gamepad)
			s_gamepad = std::make_unique<GamePad>();

		//		if (!s_padTracker)
		//			s_padTracker = std::make_unique<GamePad::ButtonStateTracker>();

		DXInput::GetInstance()->Setup(::GetModuleHandle(nullptr), hWnd);
	}

	void Update()
	{
		s_tracker.Update(s_keyboard->GetState());
		s_mouseTracker.Update(s_mouse->GetState());
		DXInput::GetInstance()->Update();
		DirectInput::UpdateDirectInputJoystick();
	}

	//Returns true while the user holds down the key identified by name.
	bool GetKey(KeyCode key)
	{
		return s_keyboard->GetState().IsKeyDown(key);
	}

	bool GetKeyDown(KeyCode key)
	{
		return s_tracker.IsKeyPressed(key);
	}

	bool GetKeyUp(KeyCode key)
	{
		return s_tracker.IsKeyReleased(key);
	}


	bool GetMouseDown(MouseButton button)
	{
		bool ret = false;
		switch (button)
		{
			case MouseButton::Left:
				ret = s_mouse->GetState().leftButton;
				break;
			case MouseButton::Right:
				ret = s_mouse->GetState().rightButton;
				break;
			case MouseButton::Middle:
				ret = s_mouse->GetState().middleButton;
				break;
			default: break;
		}

		return ret;
	}

	MouseState GetMouseState(MouseButton button)
	{
		Mouse::ButtonStateTracker::ButtonState state;
		switch (button)
		{
			case MouseButton::Left:
				state = s_mouseTracker.leftButton;
				break;
			case MouseButton::Right:
				state = s_mouseTracker.rightButton;
				break;
			case MouseButton::Middle:
				state = s_mouseTracker.middleButton;
				break;
			default:
				break;
		}

		MouseState ret;
		ret.isPressed = (state == Mouse::ButtonStateTracker::PRESSED);
		ret.isReleased = (state == Mouse::ButtonStateTracker::RELEASED);
		ret.isHeld = (state == Mouse::ButtonStateTracker::HELD);
		ret.isUp = (state == Mouse::ButtonStateTracker::UP);

		return ret;
	}

	// これはスクリーンポジションなのかな...
	tama::Int2 GetMousePosition()
	{
		tama::Int2 p;
		p.x = s_mouse->GetState().x;
		p.y = s_mouse->GetState().y;
		return tama::Int2(p.x, p.y);
	}

	bool hoge()
	{
		auto state = s_gamepad->GetState(0);
		auto state2 = s_gamepad->GetState(1);
		auto state3 = s_gamepad->GetState(2);
		auto state4 = s_gamepad->GetState(3);
		if (state.IsConnected())
		{
			s_padTracker->Update(state);

			if (s_padTracker->a == GamePad::ButtonStateTracker::PRESSED)
				// Take an action when Button A is first pressed, but don't do it again until
				// the button is released and then pressed again
			{
				return true;
			}
		}
		return false;
	}

	namespace XInput
	{
		void GetPadState(GamePad::ButtonStateTracker::ButtonState state, PadButtonState* pState)
		{
			using S = GamePad::ButtonStateTracker::ButtonState;

			pState->isPressed = (state == S::PRESSED);
			pState->isReleased = (state == S::RELEASED);
			pState->isHeld = (state == S::HELD);
			pState->isUp = (state == S::UP);
		}

		PadButtonState GetPadButton(uint32_t index, PadButton button)
		{
			assert(index <= 4);
			PadButtonState ret;

			auto state = s_gamepad->GetState(index);
			if (state.IsConnected())
			{
				s_padTracker->Update(state);

				switch (button)
				{
					case PadButton::A:
						GetPadState(s_padTracker->a, &ret);
						break;
					case PadButton::B:
						GetPadState(s_padTracker->b, &ret);
						break;
					case PadButton::X:
						GetPadState(s_padTracker->x, &ret);
						break;
					case PadButton::Y:
						GetPadState(s_padTracker->y, &ret);
						break;
					case PadButton::Left:
						GetPadState(s_padTracker->dpadLeft, &ret);
						break;
					case PadButton::Right:
						GetPadState(s_padTracker->dpadRight, &ret);
						break;
					case PadButton::Up:
						GetPadState(s_padTracker->dpadUp, &ret);
						break;
					case PadButton::Down:
						GetPadState(s_padTracker->dpadDown, &ret);
						break;
					case PadButton::LeftShoulder:
						GetPadState(s_padTracker->leftShoulder, &ret);
						break;
					case PadButton::RightShoulder:
						GetPadState(s_padTracker->rightShoulder, &ret);
						break;
					default:
						break;
				}
			}
			return ret;
		}
	}

	namespace DirectInput
	{
		struct StickState
		{
			PadButtonState left, right, up, down;
		};
		std::array<StickState, 4> s_stickStates;


		bool IsAnyGamepadActive()
		{
			return DXInput::GetInstance()->GetDetectJoypadCount() > 0;
		}

		PadButtonState _get(const PadButtonState& before, float value, float dest)
		{
			PadButtonState state;
			bool isPushed = value == dest;

			if (isPushed)
			{
				state.isHeld = true;
				if (!before.isHeld && state.isHeld)
					state.isPressed = true;
			}
			else
			{
				state.isReleased = true;
				if (!before.isReleased && state.isReleased)
					state.isUp = true;
			}

			return state;
		}

		void UpdateDirectInputJoystick()
		{
			for (int player = 0; player < 4; player++)
			{
				const float vertical = DXInput::GetInstance()->GetVertical(player);
				const float horizontal = DXInput::GetInstance()->GetHorizontal(player);

				s_stickStates.at(player).up = _get(s_stickStates.at(player).up, vertical, 1.f);
				s_stickStates.at(player).down = _get(s_stickStates.at(player).down, vertical, -1.f);
				s_stickStates.at(player).left = _get(s_stickStates.at(player).left, horizontal, -1.f);
				s_stickStates.at(player).right = _get(s_stickStates.at(player).right, horizontal, 1.f);
			}
		}

		bool GetButtonDown(unsigned player, Button button)
		{
			// 通常のボタン
			if (static_cast<int>(button) <= 9)
			{
				return DXInput::GetInstance()->GetIsJustPressedJoypad(player, static_cast<int>(button));
			}
			// 方向キー
			else
			{
				if (button == Button::Up)
					return s_stickStates.at(player).up.isPressed;
				if (button == Button::Down)
					return s_stickStates.at(player).down.isPressed;
				if (button == Button::Left)
					return s_stickStates.at(player).left.isPressed;
				if (button == Button::Right)
					return s_stickStates.at(player).right.isPressed;
			}
			return false;
		}
	}
}