#ifndef IMAGE_H_
#define IMAGE_H_

#include "Singleton.h"

#include <string>
#include <vector>
#include <DxLib.h>

//テクスチャID
enum
{
	Texture_icon_player,
	Texture_icon_slime,
	Texture_icon_skeleton,
	Texture_icon_mage,
	Texture_icon_mimic,
	Texture_icon_blackKnight,
	Texture_icon_chest,
	Texture_hpPortion,
	Texture_silhouette_slime,
	Texture_silhouette_skeleton,
	Texture_silhouette_mage,
	Texture_silhouette_mimic,
	Texture_silhouette_blackKnight,
	Texture_background_oldmap,
	Texture_mapArea,
	Texture_GaugeFrame,
	Texture_GaugeBarGreen,
	Texture_GaugeBarRed,
	Texture_GaugeBarGray,
	Texture_ControllerUI,
	Texture_Spark,
	Texture_CircleMap,
	Texture_MapIconRed,
	Texture_MapIconGreen,
	Texture_buttonParameterCheck,
	Texture_buttonParameterEnhance,
	Texture_background_parameter,
	Texture_cursor,
	Texture_buttonEnhance,
	Texture_gem,
	Texture_upArrow,
	Texture_downArrow,
	Texture_rightArrow,
	Texture_titleLogo,
	Texture_textGameStart,
	Texture_textParameter,
	Texture_board_battleResult,
	Texture_board_gameResult,
};

//テクスチャ管理クラス
class Image : public Singleton<Image>
{
public:
	void load(const std::string& scene = "");
	void clear(const std::string& scene = "");
	//dxlib由来のテクスチャハンドルを取得する
	int texture_handle(int texture_id);
	//位置のみを指定して画像描画
	void draw_graph(unsigned int texture_id, float left = 0.0f, float top = 0.0f, bool is_trans = true);
	//位置、スケール、回転角度を指定して画像描画
	void draw_rota_graph(unsigned int texture_id, float x = 0.0f, float y = 0.0f, float ex_rate = 1.0f, float rota_angle = 0.0f, bool is_trans = true);
	//画像の一部を描画
	void draw_rect_graph(unsigned int texture_id, float x, float y, int srcX, int srcY, int width, int height, bool is_trans = true);
private:
	int load_texture(const char* file_name, const std::string& scene = "", std::vector<std::string> use_sceces = { "" });
	//テクスチャを読み込んでIDと紐づける
	int load_texture(const char* file_name, int texture_id, const std::string& scene = "", std::vector<std::string> use_scenes = { "" });
	//1枚の画像メモリを解放
	void clear_a_texture(int texture_id);

private:
	Image() = default;
	‾Image() = default;

	friend class Singleton<Image>;
};
#endif//!IMAGE_H_

