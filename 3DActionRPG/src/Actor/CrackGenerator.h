#ifndef CRACK_GENERATOR_H_
#define CRACK_GENERATOR_H_

#include "Actor.h"
#include "Util/SimpleTimer.h"

class IWorld;

class CrackGenerator :
    public Actor
{
public:
    CrackGenerator(IWorld& world, const Vector3& position, const Vector3& direction);
    virtual void update(float delta_time) override;
private:
    void generate_crack();

private:
    SimpleTimer timer_;
    Vector3 direction_;
    IWorld& world_;
};
#endif//!CRACK_GENERATOR_H_

