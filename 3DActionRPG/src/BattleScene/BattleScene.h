#ifndef BATTLE_SCENE_H
#define BATTLE_SCENE_H

#include "IScene.h"
#include "World.h"
#include "Light.h"
#include "Field.h"
#include "EnemySpawner.h"
#include "BattleResultScene/BattleResultScene.h"
#include "Actor/ExtendableBarGauge.h"
#include "Actor/Timer.h"
#include "Util/Animation/FadeInAnimation.h"

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
    void update_start(float delta_time);
    void update_battle(float delta_time);
    void update_battle_result(float delta_time);

    //戦闘が終了したか？
    bool is_settled() const;
    void spawn_enemy(const std::string& enemy);
   

private:
    //ワールドクラス
    World world_;
    //ライト
    Light light_;
    //敵生成器
    EnemySpawner enemy_spawner_;
    //HPゲージ
    ExtendableBarGauge hp_gauge_;
    //バトル開始テキスト
    FadeInAnimation start_text_;
    //タイマー
    Timer timer_;

    //シーン終了フラグ
    bool is_end_{ false };
    //シーン状態
    int scene_state_;
    //バトルリザルトシーン
    BattleResultScene result_scene_;
    //リザルト
    BattleResultScene::BattleResultData result_;
    //プレイヤーデータ参照
    PlayerDatabase& p_DB_;
};
#endif//!BATTLE_SCENE_H

