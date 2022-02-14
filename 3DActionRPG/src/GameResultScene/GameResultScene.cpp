#include "GameResultScene.h"
#include <DxLib.h>
#include "Input.h"
#include "MapScene/MapManager.h"

void GameResultScene::start(void* data)
{
    is_end_ = false;
}

void GameResultScene::update(float delta_time)
{
    //ForDebug:シーンを抜け出す仮動作
    if (Input::get_button_down(PAD_INPUT_1))     {
        is_end_ = true;
    }
}

void GameResultScene::draw() const
{
    DxLib::DrawString(0, 0, "ゲームリザルトシーン", GetColor(255, 255, 255));

}

bool GameResultScene::is_end() const
{
    return is_end_;
}

std::string GameResultScene::next() const
{
    return "TitleScene";
}

void GameResultScene::end()
{
    //マップデータを削除
    MapManager::GetInstance().clear();
}

void* GameResultScene::data()
{
    return nullptr;
}
