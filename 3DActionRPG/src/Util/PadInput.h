#ifndef PAD_INPUT_H_
#define PAD_INPUT_H_

#include "Singleton.h"

#include <array>
#include <DxLib.h>

enum class Stick
{
	RIGHT,
	LEFT
};

enum class Axis
{
	X,
	Y
};

#include "Vector2.h"

class PadInput : public Singleton<PadInput>
{
public:
	void Init();
	void Update();
	bool GetButton(int buttonId);
	bool GetButtonDown(int buttonId);
	bool GetButtonUp(int buttonId);
	Vector2 GetRightStick();
	Vector2 GetLeftStick();
	int GetRightStickX();
	int GetRightStickY();
	int GetLeftStickX();
	int GetLeftStickY();
	//右をx軸正方向、下をy軸正方向とするラジアン
	float GetRightStickAngle();
	//右をx軸正方向、下をy軸正方向とするラジアン
	float GetLeftStickAngle();
	float GetStickInputDirection(Stick stick, Axis axis);
private:
	XINPUT_STATE padInput;
	std::array<unsigned char, 16> prevButtonInput;
	std::array<unsigned char, 16> currentButtonInput;

private:
	PadInput() = default;
	‾PadInput() = default;
private:
	friend class Singleton<PadInput>;
};
#endif//!PAD_INPUT_H_
