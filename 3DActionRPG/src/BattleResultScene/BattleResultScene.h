#ifndef BATTLE_RESULT_SCENE_H_
#define BATTLE_RESULT_SCENE_H_

#include "IScene.h"

#include <string>
#include <unordered_map>

#include "Actor/TimerUI.h"

class EnemyDatabase;

//毎バトル後に呼ばれるシーン
class BattleResultScene :
    public IScene
{
public:
    struct BattleResultData
    {
        std::string battle_result;
        std::unordered_map<std::string, int> basterd_list;
        float time{ 0.0f };
    };
public:
    BattleResultScene();
    virtual void start(void* data = nullptr) override;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override;
    virtual std::string next() const override;
    virtual void end() override;
    virtual void* data() override;
private:
    //ジェム計算関数
    int calc_enemy_gem(const std::string& enemy, int num_basterd) const;
    int calc_bonus_gem() const;
    int calc_total_gem() const;
    //描画関数
    void draw_BG() const;
    void draw_items() const;
    void draw_monster_result() const;
    void draw_line_result(float height, int text_color, const std::string& e_name, int e_basterd) const;
    void draw_time_bonus() const;
    void draw_total_result() const;
    //背景スクロール中か？
    bool is_scrolling() const;

private:
    bool is_end_{ true };
    //バトル所要時間
    TimerUI result_time_;
    //経過時間
    TimerUI scene_timer_;
    //シーン背景の高さ
    float BG_height_;


    //バトル勝敗と敵ごとの討伐数データ
    BattleResultData result_;
    //敵データベース参照
    EnemyDatabase& e_DB_;
};
#endif//!BATTLE_RESULT_SCENE_H_

