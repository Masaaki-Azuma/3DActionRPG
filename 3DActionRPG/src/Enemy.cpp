#include "Enemy.h"
#include "GameManager.h"

inline Enemy::Enemy(float x, float y, float z):
    gm{GameManager::GetInstance()}
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Enemy::update()
{
}

void Enemy::draw()
{
}

inline void Enemy::OnCollisionPlayerBulleet(std::shared_ptr<Actor> playerBullet)
{
    life -= 1;
    if (life <= 0) {
        is_dead = true;
    }
}
