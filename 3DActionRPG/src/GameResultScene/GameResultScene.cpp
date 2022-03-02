#include "GameResultScene.h"

#include <DxLib.h>

#include "Util/Input.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/Font.h"
#include "AssetsManager/PlayerDatabase.h"
#include "MapScene/MapManager.h"
#include "Screen.h"

void GameResultScene::start(void* data)
{
    Image::load("GameResultScene");
    is_end_ = false;
}

void GameResultScene::update(float delta_time)
{
    //ForDebug:シーンを抜け出す仮動作
    if (Input::get_button_down(PAD_INPUT_1)) {
        is_end_ = true;
    }
}

void GameResultScene::draw() const
{
    const int text_color = DxLib::GetColor(0, 0, 0);
    //背景描画
    Image::draw_graph(Texture_background_oldmap);
    Image::draw_graph(Texture_board_gameResult);

    int get_gem = p_DB_.get_current_parameter().total_gem;
    int old_possessed_gem = p_DB_.get_master_parameter().total_gem;
    int new_possessed_gem = old_possessed_gem + get_gem;
    static const float height_get_gem{ 320 };
    static const float height_possessed_gem{ 550 };

    Font::draw_right_aligned(730, height_get_gem, "入手ジェム：", text_color, Font::japanese_font_50);
    Image::draw_graph(Texture_gem, 820, height_get_gem);
    Font::draw(910, height_get_gem, "×" + std::to_string(get_gem), text_color, Font::japanese_font_50);

    Font::draw_right_aligned(730, height_possessed_gem, "所持ジェム：", text_color, Font::japanese_font_50);
    Image::draw_graph(Texture_gem, 820, height_possessed_gem);
    Font::draw(910, height_possessed_gem, "×" + std::to_string(old_possessed_gem), text_color, Font::japanese_font_50);
    Image::draw_graph(Texture_rightArrow, 1130, height_possessed_gem);
    Image::draw_graph(Texture_gem, 1220, height_possessed_gem);
    Font::draw(1310, height_possessed_gem, "×" + std::to_string(new_possessed_gem), text_color, Font::japanese_font_50);

    Font::draw_centered(900, "Aボタンで冒険を終える", text_color, Font::japanese_font_35);
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
    //1プレイで得たジェム数を取得
    int gained_gem = p_DB_.get_current_parameter().total_gem;
    //セーブデータに取得ジェムを追加
    p_DB_.add_possessed_jem(gained_gem);
    //マップデータを削除
    MapManager::GetInstance().clear();

    Image::clear();
}

void* GameResultScene::data()
{
    return nullptr;
}
