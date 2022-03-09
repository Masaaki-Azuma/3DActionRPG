#include "BattleResultScene.h"

#include <DxLib.h>
#include <cassert>
#include <numeric>
#include <unordered_map>

#include "Util/Input.h"
#include "AssetsManager/PlayerDatabase.h"
#include "AssetsManager/EnemyDatabase.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/Sound.h"
#include "AssetsManager/Font.h"
#include "Screen.h"

static const unsigned int text_color{ DxLib::GetColor(0, 0, 0) }; //黒
static const int result_left{ 770 };                                              //戦果の左端
static const int result_height_base{ 250 };                                       //戦果の先頭行高さ
static const int result_height_interval{ 150 };                                   //戦果の行間隔
static const int bonus_height{ result_height_base + result_height_interval * 3 }; //タイムボーナス行高さ
static const int total_height{ result_height_base + result_height_interval * 4 }; //トータルジェム行高さ
static const std::vector<float> TimeList{ 0.6f, 1.2f, 1.8f };                     //シーン時間表
static const std::vector<std::pair<int, int>> time_bonus_gem_list{                //バトル時間と獲得ジェムの対応表
    {30, 300},
    {60, 200},
    {90, 100}
};

BattleResultScene::BattleResultScene():
    BG_height_{0.0f},
    e_DB_{EnemyDatabase::GetInstance()}
{
}

void BattleResultScene::start(void* data)
{
    Sound& sound = Sound::GetInstance();
    sound.stop_BGM();
    sound.load("BattleResultScene");
    Image::load("BattleResultScene");

    is_end_ = false;
    scene_timer_.reset();
    BG_height_ = -Screen::Height / 2;

    //データを本来の型へキャストして取得
    result_ = *(static_cast<BattleResultData*>(data));

    //結果表示用のタイマーを設定
    result_time_.reset(result_.time);
    result_time_.set_color(DxLib::GetColor(0, 0, 0));
    result_time_.set_font(Font::japanese_font_50);

    sound.play_SE(SE_BattleResult);
}

void BattleResultScene::update(float delta_time)
{
    static const float ScrollSpeed{ 10.0f };
    //背景を下へスクロール
    BG_height_ = (std::min)(BG_height_ + ScrollSpeed, Screen::Height / 2.0f);
    if (is_scrolling()) return;

    //シーン経過時間更新
    scene_timer_.update(delta_time);
    //全結果表示後Aボタンで終了
    if (Input::get_button(PAD_INPUT_1) && scene_timer_.has_elapsed(TimeList.back())) { //A
        Sound::GetInstance().play_SE(SE_Select);
        is_end_ = true;
    }

}

void BattleResultScene::draw() const
{
    /*背景表示*/
    draw_BG();
    if (is_scrolling()) return;
    /*項目表示*/
    draw_items();

    if (!scene_timer_.has_elapsed(TimeList.at(0))) return;
    if (scene_timer_.has_excessed(TimeList.at(0))) {
        Sound::GetInstance().play_SE(SE_ResultAppear01);
    }

    /*モンスター討伐結果表示*/
    draw_monster_result();

    if (!scene_timer_.has_elapsed(TimeList.at(1))) return;
    if (scene_timer_.has_excessed(TimeList.at(1))) {
        Sound::GetInstance().play_SE(SE_ResultAppear01);
    }

    /*タイムボーナス表示*/
    draw_time_bonus();

    if (!scene_timer_.has_elapsed(TimeList.at(2))) return;
    if (scene_timer_.has_excessed(TimeList.at(2))) {
        Sound::GetInstance().play_SE(SE_ResultAppear02);
    }

    /*合計ジェム表示*/
    draw_total_result();

    //操作誘導表示
    Font::draw_centered(950, "Bボタンで戦闘を終える", text_color, Font::japanese_font_35);

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

//敵討伐によって得られたジェム数を取得
int BattleResultScene::calc_enemy_gem(const std::string& enemy, int num_basterd) const
{
    if (num_basterd == 0) return 0;
    //ある種族の敵一体当たりのドロップジェム数
    int gem_per_enemy = EnemyDatabase::GetInstance().get_drop_gem(enemy);
    //その種族から得られた総ジェム数
    int gem_per_species = gem_per_enemy * num_basterd;
    return gem_per_species;
}

//タイムボーナスによるジェム数を取得
int BattleResultScene::calc_bonus_gem() const
{
    if (result_.battle_result == "Lose") return 0;
    int bonus_gem = 0;
    for (auto p : time_bonus_gem_list) {
        if (result_time_.get_sec() <= p.first) {
            bonus_gem = p.second;
            break;
        }
    }
    return bonus_gem;
}

//総ジェム数を取得
int BattleResultScene::calc_total_gem() const
{
    int total_gem = 0;
    for (auto p : result_.basterd_list) {
        total_gem += calc_enemy_gem(p.first, p.second);
    }
    total_gem += calc_bonus_gem();
    return total_gem;
}

//背景描画
void BattleResultScene::draw_BG() const
{
    Image::draw_rota_graph(Texture_board_battleResult, Screen::Width / 2, BG_height_);

}

//項目描画
void BattleResultScene::draw_items() const
{
    //項目
    Font::draw_right_aligned(result_left, result_height_base, "戦果：", text_color, Font::japanese_font_50);
    Font::draw_right_aligned(result_left, bonus_height, "タイムボーナス：", text_color, Font::japanese_font_50);
    Font::draw_right_aligned(result_left, total_height, "合計：", text_color, Font::japanese_font_50);
}

//敵討伐結果描画
void BattleResultScene::draw_monster_result() const
{
    //TODO:一匹も倒さず敗北した場合0を表示
    //戦果をモンスターごとに表示
    int species_counter = 0;
    for (const auto p : result_.basterd_list) {
        //表示行の高さ
        const float line_height = static_cast<float>(result_height_base + result_height_interval * species_counter);
        //一行分の戦果を表示
        draw_line_result(line_height, text_color, p.first, p.second);

        species_counter++;
    }

    //一体も敵を倒さなかった場合
    if (species_counter == 0) {
        draw_line_result(result_height_base, text_color, "", 0);
    }
}

//ある種族の敵の討伐結果描画
void BattleResultScene::draw_line_result(float height, int text_color, const std::string& e_name, int e_basterd) const
{
    //一体以上敵を倒した場合描画
    if (e_name != "") {
        // 敵のアイコン
        Image::draw_graph(e_DB_.enemy_icon_table(e_name), 800.0f, height - 50);
        //敵討伐数
        Font::draw(920, height, "×" + std::to_string(e_basterd), text_color, Font::japanese_font_50);
        //右矢印
        Image::draw_graph(Texture_rightArrow, 1130, height);
    }
    //ジェム画像
    Image::draw_graph(Texture_gem, 1260, height);
    //ジェム数
    int gem = calc_enemy_gem(e_name, e_basterd);
    Font::draw(1350, height, "×" + std::to_string(gem), text_color, Font::japanese_font_50);
}

//タイムボーナス描画
void BattleResultScene::draw_time_bonus() const
{
    //勝利時のみ所要時間を表示
    if (result_.battle_result == "Win") {
        //経過時間
        static const Vector3 timer_pos{ 850, bonus_height };
        result_time_.draw(timer_pos);
        //右矢印
        Image::draw_graph(Texture_rightArrow, 1130, bonus_height);
    }
    //ジェム画像
    Image::draw_graph(Texture_gem, 1260, bonus_height);
    //ジェム数
    int gem = calc_bonus_gem();
    Font::draw(1350, bonus_height, "×" + std::to_string(gem), text_color, Font::japanese_font_50);
}

//総ジェム描画
void BattleResultScene::draw_total_result() const
{
    Image::draw_graph(Texture_gem, 1260, total_height);
    Font::draw(1350, total_height, "×" + std::to_string(calc_total_gem()), text_color, Font::japanese_font_50);
}

//背景がスクロール中か？
bool BattleResultScene::is_scrolling() const
{
    return BG_height_ < Screen::Height / 2.0f;
}

