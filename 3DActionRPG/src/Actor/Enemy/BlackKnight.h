#ifndef BLACK_KNIGHT_H_
#define BLACK_KNIGHT_H_

#include "Enemy.h"

class BlackKnight :
    public Enemy
{
public:
    struct StateBK : State
    {
        static const unsigned int Tackle = State::DeriveTop;
        static const unsigned int Slash = State::DeriveTop + 1;
    };
public:
    BlackKnight(IWorld* world, const Vector3& position, const Vector3& rotation = Vector3{ 0.0f, 180.0f, 0.0f });
    virtual void react(Actor& other) override;
protected:
    virtual void update_state(float delta_time) override;
private:
    void move(float delta_time);
    void attack(float delta_time);
    void slash(float delta_time);
    void tackle(float delta_time);
    void damage(float delta_time);

    virtual void draw_debug() const override;

private:
    bool has_attacked{ false };
};
#endif//!BLACK_KNIGHT_H_
