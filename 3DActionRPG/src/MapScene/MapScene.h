#ifndef MAP_SCENE_H_
#define MAP_SCENE_H_

#include "IScene.h"

#include <string>

#include "Util/SlideInAnimation.h"
#include "Actor/ExtendableBarGauge.h"


class MapManager;
class PlayerDatabase;

class MapScene :
    public IScene
{
public:
    MapScene();
    ‾MapScene() = default;
    virtual void start(void* data = nullptr) override;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override;
    virtual std::string next() const override;
    virtual void end() override;
    virtual void* data() override;
private:
    //ForDebug:
    void restart_area();

private:
    bool is_end_;
    ExtendableBarGauge hp_gauge_;
    SlideInAnimation encount_text_;

    MapManager& map_;
    PlayerDatabase& p_DB_;
    
};
#endif//!MAP_SCENE_H_

