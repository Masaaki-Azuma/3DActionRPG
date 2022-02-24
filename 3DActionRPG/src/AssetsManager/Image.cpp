#include "Image.h"

#include <assert.h>


std::vector<int> Image::texture_id_list(256, -1);
int Image::texture_couter{ 0 };

void Image::load(const std::string& scene)
{
  
    load_texture("Assets/Image/icon_slime_128x128.png", Texture_icon_slime, scene, { "MapScene" });
    load_texture("Assets/Image/icon_skeleton_128x128.png", Texture_icon_skeleton, scene, { "MapScene" });
    load_texture("Assets/Image/icon_mage_128x128.png", Texture_icon_mage, scene, { "MapScene" });
    load_texture("Assets/Image/icon_mimic_128x128.png", Texture_icon_mimic, scene, { "MapScene" });
    load_texture("Assets/Image/icon_blackKnight_128x128.png", Texture_icon_blackKnight, scene, { "MapScene" });

    load_texture("Assets/Image/mapArea.png", Texture_mapArea, scene, { "MapScene" });
    load_texture("Assets/Image/background_oldmap01_pale.png", Texture_background_oldmap, scene, { "TitleScene", "MapScene" });
    load_texture("Assets/Image/gauge_frame.png", Texture_GaugeFrame, scene, { "MapScene", "BattleScene" });
    load_texture("Assets/Image/gauge_green.png", Texture_GaugeBarGreen, scene, { "MapScene", "BattleScene" });
}

void Image::clear(const std::string& scene)
{
    //全画像メモリを解放
    for (auto& handle : texture_id_list) {
        DxLib::DeleteGraph(handle);
        handle = -1;
    }

    /*
    clear_a_texture(Texture_icon_slime);
    clear_a_texture(Texture_icon_mimic);
    clear_a_texture(Texture_icon_blackKnight);*/
}

const int Image::texture_handle(int texture_id)
{
    return texture_id_list[texture_id];
}

/// <summary>
/// DxLib::DrawGraphFのラッパー関数
/// </summary>
/// <param name="texture_id"></param>
/// <param name="left">画像の左端x</param>
/// <param name="top">画像の上端y</param>
/// <param name="is_trans"></param>
void Image::draw_graph(unsigned int texture_id, float left, float top, bool is_trans)
{
    DxLib::DrawGraphF(left, top, Image::texture_handle(texture_id), is_trans);
}

/// DxLib::DrawRotaGraphFのラッパー関数
/// </summary>
/// <param name="texture_id"></param>
/// <param name="x">画像の中心x</param>
/// <param name="y">画像の中心y画像の中心x</param>
/// <param name="ex_rate"></param>
/// <param name="rota_angle"></param>
/// <param name="is_trans"></param>
void Image::draw_rota_graph(unsigned int texture_id, float x, float y, float ex_rate, float rota_angle, bool is_trans)
{
    DxLib::DrawRotaGraphF(x, y, ex_rate, rota_angle, Image::texture_handle(texture_id), is_trans);
}

int Image::load_texture(const char* file_name, const std::string& scene, std::vector<std::string> use_sceces)
{
    int handle = DxLib::LoadGraph(file_name);
    assert(handle != -1);
    return handle;
}

int Image::load_texture(const char* file_name, int texture_id, const std::string& scene, std::vector<std::string> use_scenes)
{
    /*ロードが必要なシーンか判定*/
    auto scene_itr = std::find(use_scenes.begin(), use_scenes.end(), scene);
    if (scene_itr == use_scenes.end()) return -1; //読み込むシーンではない

    /*実際のロード処理*/
    int handle = DxLib::LoadGraph(file_name);
    //画像が正しく読み込まれていなければ停止
    assert(handle != -1);
    //ハンドル配列メモリが不足したら2倍に拡張
    if (texture_id >= (int)texture_id_list.size()) {
        texture_id_list.resize(texture_id_list.size() * 2);
    }
    //画像IDをインデックスとして画像ハンドルを格納
    texture_id_list[texture_id] = handle;
    return handle;
}

void Image::clear_a_texture(int texture_id)
{
    DxLib::DeleteGraph(texture_id_list[texture_id]);
    texture_id_list[texture_id] = -1;
}
