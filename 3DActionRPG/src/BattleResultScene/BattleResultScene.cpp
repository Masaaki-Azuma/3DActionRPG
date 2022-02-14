#include "BattleResultScene.h"

#include <DxLib.h>

#include "Util/Input.h"

void BattleResultScene::start(void* data)
{
    is_end_ = false;
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
