#pragma once
#include "Actor.h"

class GameManager;

class Enemy :
    public Actor
{
public:
    Enemy(float x, float y, float z = 0);
    ‾Enemy() = default;

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void OnCollisionPlayerBulleet(std::shared_ptr<Actor> playerBullet);

private:
    GameManager& gm;
    float collisionRadius = 32; // 当たり判定の半径
    bool isDead = false; // 死亡フラグ
    float life = 1; // 耐久力
};

