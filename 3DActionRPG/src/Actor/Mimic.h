#ifndef MIMIC_H_
#define MIMIC_H_

#include "Actor/Enemy.h"
class Mimic :
    public Enemy
{
public:
    Mimic(IWorld* world, const Vector3& position, const Vector3& rotation = Vector3{ 0.0f, 180.0f, 0.0f });
    virtual void react(Actor& other) override;

    virtual void update_state(float delta_time) override;
};
#endif//!MIMIC_H_

