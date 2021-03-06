#include "Image.h"

#include <assert.h>


std::vector<int> texture_id_list(256, -1);

void Image::load(const std::string& scene)
{
    load_texture("Assets/Image/title_logo.png", Texture_titleLogo, scene, { "TitleScene" });
    load_texture("Assets/Image/text_gameStart.png", Texture_textGameStart, scene, { "TitleScene" });
    load_texture("Assets/Image/text_parameter.png", Texture_textParameter, scene, { "TitleScene" });
    load_texture("Assets/Image/background_oldmap01_pale.png", Texture_background_oldmap, scene, { "TitleScene", "MapScene", "GameResultScene" });
    load_texture("Assets/Image/cursor.png", Texture_cursor, scene, { "TitleScene", "ParameterScene", "MapScene" });
  
    load_texture("Assets/Image/board_battleResult.png", Texture_board_battleResult, scene, { "BattleResultScene" });
    load_texture("Assets/Image/board_gameResult.png", Texture_board_gameResult, scene, { "GameResultScene" });

    load_texture("Assets/Image/portion_green.png", Texture_hpPortion, scene, { "MapScene" });

    load_texture("Assets/Image/icon_player.png", Texture_icon_player, scene, { "MapScene" });
    load_texture("Assets/Image/icon_slime_128x128.png", Texture_icon_slime, scene, { "MapScene", "BattleResultScene" });
    load_texture("Assets/Image/icon_skeleton_128x128.png", Texture_icon_skeleton, scene, { "MapScene", "BattleResultScene" });
    //load_texture("Assets/Image/icon_mage_128x128.png", Texture_icon_mage, scene, { "MapScene", "BattleResultScene" });
    load_texture("Assets/Image/icon_mimic_128x128.png", Texture_icon_mimic, scene, { "MapScene", "BattleResultScene" });
    load_texture("Assets/Image/icon_blackKnight_128x128.png", Texture_icon_blackKnight, scene, { "MapScene", "BattleResultScene" });
    load_texture("Assets/Image/icon_chest_128x128.png", Texture_icon_chest, scene, { "MapScene", "BattleResultScene" });

    load_texture("Assets/Image/silhouette_slime_128x128.png", Texture_silhouette_slime, scene, { "MapScene" });
    load_texture("Assets/Image/silhouette_skeleton_128x128.png", Texture_silhouette_skeleton, scene, { "MapScene" });
    //load_texture("Assets/Image/silhouette_mage_128x128.png", Texture_silhouette_mage, scene, { "MapScene" });
    load_texture("Assets/Image/silhouette_mimic_128x128.png", Texture_silhouette_mimic, scene, { "MapScene" });
    load_texture("Assets/Image/silhouette_blackKnight_128x128.png", Texture_silhouette_blackKnight, scene, { "MapScene" });

    load_texture("Assets/Image/mapArea.png", Texture_mapArea, scene, { "MapScene" });

    load_texture("Assets/Image/gauge_frame.png", Texture_GaugeFrame, scene, { "ParameterScene", "MapScene", "BattleScene" });
    load_texture("Assets/Image/gauge_green.png", Texture_GaugeBarGreen, scene, { "ParameterScene", "MapScene", "BattleScene" });
    load_texture("Assets/Image/gauge_red.png", Texture_GaugeBarRed, scene, { "ParameterScene", "MapScene", "BattleScene" });
    load_texture("Assets/Image/gauge_gray.png", Texture_GaugeBarGray, scene, { "ParameterScene", "MapScene", "BattleScene" });

    load_texture("Assets/Image/controll_icon.png", Texture_ControllerUI, scene, { "BattleScene" });
    load_texture("Assets/Image/spark_icon.png", Texture_Spark, scene, { "BattleScene" });
    load_texture("Assets/Image/mapUI.png", Texture_CircleMap, scene, { "BattleScene" });
    load_texture("Assets/Image/mapIcon_red.png", Texture_MapIconRed, scene, { "BattleScene" });
    load_texture("Assets/Image/mapIcon_green.png", Texture_MapIconGreen, scene, { "BattleScene" });

    load_texture("Assets/Image/background_parameter.png", Texture_background_parameter, scene, { "ParameterScene" });
    load_texture("Assets/Image/button_parameterCheck.png", Texture_buttonParameterCheck, scene, { "ParameterScene" });
    load_texture("Assets/Image/button_parameterEnhance.png", Texture_buttonParameterEnhance, scene, { "ParameterScene" });
    load_texture("Assets/Image/button_enhance.png", Texture_buttonEnhance, scene, { "ParameterScene" });
    load_texture("Assets/Image/up_arrow.png", Texture_upArrow, scene, { "ParameterScene" });
    load_texture("Assets/Image/down_arrow.png", Texture_downArrow, scene, { "ParameterScene" });
    load_texture("Assets/Image/right_arrow.png", Texture_rightArrow, scene, { "ParameterScene", "BattleResultScene", "GameResultScene" });
    load_texture("Assets/Image/gem.png", Texture_gem, scene, { "ParameterScene", "BattleResultScene", "GameResultScene" });

}

void Image::clear(const std::string& scene)
{
    //???????????????????????????
    for (auto& handle : texture_id_list) {
        DxLib::DeleteGraph(handle);
        handle = -1;
    }
}

int Image::texture_handle(int texture_id)
{
    return texture_id_list[texture_id];
}

/// <summary>
/// DxLib::DrawGraphF?????????????????????
/// </summary>
/// <param name="texture_id"></param>
/// <param name="left">???????????????x</param>
/// <param name="top">???????????????y</param>
/// <param name="is_trans"></param>
void Image::draw_graph(unsigned int texture_id, float left, float top, bool is_trans)
{
    DxLib::DrawGraphF(left, top, Image::texture_handle(texture_id), is_trans);
}

/// DxLib::DrawRotaGraphF?????????????????????
/// </summary>
/// <param name="texture_id"></param>
/// <param name="x">???????????????x</param>
/// <param name="y">???????????????y???????????????x</param>
/// <param name="ex_rate"></param>
/// <param name="rota_angle"></param>
/// <param name="is_trans"></param>
void Image::draw_rota_graph(unsigned int texture_id, float x, float y, float ex_rate, float rota_angle, bool is_trans)
{
    DxLib::DrawRotaGraphF(x, y, ex_rate, rota_angle, Image::texture_handle(texture_id), is_trans);
}

void Image::draw_rect_graph(unsigned int texture_id, float x, float y, int srcX, int srcY, int width, int height, bool is_trans)
{
    DxLib::DrawRectGraphF(x, y, srcX, srcY, width, height, Image::texture_handle(texture_id), is_trans);
}

int Image::load_texture(const char* file_name, const std::string& scene, std::vector<std::string> use_sceces)
{
    int handle = DxLib::LoadGraph(file_name);
    assert(handle != -1);
    return handle;
}

int Image::load_texture(const char* file_name, int texture_id, const std::string& scene, std::vector<std::string> use_scenes)
{

    /*???????????????????????????????????????*/
    auto scene_itr = std::find(use_scenes.begin(), use_scenes.end(), scene);
    if (scene_itr == use_scenes.end()) return -1; //?????????????????????????????????

    if (texture_id_list[texture_id] != -1) return texture_id_list[texture_id]; //???????????????????????????????????????

    /*????????????????????????*/
    int handle = DxLib::LoadGraph(file_name);
    //?????????????????????????????????????????????????????????
    assert(handle != -1);
    //?????????????????????????????????????????????2????????????
    if (texture_id >= (int)texture_id_list.size()) {
        texture_id_list.resize(texture_id_list.size() * 2);
    }
    //??????ID?????????????????????????????????????????????????????????
    texture_id_list[texture_id] = handle;
    return handle;
}

void Image::clear_a_texture(int texture_id)
{
    DxLib::DeleteGraph(texture_id_list[texture_id]);
    texture_id_list[texture_id] = -1;
}
