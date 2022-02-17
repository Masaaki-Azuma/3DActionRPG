#include "TitleScene.h"

#include <DxLib.h>
#include <cassert>

#include "Util/Input.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/PlayerDatabase.h"
#include "AssetsManager/EnemyDatabase.h"
#include "MapScene/MapManager.h"


enum
{
    GameStart = 0,
    Parameter,
};
const int NumSelect{ 2 }; //選択肢の数

void TitleScene::start(void* data)
{
    Image::load("TitleScene");
    is_end_ = false;
    select_index_ = 0;

}

void TitleScene::update(float delta_time)
{
    select_menu();

    //ForDebug:シーンを抜け出す仮動作
    if (Input::get_button_down(PAD_INPUT_1))
    {
        is_end_ = true;
    }
}

void TitleScene::draw() const
{
    Image::draw_graph(Texture_background_oldmap);
    int white = GetColor(255, 255, 255);
    DxLib::DrawString(0, 0, "タイトルシーン", white);
    DxLib::DrawString(20, 40, "ゲームスタート", white);
    DxLib::DrawString(20, 60, "パラメータ", white);

    DxLib::DrawString(0, 40 + 20 * select_index_, "●", white);
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
        //パラメータをロード
        PlayerDatabase::GetInstance().set_initial_parameter();
        EnemyDatabase::GetInstance().load("Assets/Parameters/EnemyParameter.csv");
    }
    else if (next() == "ParameterScene") {
        /*マスターデータを読める状態にしておく*/
    }
    Image::clear();
}

void* TitleScene::data()
{
    return nullptr;
}

void TitleScene::select_menu()
{
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
