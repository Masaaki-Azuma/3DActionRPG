#ifndef CNTROLL_UI_H_
#define CNTROLL_UI_H_

#include "Actor.h"

class Image;
class PadInput;

class ControllUI :
    public Actor
{
public:
    ControllUI(IWorld& world, const Vector3& position);
    virtual void update(float delta_time) override;
    virtual void draw() const override;
private:
    void draw_spark() const;

private:
    bool is_pressed_A{ false };
    bool is_pressed_B{ false };

    Image& image_;
    PadInput& input_;
};
#endif//!CNTROLL_UI_H_

