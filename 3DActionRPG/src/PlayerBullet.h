#pragma once

#include "Actor.h"
#include "DxLib.h"
#include "Image.h"
#include "Screen.h"

class PlayerBullet : public Actor
{
public:
    const float Speed = 25; // 移動速度
    const int VisibleRadius = 16; // 見た目の半径

    //int memory_array[1000000];

    float x; // x座標
    float y; // y座標
    bool isDead = false; // 死亡フラグ

    // コンストラクタ
    PlayerBullet(float x, float y)
    {
        this->tag = "PlauerBullet";
        this->x = x;
        this->y = y;
    }

    ‾PlayerBullet()
    {

    }

    // 更新処理
    void update() override
    {
        // 移動
        x += Speed;

        // 画面外に出たら、死亡フラグを立てる
        if (x - VisibleRadius > Screen::Width)     {
            isDead = true;
        }
    }

    // 描画処理
    void draw() override
    {
        DrawRotaGraphF(x, y, 1, 0, Image::bossImage, TRUE);
    }

};

