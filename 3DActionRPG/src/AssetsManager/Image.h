#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>
#include <vector>
#include <DxLib.h>

//テクスチャID
enum
{
	Texture_test00 = 0,
	Texture_test01,
	Texture_test02,
	Texture_test03,
	Texture_icon_slime,
	Texture_icon_mimic,
	Texture_icon_blackKnight,
};

/*
 現状2通りの画像へのアクセス方法がある
 １．公開画像ハンドルを直接使ってDxLib関数で描画
 ２．texture_handle()にテクスチャIDを入れることで間接的に変数を得る
*/
class Image
{
public:
	Image() = default;
	‾Image() = default;
	static void load(const std::string& scene = "");
	static void clear(const std::string& scene = "");
	//dxlib由来のテクスチャハンドルを取得する
	static const int texture_handle(int texture_id);
private:
	static int load_texture(const char* file_name, const std::string& scene = "", std::vector<std::string> use_sceces = { "" });
	//テクスチャを読み込んでIDと紐づける
	static int load_texture(const char* file_name, int texture_id);
	//1枚の画像メモリを解放
	static void clear_a_texture(int texture_id);


public:
	static std::vector<int> texture_id_list;
	static int texture_couter;
	static int test_image00;
	static int test_image01;
	static int test_image02;
	static int test_image03;
};
#endif//!IMAGE_H_

