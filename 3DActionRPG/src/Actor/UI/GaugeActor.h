#ifndef GAUGE_ACTOR_H_
#define GAUGE_ACTOR_H_

#include "../Actor.h"
#include "UI/ExtendableBarGauge.h"

class PlayerDatabase;

class GaugeActor :
    public Actor
{
public:
    GaugeActor(float left, float top, int width, int height, int frame_id, int bar_id, int barBG_id = -1);
    virtual void draw_gui() const override;

private:
    ExtendableBarGauge gauge_;

    PlayerDatabase& p_DB_;
};
#endif//!GAUGE_ACTOR_H_

