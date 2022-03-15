#ifndef JEWEL_ATTACK_H_
#define JEWEL_ATTACK_H_

#include "AttackCollider.h"
#include <DxLib.h>

class JewelAttack :
    public AttackCollider
{
public:
    JewelAttack(IWorld* world, const Vector3& position, const Vector3& velocity);
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual void react(Actor& other) override;
private:
    void move(float delta_time);

private:
    int mesh_;
    DxLib::COLOR_F color_;
    Vector3 rot_velocity_;
};
#endif//!JEWEL_ATTACK_H_

