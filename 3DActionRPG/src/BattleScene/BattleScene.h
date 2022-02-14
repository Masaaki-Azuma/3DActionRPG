#ifndef BATTLE_SCENE_H
#define BATTLE_SCENE_H

#include "IScene.h"
#include "World.h"

class BattleScene :
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
    void spawn_enemy(const std::string& enemy);

private:
    //ワールドクラス
    World world_;
    //シーン終了フラグ
    bool is_end_{ false };
};
#endif//!BATTLE_SCENE_H

