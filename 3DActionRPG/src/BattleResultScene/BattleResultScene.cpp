#include "BattleResultScene.h"

#include <DxLib.h>
#include <cassert>
#include <numeric>
#include <unordered_map>

#include "Util/Input.h"
#include "AssetsManager/PlayerDatabase.h"
#include "AssetsManager/EnemyDatabase.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/Font.h"
#include "Screen.h"

//TODO:EnemyDatabaseを使って統合せよ
static std::unordered_map<std::string, unsigned int> icon_dictionary =
{
    {"Slime", Texture_icon_slime},
    {"Skeleton", Texture_icon_skeleton},
    {"Mage", Texture_icon_mage},
    {"Mimic", Texture_icon_mimic},
    {"BlackKnight", Texture_icon_blackKnight}
};


BattleResultScene::BattleResultScene():
    e_DB_{EnemyDatabase::GetInstance()}
{
}

void BattleResultScene::start(void* data)
{
    Image::load("BattleResultScene");

    is_end_ = false;
    //データを本来の型へキャストして取得
    result_ = *(static_cast<BattleResultData*>(data));
}

void BattleResultScene::update(float delta_time)
{
    //ForDebug:シーン遷移チート
    if (Input::get_button(PAD_INPUT_4)) { //A
        is_end_ = true;
    }
}

void BattleResultScene::draw() const
{
    int text_color = DxLib::GetColor(0, 0, 0); //黒
    //背景
    Image::draw_rota_graph(Texture_board_battleResult, Screen::Width / 2, Screen::Height / 2);

    //戦果の先頭行高さ
    static const int result_height_base{ 250 };
    //戦果の行間隔
    static const int result_height_interval{ 150 };
    //項目
    Font::draw_right_aligned(770, result_height_base, "戦果：", text_color, Font::japanese_font_50);
    Font::draw_right_aligned(770, 650, "タイムボーナス：", text_color, Font::japanese_font_50);
    Font::draw_right_aligned(770, 840, "合計：", text_color, Font::japanese_font_50);

    //戦果をモンスターごとに表示
    int species_counter = 0;
    for (const auto p : result_.basterd_list) {
        //表示行の高さ
        const float line_height = static_cast<float>(result_height_base + result_height_interval * species_counter);
        //一行分の戦果を表示
        draw_line_result(line_height, text_color, p.first, p.second);
        
        species_counter++;
    }

    //合計ジェム表示
    Image::draw_graph(Texture_gem, 1260, 840);
    Font::draw(1350, 840, "×" + std::to_string(calc_total_gem()), text_color, Font::japanese_font_50);
}

bool BattleResultScene::is_end() const
{
    return is_end_;
}

std::string BattleResultScene::next() const
{
    if (result_.battle_result == "Win")  return "MapScene";
    else if (result_.battle_result == "Lose") return "GameResultScene";

    assert(!"不正なbattle_resultが送られてきています");
    return "";
}

void BattleResultScene::end()
{
    //ジェムを一時入手ジェムとして計上
    PlayerDatabase::GetInstance().add_gem(calc_total_gem());
    Image::clear();
}

void* BattleResultScene::data()
{
    return nullptr;
}

int BattleResultScene::calc_gem(const std::string& enemy, int num_basterd) const
{
    //ある種族の敵一体当たりのドロップジェム数
    int gem_per_enemy = EnemyDatabase::GetInstance().get_drop_gem(enemy);
    //その種族から得られた総ジェム数
    int gem_per_species = gem_per_enemy * num_basterd;
    return gem_per_species;
}

int BattleResultScene::calc_total_gem() const
{
    int total_gem = 0;
    for (auto p : result_.basterd_list) {
        total_gem += calc_gem(p.first, p.second);
    }
    return total_gem;
}

void BattleResultScene::draw_line_result(float height, int text_color, const std::string& e_name, int e_basterd) const
{
    // 敵のアイコン
    Image::draw_graph(icon_dictionary[e_name], 800.0f, height - 50);
    //敵討伐数
    Font::draw(920, height, "×" + std::to_string(e_basterd), text_color, Font::japanese_font_50);
    //右矢印
    Image::draw_graph(Texture_rightArrow, 1130, height);
    //ジェム画像
    Image::draw_graph(Texture_gem, 1260, height);
    //ジェム数
    int gem = calc_gem(e_name, e_basterd);
    Font::draw(1350, height, "×" + std::to_string(gem), text_color, Font::japanese_font_50);
}
