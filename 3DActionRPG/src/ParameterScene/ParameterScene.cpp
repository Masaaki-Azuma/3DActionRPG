#include "ParameterScene.h"

#include <DxLib.h>
#include <cassert>

#include "Util/Input.h"

enum //パラメータの強化内容
{
    Enhance_hp = 0,
    Enhance_attack,
};

enum //選択可能な項目内容
{
    Menu_CheckParameter = 0,
    Menu_EnhanceParameter,
};

//選択可能なメニュー項目数
const int NumMenu{ 2 };
//強化可能なパラメータの数
const int NumEnhanceableParameter{ 2 };


void ParameterScene::start(void* data)
{
    is_end_ = false;
    selected_parameter_index = 0;
    selected_menu_index = 0;
    menu_state = State::SelectMenu;
}

void ParameterScene::update(float delta_time)
{
    //選択状態によって分岐
    switch (menu_state) {
    case State::SelectMenu:      select_menu();               break;
    case State::CheckParameter:  check_parameter();           break;
    case State::EnhanceParamter: select_enhanced_parameter(); break;
    }

}

void ParameterScene::draw() const
{
    int white = GetColor(255, 255, 255);
    //ForDebug:シーン名仮表示
    DxLib::DrawString(0, 0, "パラメーターシーン", GetColor(255, 255, 255));

    //ForDebug:選択状態仮表示
    std::string state_text;
    switch (menu_state) {
    case State::SelectMenu:      state_text = "メニュー選択状態";     break;
    case State::CheckParameter:  state_text = "パラメーター確認状態"; break;
    case State::EnhanceParamter: state_text = "パラメーター強化状態"; break;
    }
    DxLib::DrawString(0, 20, state_text.c_str(), GetColor(255, 255, 255));

    //ForDebug:メニュー選択肢仮表示
    if (menu_state == State::SelectMenu) {
        DxLib::DrawString(20, 60, "パラメーター確認", white);
        DxLib::DrawString(20, 80, "パラメーター強化", white);
        DxLib::DrawString(0, 60 + 20 * (selected_menu_index), "●", white);


    }
    //ForDebug:増加ボタン仮表示
    if (menu_state == State::EnhanceParamter) {
        DxLib::DrawString(200, 20, "hp増加", white);
        DxLib::DrawString(200, 40, "attack増加", white);
        DxLib::DrawString(180, 20 + 20 * (selected_parameter_index), "●", white);

        DxLib::DrawFormatString(200, 100, white, "hp: %d, attack: %d", p_DB_.get_master_parameter().hp, p_DB_.get_master_parameter().attack);
    }
   

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

void ParameterScene::check_parameter()
{
    //戻るボタンで項目選択へ
    if (Input::get_button_down(PAD_INPUT_2)) {
        menu_state = State::SelectMenu;
    }
}

void ParameterScene::select_menu()
{
    //戻るボタンでタイトルへもどる
    if (Input::get_button_down(PAD_INPUT_2)) {
        is_end_ = true;
        return;
    }

    //決定時に選択されているパラメータを強化
    if (Input::get_button_down(PAD_INPUT_1)) {
        switch (selected_menu_index) {
        case Menu_CheckParameter:   menu_state = State::CheckParameter;  break;
        case Menu_EnhanceParameter: menu_state = State::EnhanceParamter; break;
        default:                    assert(!"selected_menu_index_が予期しない値をとっています"); break;
        }
    }

    //上下キーで選択肢を変更
    if (Input::get_button_down(PAD_INPUT_DOWN)) {
        selected_menu_index = (selected_menu_index + 1) % NumMenu;
    }
    else if (Input::get_button_down(PAD_INPUT_UP)) {
        selected_menu_index = (selected_menu_index + NumMenu - 1) % NumMenu;
    }
}

void ParameterScene::select_enhanced_parameter()
{
    //戻るボタンで項目選択へ
    if (Input::get_button_down(PAD_INPUT_2)) {
        menu_state = State::SelectMenu;
    }

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
