#ifndef CRACK_ATTACK_
#define CRACK_ATTACK_

#include "Actor/Actor.h"
#include "Util/SimpleTimer.h"

//地割れ攻撃
class CrackAttack :
    public Actor
{
public:
    CrackAttack(IWorld* world, const Vector3& position);
    virtual void update(float delta_time) override;
    virtual void draw() const override;

private:
    SimpleTimer timer_;
    int mesh_;
};
#endif//!CRACK_ATTACK_
