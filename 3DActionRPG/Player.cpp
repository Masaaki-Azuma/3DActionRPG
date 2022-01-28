#include "Player.h"
#include "Input.h"
#include "Image.h"
#include "Game.h"
#include "PlayerBullet.h"
#include "GameManager.h"

const float MoveSpeed{ 6 };

Player::Player(float x, float y):
    gm{GameManager::GetInstance()}
{
}

void Player::update()
{
    float vx = 0; // x方向移動速度
    float vy = 0; // y方向移動速度

    if (Input::get_button(PAD_INPUT_LEFT))     {
        vx = -MoveSpeed; // 左
    }
    else if (Input::get_button(PAD_INPUT_RIGHT))     {
        vx = MoveSpeed; // 右
    }
    if (Input::get_button(PAD_INPUT_UP))     {
        vy = -MoveSpeed; // 上
    }
    else if (Input::get_button(PAD_INPUT_DOWN))     {
        vy = MoveSpeed; // 下
    }

    // 実際に位置を動かす
    x += vx;
    y += vy;

    if (Input::get_button_down(PAD_INPUT_10)) {
        gm.player_bullets.push_back(std::make_shared<PlayerBullet>(x, y));
    }
}

void Player::draw()
{
    DrawRotaGraphF(x, 200, 1.0f, 0.0f, Image::bossImage, true);
}
