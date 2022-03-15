#include "ParameterScene.h"

#include <DxLib.h>
#include <cassert>

#include "AssetsManager/Image.h"
#include "AssetsManager/Sound.h"
#include "AssetsManager/Font.h"
#include "AssetsManager/PlayerDatabase.h"
#include "Util/PadInput.h"

enum //強化するパラメーターの選択状態
{
    Enhance_hp = 0,
    Enhance_attack,
};

enum //メニューの選択状態
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

enum //各種パラメーターを表す配列インデックス
{
    Col_Hp,
    Col_Attack,
};
//必要ジェム数
const int RequiredGemList[NumEnhanceableParameter]{ 100, 500 };
//パラメータ上昇値
const int RiseValue[NumEnhanceableParameter]{ 100, 10 };

ParameterScene::ParameterScene():
    p_DB_{PlayerDatabase::GetInstance()},
    input_{PadInput::GetInstance()},
    image_{image_.GetInstance()}
{
}

void ParameterScene::start(void* data)
{
    image_.load("ParameterScene");
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
    image_.draw_graph(Texture_background_parameter);
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
    image_.clear();
}

void* ParameterScene::data()
{
    return nullptr;
}

void ParameterScene::check_parameter()
{
    //戻るボタンで項目選択へ
    if (input_.GetButtonDown(XINPUT_BUTTON_A)) {
        Sound::GetInstance().play_SE(SE_Cancel);
        menu_state = State::SelectMenu;
    }
}

void ParameterScene::select_menu()
{
    //戻るボタンでタイトルへもどる
    if (input_.GetButtonDown(XINPUT_BUTTON_A)) {
        Sound::GetInstance().play_SE(SE_Cancel);
        is_end_ = true;
        return;
    }

    //メニューを選択
    if (input_.GetButtonDown(XINPUT_BUTTON_B)) {
        Sound::GetInstance().play_SE(SE_Select);
        switch (selected_menu_index) {
        case Menu_CheckParameter:   menu_state = State::CheckParameter;  break;
        case Menu_EnhanceParameter: menu_state = State::EnhanceParamter; break;
        default:                    assert(!"selected_menu_index_が予期しない値をとっています"); break;
        }
    }

    //上下キーで選択肢を変更
    if (input_.GetButtonDown(XINPUT_BUTTON_DPAD_DOWN)) {
        Sound::GetInstance().play_SE(SE_CursorMove);
        selected_menu_index = (selected_menu_index + 1) % NumMenu;
    }
    else if (input_.GetButtonDown(XINPUT_BUTTON_DPAD_UP)) {
        Sound::GetInstance().play_SE(SE_CursorMove);
        selected_menu_index = (selected_menu_index + NumMenu - 1) % NumMenu;
    }
}

void ParameterScene::select_enhanced_parameter()
{
    //戻るボタンで項目選択へ
    if (input_.GetButtonDown(XINPUT_BUTTON_A)) {
        Sound::GetInstance().play_SE(SE_Cancel);
        menu_state = State::SelectMenu;
    }

    //決定時に選択されているパラメータを強化
    if (input_.GetButtonDown(XINPUT_BUTTON_B)) {
        switch (selected_parameter_index) {
        case Enhance_hp:     try_enhance_hp();    break;
        case Enhance_attack: try_enhance_attack(); break;
        default:             assert(!"selected_parameter_が予期しない値をとっています"); break;
        }
    }
    //上下キーで選択肢を変更
    if (input_.GetButtonDown(XINPUT_BUTTON_DPAD_DOWN)) {
        Sound::GetInstance().play_SE(SE_CursorMove);
        selected_parameter_index = (selected_parameter_index + 1) % NumEnhanceableParameter;
    }
    else if (input_.GetButtonDown(XINPUT_BUTTON_DPAD_UP)) {
        Sound::GetInstance().play_SE(SE_CursorMove);
        selected_parameter_index = (selected_parameter_index + NumEnhanceableParameter - 1) % NumEnhanceableParameter;
    }

}

void ParameterScene::try_enhance_hp()
{
    int possesed_gem = p_DB_.get_master_parameter().total_gem;
    int required_gem = RequiredGemList[Col_Hp];
    //ジェムが足りており、かつ体力が最大強化されていなければ強化
    if (possesed_gem >= required_gem && p_DB_.get_master_parameter().hp < p_DB_.limit_hp()) {
        Sound::GetInstance().play_SE(SE_Enhance);
        p_DB_.use_gem(required_gem);
        p_DB_.enhance_hp(RiseValue[Col_Hp]);
        hp_gauge_.extend(p_DB_.get_master_parameter().hp, p_DB_.limit_hp());
    }
    else {
        Sound::GetInstance().play_SE(SE_Buzzer);
    }
}

void ParameterScene::try_enhance_attack()
{
    int possesed_gem = p_DB_.get_master_parameter().total_gem;
    int required_gem = RequiredGemList[Col_Attack];
    //ジェムが足りており、かつ体力が最大強化されていなければ強化
    if (possesed_gem >= required_gem && p_DB_.get_master_parameter().attack < p_DB_.limit_attack()) {
        Sound::GetInstance().play_SE(SE_Enhance);
        p_DB_.use_gem(required_gem);
        p_DB_.enhance_attack(RiseValue[Col_Attack]);
        attack_gauge_.extend(p_DB_.get_master_parameter().attack, p_DB_.limit_attack());
    }
    else {
        Sound::GetInstance().play_SE(SE_Buzzer);
    }
}

//画面左のメニュー
void ParameterScene::draw_menu() const
{
    //テキスト色
    static const int text_color = GetColor(26, 10, 4); //こげ茶
    //メニュー項目描画
    image_.draw_graph(Texture_buttonParameterCheck, 190, 130);
    image_.draw_graph(Texture_buttonParameterEnhance, 190, 130 + MenuInterval);

    //項目概要
    std::string message;
    if      (selected_menu_index == Menu_CheckParameter)   message = "現在のパラメーターを確認します";
    else if (selected_menu_index == Menu_EnhanceParameter) message = "ジェムを使ってパラメーターを強化します";
    //文字列を改行
    std::string restructed_message = restruct_string(message);
    DxLib::DrawStringToHandle(190, 550, restructed_message.c_str(), text_color, Font::japanese_font_35);
}

void ParameterScene::draw_detail_parameter() const
{
    static const int text_color = GetColor(0, 0, 0);
    //hpゲージ描画
    DxLib::DrawFormatStringToHandle(1280, 130, text_color, Font::japanese_font_35, "体力：%d", p_DB_.get_master_parameter().hp);
    hp_gauge_.draw_gui(static_cast<float>(p_DB_.get_master_parameter().hp));

    //攻撃力ゲージ描画
    DxLib::DrawFormatStringToHandle(1280, 130 + EnhanceButtonInverval, text_color, Font::japanese_font_35, "攻撃力：%d", p_DB_.get_master_parameter().attack);
    attack_gauge_.draw_gui(static_cast<float>(p_DB_.get_master_parameter().attack));

    //所持ジェム描画
    DxLib::DrawFormatStringToHandle(800, 920, text_color, Font::japanese_font_35, "所持ジェム：　　　　　　　×%d", p_DB_.get_master_parameter().total_gem);
    image_.draw_graph(Texture_gem, 1100, 910);

    //パラメーター強化に関する表示
    if (selected_menu_index == Menu_EnhanceParameter || menu_state == State::EnhanceParamter) {
        //体力強化メニュー
        draw_hp_enhance_menu(text_color);
        //攻撃力強化メニュー
        draw_attack_enhance_menu(text_color);
    }

    //選択状態に応じた位置にメニューカーソル描画
    switch (menu_state) {
    case State::SelectMenu:      image_.draw_rota_graph(Texture_cursor, 125.0f, 175.0f + MenuInterval * selected_menu_index);               break;
    case State::EnhanceParamter: image_.draw_rota_graph(Texture_cursor, 790.0f, 320.0f + EnhanceButtonInverval * selected_parameter_index); break;
    }
}

void ParameterScene::draw_hp_enhance_menu(int text_color) const
{
    image_.draw_graph(Texture_buttonEnhance, 850, 280);
    image_.draw_rota_graph(Texture_gem, 1200, 320);

    Font::draw(1230, 305, "×" + std::to_string(RequiredGemList[Col_Hp]), text_color, Font::japanese_font_35);
    image_.draw_rota_graph(Texture_downArrow, 1370, 320);
    image_.draw_rota_graph(Texture_rightArrow, 1480, 320);
    Font::draw(1560, 305, "HP " + std::to_string(RiseValue[Col_Hp]), text_color, Font::japanese_font_35);
    image_.draw_rota_graph(Texture_upArrow, 1730, 320);
}

void ParameterScene::draw_attack_enhance_menu(int text_color) const
{
    image_.draw_graph(Texture_buttonEnhance, 850, 280 + EnhanceButtonInverval);
    image_.draw_rota_graph(Texture_gem, 1200, 320 + EnhanceButtonInverval);
    Font::draw(1230, 305 + EnhanceButtonInverval, "×" + std::to_string(RequiredGemList[Col_Attack]), text_color, Font::japanese_font_35);
    image_.draw_rota_graph(Texture_downArrow, 1370, 320 + EnhanceButtonInverval);
    image_.draw_rota_graph(Texture_rightArrow, 1480, 320 + EnhanceButtonInverval);
    Font::draw(1560, 305 + EnhanceButtonInverval, "ATK " + std::to_string(RiseValue[Col_Attack]), text_color, Font::japanese_font_35);
    image_.draw_rota_graph(Texture_upArrow, 1730, 320 + EnhanceButtonInverval);
}

std::string ParameterScene::restruct_string(const std::string& str) const
{
    //改行後の文字列
    std::string restructed_message;
    int count = 0;
    static const int MaxChar = 18;
    //半角文字MaxChar文字ごとに改行
    for (auto c : str) {
        restructed_message.push_back(c);
        count++;
        if (count >= MaxChar) {
            restructed_message.push_back('¥n');
            count = 0;
        }
    }
    return restructed_message;
}
