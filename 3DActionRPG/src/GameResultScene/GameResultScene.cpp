#include "GameResultScene.h"

#include <DxLib.h>

#include "Util/PadInput.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/Sound.h"
#include "AssetsManager/Font.h"
#include "AssetsManager/PlayerDatabase.h"
#include "MapScene/MapManager.h"
#include "Screen.h"

GameResultScene::GameResultScene():
    image_{image_.GetInstance()}
{
}

void GameResultScene::start(void* data)
{
    image_.load("GameResultScene");
    Sound::GetInstance().load("GameResultScene");
    Sound::GetInstance().stop_BGM();
    is_end_ = false;

    Sound::GetInstance().play_SE(SE_GameResult);
}

void GameResultScene::update(float delta_time)
{
    PadInput& input = PadInput::GetInstance();
    //決定キーでシーン終了
    if (input.GetButtonDown(XINPUT_BUTTON_B)) {
        Sound::GetInstance().play_SE(SE_Select);
        is_end_ = true;
    }
}

void GameResultScene::draw() const
{
    const int text_color = DxLib::GetColor(0, 0, 0);
    //背景描画
    image_.draw_graph(Texture_background_oldmap);
    image_.draw_graph(Texture_board_gameResult);

    int get_gem = p_DB_.get_current_parameter().total_gem;
    int old_possessed_gem = p_DB_.get_master_parameter().total_gem;
    int new_possessed_gem = old_possessed_gem + get_gem;
    static const float height_get_gem{ 320 };
    static const float height_possessed_gem{ 550 };

    Font::draw_right_aligned(730, height_get_gem, "入手ジェム：", text_color, Font::japanese_font_50);
    image_.draw_graph(Texture_gem, 820, height_get_gem);
    Font::draw(910, height_get_gem, "×" + std::to_string(get_gem), text_color, Font::japanese_font_50);

    Font::draw_right_aligned(730, height_possessed_gem, "所持ジェム：", text_color, Font::japanese_font_50);
    image_.draw_graph(Texture_gem, 820, height_possessed_gem);
    Font::draw(910, height_possessed_gem, "×" + std::to_string(old_possessed_gem), text_color, Font::japanese_font_50);
    image_.draw_graph(Texture_rightArrow, 1130, height_possessed_gem);
    image_.draw_graph(Texture_gem, 1220, height_possessed_gem);
    Font::draw(1310, height_possessed_gem, "×" + std::to_string(new_possessed_gem), text_color, Font::japanese_font_50);

    Font::draw_centered(900, "Bボタンで冒険を終える", text_color, Font::japanese_font_35);
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

    image_.clear();
}

void* GameResultScene::data()
{
    return nullptr;
}
