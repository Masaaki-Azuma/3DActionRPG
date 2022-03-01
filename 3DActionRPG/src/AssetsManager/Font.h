#ifndef FONT_H_
#define FONT_H_

#include <string>

class Font
{
public:
	static void load();
	static void clear();
	//左詰め描画
	static void draw(float left, float top, const std::string& text, int color = NULL, unsigned int handle = NULL);
	//右詰め描画
	static void draw_right_aligned(float right, float top, const std::string& text, int color = NULL, unsigned int handle = NULL);
	//中央描画
	static void draw_centered(float top, const std::string& text, int color = NULL, unsigned int handle = NULL);

public:
	static int japanese_font_35;
	static int japanese_font_50;
	static int english_font_35;

private:
	Font() = default;
	‾Font() = default;
};
#endif//!FONT_H_

