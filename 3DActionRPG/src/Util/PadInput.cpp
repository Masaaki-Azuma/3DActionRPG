#include "PadInput.h"

#include <algorithm>
#include <cmath>

static const float StickThreshold{ 0.35f };

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
	//-1〜1にクランプ
	float RX = (std::min)((std::max)(GetRightStickX() / 32767.0f, -1.0f), 1.0f);
	float RY = (std::min)((std::max)(GetRightStickY() / 32767.0f, -1.0f), 1.0f);
	//スティックの傾きが一定以下なら0とみなす
	if (std::fabsf(RX) < StickThreshold) RX = 0.0f;
	if (std::fabsf(RY) < StickThreshold) RY = 0.0f;
	return Vector2{ RX, RY };
}

Vector2 PadInput::GetLeftStick()
{
	//-1〜1にクランプ
	float LX = (std::min)((std::max)(GetLeftStickX() / 32767.0f, -1.0f), 1.0f);
	float LY = (std::min)((std::max)(GetLeftStickY() / 32767.0f, -1.0f), 1.0f);
	//スティックの傾きが一定以下なら0とみなす
	if (std::fabsf(LX) < StickThreshold) LX = 0.0f;
	if (std::fabsf(LY) < StickThreshold) LY = 0.0f;
	return Vector2{ LX, LY };
	
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

/// <param name="stick">スティックの指定</param>
/// <param name="axis">得る方向の指定</param>
/// <returns></returns>
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