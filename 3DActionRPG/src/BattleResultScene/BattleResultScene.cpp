#include "BattleResultScene.h"

#include <DxLib.h>

#include "Util/Input.h"

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
        DxLib::DrawFormatString(0, 50 * (species_counter + 1), GetColor(255, 255, 255), "enemy: %s, num: %d", p.first.c_str(), p.second);
        species_counter++;
    }
}

bool BattleResultScene::is_end() const
{
    return is_end_;
}

std::string BattleResultScene::next() const
{
    return "MapScene";
}

void BattleResultScene::end()
{
}

void* BattleResultScene::data()
{
    return nullptr;
}
