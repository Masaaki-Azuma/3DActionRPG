#include "BattleResultScene.h"

#include <DxLib.h>
#include <cassert>

#include "Util/Input.h"
#include "AssetsManager/EnemyDatabase.h"

void BattleResultScene::start(void* data)
{
    is_end_ = false;
    //データを本来の型へキャストして取得
    result_ = *(static_cast<BattleResultData*>(data));

}

void BattleResultScene::update(float delta_time)
{
    //ForDebug:シーン遷移チート
    if (Input::get_button(PAD_INPUT_4)) { //A
        is_end_ = true;
    }
}

void BattleResultScene::draw() const
{

    //ForDebug:シーン仮表示
    DxLib::DrawString(0, 0, "バトルリザルトシーン", GetColor(255, 255, 255));

    //ForDebug:討伐数
    int species_counter = 0;
    for (const auto p : result_.basterd_list) {
        int gem = calc_gem(p.first, p.second);

        DxLib::DrawFormatString(0, 50 * (species_counter + 1), GetColor(255, 255, 255), "enemy: %s, num: %d, gem: %d", p.first.c_str(), p.second, gem);
        species_counter++;
    }
}

bool BattleResultScene::is_end() const
{
    return is_end_;
}

std::string BattleResultScene::next() const
{
    if (result_.battle_result == "Win")  return "MapScene";
    else if (result_.battle_result == "Lose") return "GameResultScene";

    assert(!"不正なbattle_resultが送られてきています");
    return "";
}

void BattleResultScene::end()
{
}

void* BattleResultScene::data()
{
    return nullptr;
}

int BattleResultScene::calc_gem(const std::string& enemy, int num_basterd) const
{
    //ある種族の敵一体当たりのドロップジェム数
    int gem_per_enemy = EnemyDatabase::GetInstance().get_drop_gem(enemy);
    //その種族から得られた総ジェム数
    int gem_per_species = gem_per_enemy * num_basterd;
    return gem_per_species;
}
