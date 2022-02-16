#include "ParameterScene.h"

#include <DxLib.h>

#include "Util/Input.h"


void ParameterScene::start(void* data)
{
    is_end_ = false;
}

void ParameterScene::update(float delta_time)
{
    //ForDebug:タイトルシーン遷移チート
    if (Input::get_button_down(PAD_INPUT_4)) {
        is_end_ = true;
    }
}

void ParameterScene::draw() const
{
    //ForDebug:シーン仮表示
    DxLib::DrawString(0, 0, "パラメーターシーン", GetColor(255, 255, 255));

}

bool ParameterScene::is_end() const
{
    return is_end_;
}

std::string ParameterScene::next() const
{
    return "TitleScene";
}

void ParameterScene::end()
{
}

void* ParameterScene::data()
{
    return nullptr;
}
