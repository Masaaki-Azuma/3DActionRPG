#ifndef MIMIC_H_
#define MIMIC_H_

#include "Enemy.h"
class Mimic :
    public Enemy
{
public:
    struct StateMimic : Enemy::State
    {
        static const int Imitate = State::DeriveTop;
        static const int LongAttack = State::DeriveTop + 1;
        static const int Surprise = State::DeriveTop + 2;
    };

public:
    Mimic(IWorld* world, const Vector3& position, const Vector3& rotation = Vector3{ 0.0f, 180.0f, 0.0f });
    virtual void react_player_attack(Actor& other) override;
    virtual void update_state(float delta_time) override;
private:
    void move(float delta_time);
    void attack(float delta_time);
    void damage(float delta_time);
    void imitate(float delta_time);
    void long_attack(float delta_time);
    void surprise(float delta_time);
    //ForDebug:
    void draw_debug() const override;
};
#endif//!MIMIC_H_

