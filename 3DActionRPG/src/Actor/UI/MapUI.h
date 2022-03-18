#ifndef MAP_UI_H_
#define MAP_UI_H_

#include "../Actor.h"

class MapUI :
    public Actor
{
public:
    MapUI(IWorld& world, const Vector3& position);
    virtual void update(float delta_time);
    virtual void draw() const override;
private:
    Vector3 calc_icon_pos(const Vector3& target_pos, const Vector3& center_pos, float camera_angleY) const;
    bool is_in_cirlce(const Vector3& icon_pos) const;
};
#endif//!MAP_UI_H_

