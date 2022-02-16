#include "ParameterScene.h"

#include <DxLib.h>
#include <cassert>

#include "Util/Input.h"

enum //パラメータの強化内容
{
    Enhance_hp = 0,
    Enhance_attack,
};

//強化可能なパラメータの数
const int NumEnhanceableParameter{ 2 };


void ParameterScene::start(void* data)
{
    is_end_ = false;
    selected_parameter_index = 0;
    selected_icon_index = 0;
}

void ParameterScene::update(float delta_time)
{
    //強化パラメータの選択
    select_enhanced_parameter();

    //ForDebug:タイトルシーン遷移チート
    if (Input::get_button_down(PAD_INPUT_4)) {
        is_end_ = true;
    }
}

void ParameterScene::draw() const
{
    //ForDebug:シーン仮表示
    int white = GetColor(255, 255, 255);
    DxLib::DrawString(0, 0, "パラメーターシーン", GetColor(255, 255, 255));

    //ForDebug:増加ボタン仮表示
    DxLib::DrawString(200, 20, "hp増加", white);
    DxLib::DrawString(200, 40, "attack増加", white);
    DxLib::DrawString(180, 20 + 20 * (selected_parameter_index), "●", white);

    DxLib::DrawFormatString(200, 100, white, "hp: %d, attack: %d", p_DB_.get_master_parameter().hp, p_DB_.get_master_parameter().attack);

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

void ParameterScene::select_enhanced_parameter()
{
    //決定時に選択されているパラメータを強化
    if (Input::get_button_down(PAD_INPUT_1)) {
        switch (selected_parameter_index) {
        case Enhance_hp:     p_DB_.enhance_hp(100);    break;
        case Enhance_attack: p_DB_.enhance_attack(20); break;
        default:             assert(!"selected_parameter_が予期しない値をとっています"); break;
        }
    }

    //上下キーで選択肢を変更
    if (Input::get_button_down(PAD_INPUT_DOWN)) {
        selected_parameter_index = (selected_parameter_index + 1) % NumEnhanceableParameter;
    }
    else if (Input::get_button_down(PAD_INPUT_UP)) {
        selected_parameter_index = (selected_parameter_index + NumEnhanceableParameter - 1) % NumEnhanceableParameter;
    }

}
