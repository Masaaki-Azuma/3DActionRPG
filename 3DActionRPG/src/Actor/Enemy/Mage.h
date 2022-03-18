#ifndef MAGE_H_
#define MAGE_H_

#include "Enemy.h"

class Mage :
    public Enemy
{
public:
    Mage(IWorld* world, const Vector3& position, const Vector3& rotation = Vector3{ 0.0f, 0.0f, 0.0f });
    virtual void react(Actor& other) override;
protected:
    virtual void update_state(float delta_time) override;
private:
    void move(float delta_time);
    virtual void draw_debug() const override;
};
#endif//!MAGE_H_

