#ifndef SKELETON_H_
#define SKELETON_H_

#include "Enemy.h"

class Skeleton :
    public Enemy
{
public:
    struct StateSkeleton : Enemy::State
    {
        static const unsigned int Defense = State::DeriveTop;
    };
public:
    Skeleton(IWorld* world, const Vector3& position, const Vector3& rotation = Vector3{ 0.0f, 180.0f, 0.0f });
    virtual void react(Actor& other) override;
protected:
    virtual void update_state(float delta_time) override;
private:
    void move(float delta_time);
    void attack(float delta_time);
    void damage(float delta_time);
    void dead(float delta_time);
    void defense(float delta_time);
    bool can_be_flinched() const;

    //ForDebug
    virtual void draw_debug() const override;

private:
    //連続ダメージに対するひるみ回数
    int flinch_count_{ 0 };
};
#endif//!SKELETON_H_

