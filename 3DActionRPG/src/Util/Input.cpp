#include "Input.h"

#include <DxLib.h>

int Input::prev_state{ 0 };
int Input::current_state{ 0 };

void Input::init()
{
    prev_state = 0;
    current_state = 0;
}

void Input::update()
{
    prev_state = current_state;
    current_state = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}

bool Input::get_button(int button_id)
{
    return (current_state & button_id);
}

bool Input::get_button_down(int button_id)
{
    return ((current_state & button_id) & ‾(prev_state & button_id)) != 0;
}

bool Input::get_button_up(int button_id)
{
    return (‾(current_state & button_id) & (prev_state & button_id)) != 0;
}
