#include "TitleScene.h"
#include <DxLib.h>
#include "Input.h"
#include "MapScene/MapManager.h"

void TitleScene::start(void* data)
{
    is_end_ = false;

}

void TitleScene::update(float delta_time)
{
    //ForDebug:シーンを抜け出す仮動作
    if (Input::get_button_down(PAD_INPUT_1))
    {
        is_end_ = true;
    }
}

void TitleScene::draw() const
{
    DxLib::DrawString(0, 0, "タイトルシーン", GetColor(255, 255, 255));
}

bool TitleScene::is_end() const
{
    return is_end_;
}

std::string TitleScene::next() const
{
    return "MapScene";
}

void TitleScene::end()
{
    //マップデータを作成
    MapManager::GetInstance().generate();
}

void* TitleScene::data()
{
    return nullptr;
}
