#include "Image.h"

#include <assert.h>

std::vector<int> Image::texture_id_list(256, -1);
int Image::texture_couter{ 0 };
int Image::test_image00{ -1 };
int Image::test_image01{ -1 };
int Image::test_image02{ -1 };
int Image::test_image03{ -1 };

void Image::load(const std::string& scene)
{
   /* test_image00 = load_texture("Image/machoB01.png");
    test_image01 = load_texture("Image/macho.png");
    test_image02 = load_texture("Image/machoPlayer00.png");
    test_image03 = load_texture("Image/machoPlayer01.png");*/

    test_image00 = load_texture("Assets/Image/test/machoB01.png", Texture_test00);
    test_image01 = load_texture("Assets/Image/test/macho.png", Texture_test01);
    test_image02 = load_texture("Assets/Image/test/machoPlayer00.png", Texture_test02);
    test_image03 = load_texture("Assets/Image/test/machoPlayer01.png", Texture_test03);

}

void Image::clear(const std::string& scene)
{
    clear_a_texture(Texture_test00);
    clear_a_texture(Texture_test01);
    clear_a_texture(Texture_test02);
    clear_a_texture(Texture_test03);
}

const int Image::texture_handle(int texture_id)
{
    return texture_id_list[texture_id];
}

int Image::load_texture(const char* file_name, const std::string& scene, std::vector<std::string> use_sceces)
{
    int handle = DxLib::LoadGraph(file_name);
    assert(handle != -1);
    return handle;
}

int Image::load_texture(const char* file_name, int texture_id)
{
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
