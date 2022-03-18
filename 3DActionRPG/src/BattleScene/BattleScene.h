#ifndef BATTLE_SCENE_H
#define BATTLE_SCENE_H

#include "IScene.h"
#include "World.h"
#include "Light.h"
#include "Field.h"
#include "TimerUI.h"
#include "BattleResultScene/BattleResultScene.h"
#include "Animation/FadeInAnimation.h"

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
    //バトル開始テキスト
    FadeInAnimation start_text_;
    //タイマー
    TimerUI timer_;

    //シーン終了フラグ
    bool is_end_{ false };
    //シーン状態
    int scene_state_{ -1 };
    //バトルリザルトシーン
    BattleResultScene result_scene_;
    //リザルト
    BattleResultScene::BattleResultData result_;

    //プレイヤーデータ参照
    PlayerDatabase& p_DB_;
};
#endif//!BATTLE_SCENE_H

