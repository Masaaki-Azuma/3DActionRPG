#include "TitleScene.h"

#include <DxLib.h>
#include <cassert>

#include "Util/Input.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/PlayerDatabase.h"
#include "AssetsManager/EnemyDatabase.h"
#include "MapScene/MapManager.h"
#include "Screen.h"


enum //メニューの選択状態
{
    GameStart,
    Parameter,
};

static const int NumSelect{ 2 };      //選択肢の数
static const int MenuInterval{ 150 }; //選択肢の高さ間隔

void TitleScene::start(void* data)
{
    Image::load("TitleScene");
    is_end_ = false;
    select_index_ = 0;
}

void TitleScene::update(float delta_time)
{
    //メニュー選択
    select_menu();
}

void TitleScene::draw() const
{
    //背景描画
    Image::draw_graph(Texture_background_oldmap);
    //タイトル描画
    Image::draw_rota_graph(Texture_titleLogo, Screen::Width / 2, 380);
    //メニュー描画
    Image::draw_graph(Texture_textGameStart, 1350, 700);
    Image::draw_graph(Texture_textParameter, 1350, 700 + MenuInterval);
    //カーソル描画
    Image::draw_rota_graph(Texture_cursor, 1250.0f, 730.0f + MenuInterval * select_index_);;
}

bool TitleScene::is_end() const
{
    return is_end_;
}

std::string TitleScene::next() const
{
    switch (select_index_) {
    case GameStart: return "MapScene";
    case Parameter: return "ParameterScene";
    default:
        assert(!"select_index_が予期しない値をとっています");
        return "";
    }
}

void TitleScene::end()
{
    if (next() == "MapScene") {
        //マップデータを作成
        MapManager::GetInstance().generate();
        //プレイヤーの初期パラメータをセット
        PlayerDatabase::GetInstance().set_initial_parameter();
    }
    Image::clear();
}

void* TitleScene::data()
{
    //何も送らない
    return nullptr;
}

void TitleScene::select_menu()
{
    //決定キーで選択肢を決定
    if (Input::get_button_down(PAD_INPUT_1)) {
        is_end_ = true;
    }

    //上下キーで選択肢を変更
    if (Input::get_button_down(PAD_INPUT_DOWN)) {
        select_index_ = (select_index_ + 1) % NumSelect;
    }
    else if (Input::get_button_down(PAD_INPUT_UP)) {
        select_index_ = (select_index_ + NumSelect - 1) % NumSelect;
    }
}
