#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>
#include <vector>
#include <DxLib.h>

//テクスチャID
enum
{
	Texture_icon_slime,
	Texture_icon_skeleton,
	Texture_icon_mage,
	Texture_icon_mimic,
	Texture_icon_blackKnight,
	Texture_background_oldmap,
	Texture_mapArea,
	Texture_GaugeFrame,
	Texture_GaugeBarGreen,
	Texture_GaugeBarRed,
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

/*
 現状2通りの画像へのアクセス方法がある
 １．公開画像ハンドルを直接使ってDxLib関数で描画
 ２．texture_handle()にテクスチャIDを入れることで間接的に変数を得る
*/
class Image
{
public:
	static void load(const std::string& scene = "");
	static void clear(const std::string& scene = "");
	//dxlib由来のテクスチャハンドルを取得する
	static const int texture_handle(int texture_id);
	//位置のみを指定して画像描画
	static void draw_graph(unsigned int texture_id, float left = 0.0f, float top = 0.0f, bool is_trans = true);
	//位置、スケール、回転角度を指定して画像描画
	static void draw_rota_graph(unsigned int texture_id, float x = 0.0f, float y = 0.0f, float ex_rate = 1.0f, float rota_angle = 0.0f, bool is_trans = true);
private:
	static int load_texture(const char* file_name, const std::string& scene = "", std::vector<std::string> use_sceces = { "" });
	//テクスチャを読み込んでIDと紐づける
	static int load_texture(const char* file_name, int texture_id, const std::string& scene = "", std::vector<std::string> use_scenes = { "" });
	//1枚の画像メモリを解放
	static void clear_a_texture(int texture_id);


public:
	static std::vector<int> texture_id_list;
	static int texture_couter;

private:
	Image() = default;
	‾Image() = default;
};
#endif//!IMAGE_H_

