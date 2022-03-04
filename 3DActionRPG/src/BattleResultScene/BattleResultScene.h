#ifndef BATTLE_RESULT_SCENE_H_
#define BATTLE_RESULT_SCENE_H_

#include "IScene.h"

#include <string>
#include <unordered_map>

#include "Actor/Timer.h"

class EnemyDatabase;

class BattleResultScene :
    public IScene
{
public:
    struct BattleResultData
    {
        std::string battle_result;
        std::unordered_map<std::string, int> basterd_list;
        float time;
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
    int calc_enemy_gem(const std::string& enemy, int num_basterd) const;
    int calc_bonus_gem() const;
    int calc_total_gem() const;
    void draw_line_result(float height, int text_color, const std::string& e_name, int e_basterd) const;

private:
    bool is_end_{ true };
    //経過時間
    Timer timer_;
    //バトル勝敗と敵ごとの討伐数データ
    BattleResultData result_;
    //敵データベース参照
    EnemyDatabase& e_DB_;
};
#endif//!BATTLE_RESULT_SCENE_H_

