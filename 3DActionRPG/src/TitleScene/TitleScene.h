#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "IScene.h"

class TitleScene :
    public IScene
{
public:
    virtual void start(void* data = nullptr) override;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override;
    virtual std::string next() const override;
    virtual void end() override;
    virtual void* data() override;

private:
    bool is_end_;
};
#endif//!TITLE_SCENE_H_

