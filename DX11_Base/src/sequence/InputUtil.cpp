#include "InputUtil.h"
#include "../System/Input.h"

InputUtil::Mode InputUtil::mode;
std::unordered_map<int, int> InputUtil::keyMap;

void InputUtil::Init()
{
	if (Input::DirectInput::IsAnyGamepadActive())
	{
		mode = Mode::Gamepad;
		keyMap.emplace(Button::Up, static_cast<int>(Input::DirectInput::Button::Up));
		keyMap.emplace(Button::Down, static_cast<int>(Input::DirectInput::Button::Down));
		keyMap.emplace(Button::Left, static_cast<int>(Input::DirectInput::Button::Left));
		keyMap.emplace(Button::Right, static_cast<int>(Input::DirectInput::Button::Right));

		keyMap.emplace(Button::One, static_cast<int>(Input::DirectInput::Button::Z));
		keyMap.emplace(Button::Two, static_cast<int>(Input::DirectInput::Button::X));
		keyMap.emplace(Button::Three, static_cast<int>(Input::DirectInput::Button::C));
		keyMap.emplace(Button::Four, static_cast<int>(Input::DirectInput::Button::V));

		keyMap.emplace(Button::Start, static_cast<int>(Input::DirectInput::Button::Select));
		keyMap.emplace(Button::Select, static_cast<int>(Input::DirectInput::Button::Start));
	}
#if defined _DEBUG | 1
	else
	{
		mode = Mode::Keyboard;
		keyMap.emplace(Button::Up, static_cast<int>(Input::KeyCode::Up));
		keyMap.emplace(Button::Down, static_cast<int>(Input::KeyCode::Down));
		keyMap.emplace(Button::Left, static_cast<int>(Input::KeyCode::Left));
		keyMap.emplace(Button::Right, static_cast<int>(Input::KeyCode::Right));

		keyMap.emplace(Button::One, static_cast<int>(Input::KeyCode::S));
		keyMap.emplace(Button::Two, static_cast<int>(Input::KeyCode::X));
		keyMap.emplace(Button::Three, static_cast<int>(Input::KeyCode::Z));
		keyMap.emplace(Button::Four, static_cast<int>(Input::KeyCode::A));

		keyMap.emplace(Button::Start, static_cast<int>(Input::KeyCode::Space));
		keyMap.emplace(Button::Select, static_cast<int>(Input::KeyCode::P));
	}
#endif
}

bool InputUtil::GetButtonDown(Button button)
{
	if (mode == Mode::Gamepad)
	{
		return Input::DirectInput::GetButtonDown(0, static_cast<Input::DirectInput::Button>(keyMap.at(button)));
	}
	else /*if (mode == Mode::Keyboard)*/
	{
		return Input::GetKeyDown(static_cast<Input::KeyCode>(keyMap.at(button)));
	}
}