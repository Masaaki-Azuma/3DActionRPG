#include "ParameterScene.h"

#include <DxLib.h>
#include <cassert>

#include "AssetsManager/Image.h"
#include "AssetsManager/Sound.h"
#include "AssetsManager/Font.h"
#include "Util/Input.h"

//HACK:PlayerDatabaseのenumとまとめられないか？
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
static const int NumMenu{ 2 };
//強化可能なパラメータの数
static const int NumEnhanceableParameter{ 2 };
//メニュー項目高間隔
static const int MenuInterval{ 160 };
//強化ボタン高さ間隔
static const int EnhanceButtonInverval{ 400 };


/*以下PlayerDatabaseのenumに対応*/
//TODO:PlayerDatabaseとプログラム的に統合せよ
//必要ジェム数
const int RequiredGemList[NumEnhanceableParameter]{ 100, 500 };
//パラメータ上昇値
const int RiseValue[NumEnhanceableParameter]{ 100, 20 };

void ParameterScene::start(void* data)
{
    Image::load("ParameterScene");
    Sound::GetInstance().load("ParameterScene");

    is_end_ = false;
    selected_parameter_index = 0;
    selected_menu_index = 0;
    menu_state = State::SelectMenu;

    //体力ゲージ
    hp_gauge_ = ExtendableBarGauge{ 1240, 180, 540, 40, Texture_GaugeFrame, Texture_GaugeBarGreen, Texture_GaugeBarGray };
    hp_gauge_.extend(p_DB_.get_master_parameter().hp, p_DB_.limit_hp());
    hp_gauge_.set_edge_width(3);
    //攻撃力ゲージ
    attack_gauge_ = ExtendableBarGauge{ 1240, 180 + EnhanceButtonInverval, 540, 40, Texture_GaugeFrame, Texture_GaugeBarRed, Texture_GaugeBarGray};
    attack_gauge_.extend(p_DB_.get_master_parameter().attack, p_DB_.limit_attack());
    attack_gauge_.set_edge_width(3);
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
    //背景描画
    Image::draw_graph(Texture_background_parameter);
    //画面左のメニュー
    draw_menu();
    //画面右の詳細
    draw_detail_parameter();
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
    Image::clear();
}

void* ParameterScene::data()
{
    return nullptr;
}

void ParameterScene::check_parameter()
{
    //戻るボタンで項目選択へ
    if (Input::get_button_down(PAD_INPUT_2)) {
        Sound::GetInstance().play_SE(SE_Cancel);
        menu_state = State::SelectMenu;
    }
}

void ParameterScene::select_menu()
{
    //戻るボタンでタイトルへもどる
    if (Input::get_button_down(PAD_INPUT_2)) {
        Sound::GetInstance().play_SE(SE_Cancel);
        is_end_ = true;
        return;
    }

    //メニューを選択
    if (Input::get_button_down(PAD_INPUT_1)) {
        Sound::GetInstance().play_SE(SE_Select);
        switch (selected_menu_index) {
        case Menu_CheckParameter:   menu_state = State::CheckParameter;  break;
        case Menu_EnhanceParameter: menu_state = State::EnhanceParamter; break;
        default:                    assert(!"selected_menu_index_が予期しない値をとっています"); break;
        }
    }

    //上下キーで選択肢を変更
    if (Input::get_button_down(PAD_INPUT_DOWN)) {
        Sound::GetInstance().play_SE(SE_CursorMove);
        selected_menu_index = (selected_menu_index + 1) % NumMenu;
    }
    else if (Input::get_button_down(PAD_INPUT_UP)) {
        Sound::GetInstance().play_SE(SE_CursorMove);
        selected_menu_index = (selected_menu_index + NumMenu - 1) % NumMenu;
    }
}

void ParameterScene::select_enhanced_parameter()
{
    //戻るボタンで項目選択へ
    if (Input::get_button_down(PAD_INPUT_2)) {
        Sound::GetInstance().play_SE(SE_Cancel);
        menu_state = State::SelectMenu;
    }

    //決定時に選択されているパラメータを強化
    if (Input::get_button_down(PAD_INPUT_1)) {
        switch (selected_parameter_index) {
        case Enhance_hp:     try_enhance_hp();    break;
        case Enhance_attack: try_enhance_attack(); break;
        default:             assert(!"selected_parameter_が予期しない値をとっています"); break;
        }
    }
    //上下キーで選択肢を変更
    if (Input::get_button_down(PAD_INPUT_DOWN)) {
        Sound::GetInstance().play_SE(SE_CursorMove);
        selected_parameter_index = (selected_parameter_index + 1) % NumEnhanceableParameter;
    }
    else if (Input::get_button_down(PAD_INPUT_UP)) {
        Sound::GetInstance().play_SE(SE_CursorMove);
        selected_parameter_index = (selected_parameter_index + NumEnhanceableParameter - 1) % NumEnhanceableParameter;
    }

}

//HACK:引数に扱うパラメータを渡して、まとめようよ
void ParameterScene::try_enhance_hp()
{
    int possesed_gem = p_DB_.get_master_parameter().total_gem;
    int required_gem = RequiredGemList[ColHp];
    if (possesed_gem >= required_gem) {
        Sound::GetInstance().play_SE(SE_Enhance);
        p_DB_.use_gem(required_gem);
        p_DB_.enhance_hp(RiseValue[ColHp]);
        hp_gauge_.extend(p_DB_.get_master_parameter().hp, p_DB_.limit_hp());
    }
    else {
        Sound::GetInstance().play_SE(SE_Buzzer);
    }
}

void ParameterScene::try_enhance_attack()
{
    int possesed_gem = p_DB_.get_master_parameter().total_gem;
    int required_gem = RequiredGemList[ColAttack];
    if (possesed_gem >= required_gem) {
        Sound::GetInstance().play_SE(SE_Enhance);
        p_DB_.use_gem(required_gem);
        p_DB_.enhance_attack(RiseValue[ColAttack]);
        attack_gauge_.extend(p_DB_.get_master_parameter().attack, p_DB_.limit_attack());
    }
    else {
        Sound::GetInstance().play_SE(SE_Buzzer);
    }
}

void ParameterScene::draw_menu() const
{
    static const int brown = GetColor(26, 10, 4);
    //メニュー項目描画
    Image::draw_graph(Texture_buttonParameterCheck, 190, 130);
    Image::draw_graph(Texture_buttonParameterEnhance, 190, 130 + MenuInterval);

    //項目概要
    std::string message;
    if (selected_menu_index == Menu_CheckParameter || menu_state == State::CheckParameter)  message = "現在のパラメーターを確認します";
    else if (selected_menu_index == Menu_EnhanceParameter || menu_state == State::EnhanceParamter) message = "ジェムを使ってパラメーターを強化します";
    //文字列を改行
    std::string restructed_message = restruct_string(message);
    DxLib::DrawStringToHandle(190, 550, restructed_message.c_str(), brown, Font::japanese_font_35);
}

void ParameterScene::draw_detail_parameter() const
{
    static const int black = GetColor(0, 0, 0);
    //hpゲージ描画
    DxLib::DrawFormatStringToHandle(1280, 130, black, Font::japanese_font_35, "体力：%d", p_DB_.get_master_parameter().hp);
    hp_gauge_.draw_gui(static_cast<float>(p_DB_.get_master_parameter().hp));

    //攻撃力ゲージ描画
    DxLib::DrawFormatStringToHandle(1280, 130 + EnhanceButtonInverval, black, Font::japanese_font_35, "攻撃力：%d", p_DB_.get_master_parameter().attack);
    attack_gauge_.draw_gui(static_cast<float>(p_DB_.get_master_parameter().attack));

    //所持ジェム描画
    DxLib::DrawFormatStringToHandle(800, 920, black, Font::japanese_font_35, "所持ジェム：　　　　　　　×%d", p_DB_.get_master_parameter().total_gem);
    Image::draw_graph(Texture_gem, 1100, 910);

    //パラメーター強化に関する表示
    if (selected_menu_index == Menu_EnhanceParameter || menu_state == State::EnhanceParamter) {
        //HPパラメータ昇降値
        Image::draw_graph(Texture_buttonEnhance, 850, 280);
        Image::draw_rota_graph(Texture_gem, 1200, 320);
        DxLib::DrawFormatStringToHandle(1230, 305, black, Font::japanese_font_35, "×%d", RequiredGemList[ColHp]);
        Image::draw_rota_graph(Texture_downArrow, 1370, 320);
        Image::draw_rota_graph(Texture_rightArrow, 1480, 320);
        DxLib::DrawFormatStringToHandle(1560, 305, black, Font::japanese_font_35, "HP %d", RiseValue[ColHp]);
        Image::draw_rota_graph(Texture_upArrow, 1730, 320);

        //ATKパラメータ昇降値
        Image::draw_graph(Texture_buttonEnhance, 850, 280 + EnhanceButtonInverval);
        Image::draw_rota_graph(Texture_gem, 1200, 320 + EnhanceButtonInverval);
        DxLib::DrawFormatStringToHandle(1230, 305 + EnhanceButtonInverval, black, Font::japanese_font_35, "×%d", RequiredGemList[ColAttack]);
        Image::draw_rota_graph(Texture_downArrow, 1370, 320 + EnhanceButtonInverval);
        Image::draw_rota_graph(Texture_rightArrow, 1480, 320 + EnhanceButtonInverval);
        DxLib::DrawFormatStringToHandle(1560, 305 + EnhanceButtonInverval, black, Font::japanese_font_35, "ATK %d", RiseValue[ColAttack]);
        Image::draw_rota_graph(Texture_upArrow, 1730, 320 + EnhanceButtonInverval);
    }

    //選択状態に応じた位置にメニューカーソル描画
    switch (menu_state) {
    case State::SelectMenu:      Image::draw_rota_graph(Texture_cursor, 125.0f, 175.0f + MenuInterval * selected_menu_index);               break;
    case State::EnhanceParamter: Image::draw_rota_graph(Texture_cursor, 790.0f, 320.0f + EnhanceButtonInverval * selected_parameter_index); break;
    }
}

std::string ParameterScene::restruct_string(const std::string& str) const
{
    //改行後の文字列
    std::string restructed_message;
    int count = 0;
    const int max_char = 18;
    for (auto c : str) {
        restructed_message.push_back(c);
        count++;
        if (count >= max_char) {
            restructed_message.push_back('¥n');
            count = 0;
        }
    }
    return restructed_message;
}
