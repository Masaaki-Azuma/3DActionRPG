#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>
#include <vector>
#include <DxLib.h>

class Image
{
public:
	Image() = default;
	‾Image() = default;
	static void load(const std::string& scene = "");
private:
	static int load_div_texture(const char* file_name, const std::string& scene = "");
	static int load_texture(const char* file_name, const std::string& scene = "", std::vector<std::string> use_sceces = { "" });

public:
	static int bossImage;
};
#endif//!IMAGE_H_

