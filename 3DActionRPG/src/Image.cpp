#include "Image.h"
#include <assert.h>
#include "Screen.h"

int Image::bossImage{ -1 };

void Image::load(const std::string& scene)
{
    bossImage= load_texture("Assets/Image/test.png");
}

int Image::load_texture(const char* file_name, const std::string& scene, std::vector<std::string> use_sceces)
{
    int handle = LoadGraph(file_name);
    assert(handle != -1);
    return handle;
}
