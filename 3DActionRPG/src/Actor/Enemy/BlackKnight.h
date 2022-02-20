#ifndef BLACK_KNIGHT_H_
#define BLACK_KNIGHT_H_

#include "Enemy.h"

class BlackKnight :
    public Enemy
{
public:
    BlackKnight(IWorld* world, const Vector3& position, const Vector3& rotation = Vector3{ 0.0f, 180.0f, 0.0f });
    //virtual void react(Actor& other) override;
protected:
    virtual void update_state(float delta_time) override;
private:
};
#endif//!BLACK_KNIGHT_H_
