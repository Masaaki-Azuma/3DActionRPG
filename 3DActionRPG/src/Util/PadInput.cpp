#include "PadInput.h"

#include <cmath>


void PadInput::Init()
{
	for (int i = 0; i < 16; i++) {
		prevButtonInput[i] = 0;
		currentButtonInput[i] = 0;
	}
}

void PadInput::Update()
{
	for (int i = 0; i < 16; i++) {
		prevButtonInput[i] = currentButtonInput[i];
	}
	GetJoypadXInputState(DX_INPUT_PAD1, &padInput);

	for (int i = 0; i < 16; i++) {
		currentButtonInput[i] = padInput.Buttons[i];
	}
}

/// <summary>
/// ボタンが押されているか？
/// </summary>
/// <param name="buttonId">XINPUT_BUTTONから始まるボタンID</param>
/// <returns></returns>
bool PadInput::GetButton(int buttonId)
{
	return currentButtonInput[buttonId] == 1;
}

/// <summary>
/// ボタンが押された直後か？
/// </summary>
/// <param name="buttonId">XINPUT_BUTTONから始まるボタンID</param>
/// <returns></returns>
bool PadInput::GetButtonDown(int buttonId)
{
	return currentButtonInput[buttonId] && !prevButtonInput[buttonId];
}

/// <summary>
/// ボタンが離した直後か？
/// </summary>
/// <param name="buttonId">XINPUT_BUTTONから始まるボタンID</param>
/// <returns></returns>
bool PadInput::GetButtonUp(int buttonId)
{
	return prevButtonInput[buttonId] && !currentButtonInput[buttonId];
}

Vector2 PadInput::GetRightStick()
{
	Vector2 direction = Vector2{ static_cast<float>(GetRightStickX()), static_cast<float>(GetRightStickY()) };
	return direction.Normalize();
}

Vector2 PadInput::GetLeftStick()
{
	Vector2 direction = Vector2{ static_cast<float>(GetLeftStickX()), static_cast<float>(GetLeftStickY()) };
	return direction.Normalize();
}

int PadInput::GetRightStickX()
{
	return padInput.ThumbRX;
}

int PadInput::GetRightStickY()
{
	return padInput.ThumbRY;
}

int PadInput::GetLeftStickX()
{
	return padInput.ThumbLX;
}

int PadInput::GetLeftStickY()
{
	return padInput.ThumbLY;
}

float PadInput::GetRightStickAngle()
{
	return static_cast<float>(std::atan2(padInput.ThumbRY, padInput.ThumbRX) * -1);
}

float PadInput::GetLeftStickAngle()
{
	return static_cast<float>(std::atan2(padInput.ThumbLY, padInput.ThumbLX) * -1);
}

//(スティックの指定、得る方向の指定)
float PadInput::GetStickInputDirection(Stick stick, Axis axis)
{
	if (stick == Stick::RIGHT) {
		if (padInput.ThumbRX == 0 && padInput.ThumbRY == 0) { return 0; }

		if (axis == Axis::X)     return std::cosf(PadInput::GetRightStickAngle());
		else if (axis == Axis::Y) return std::sinf(PadInput::GetRightStickAngle());
	}
	else if (stick == Stick::LEFT) {
		if (padInput.ThumbLX == 0 && padInput.ThumbLY == 0) { return 0; }

		if (axis == Axis::X)      return std::cosf(PadInput::GetLeftStickAngle());
		else if (axis == Axis::Y) return std::sinf(PadInput::GetLeftStickAngle());
	}
	
	return 0;
}