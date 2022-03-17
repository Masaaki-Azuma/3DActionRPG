#ifndef CNTROLL_UI_H_
#define CNTROLL_UI_H_

#include "Actor.h"
class ControllUI :
    public Actor
{
public:
    ControllUI(const Vector3& position);
    virtual void update(float delta_time) override;
    virtual void draw() const override;
};
#endif//!CNTROLL_UI_H_

