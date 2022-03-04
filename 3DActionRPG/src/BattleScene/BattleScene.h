#ifndef BATTLE_SCENE_H
#define BATTLE_SCENE_H

#include "IScene.h"
#include "World.h"
#include "Light.h"
#include "Field.h"
#include "BattleResultScene/BattleResultScene.h"
#include "Actor/ExtendableBarGauge.h"

class PlayerDatabase;

class BattleScene :
    public IScene
{
public:
    BattleScene();
    virtual void start(void* data = nullptr) override;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override;
    virtual std::string next() const override;
    virtual void end() override;
    virtual void* data() override;
private:
    //戦闘が終了したか？
    bool is_settled() const;
    void spawn_enemy(const std::string& enemy);
   

private:
    //ワールドクラス
    World world_;
    //ライト
    Light light_;
    //HPゲージ
    ExtendableBarGauge hp_gauge_;

    //シーン終了フラグ
    bool is_end_{ false };
    //バトルリザルトシーン
    BattleResultScene result_scene_;
    //リザルト
    BattleResultScene::BattleResultData result_;
    //プレイヤーデータ参照
    PlayerDatabase& p_DB_;
};
#endif//!BATTLE_SCENE_H

