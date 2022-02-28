#include "Font.h"

#include <DxLib.h>

static const unsigned int white{ DxLib::GetColor(255,255,255) };

int Font::japanese_font_35{ -1 };
int Font::japanese_font_50{ -1 };
int Font::english_font_35{ -1 };


void Font::load()
{
	//全フォントデータを取得
	japanese_font_35 = DxLib::CreateFontToHandle("HGS創英ﾌﾟﾚｾﾞﾝｽEB", 35, -1, DX_FONTTYPE_ANTIALIASING);
	japanese_font_50 = DxLib::CreateFontToHandle("HGS創英ﾌﾟﾚｾﾞﾝｽEB", 50, -1, DX_FONTTYPE_ANTIALIASING);
	english_font_35 = DxLib::CreateFontToHandle("UD デジタル 教科書体 NK-B", 35, -1, DX_FONTTYPE_ANTIALIASING);
	//english_font = DxLib::CreateFontToHandle("Arial Bold", 30, -1, DX_FONTTYPE_ANTIALIASING);
}

void Font::clear()
{
	//全フォントメモリを解放
	DxLib::InitFontToHandle();
}


void Font::draw(float left, float top, const std::string& text, int color, unsigned int handle)
{
	//色指定がなければ白
	if (color == NULL) color = white;
	//フォントハンドル指定の有無で分岐
	if (handle == NULL) DxLib::DrawStringF(left, top, text.c_str(), color);
	else                DxLib::DrawStringFToHandle(left, top, text.c_str(), color, handle);
}

void Font::draw_right_aligned(float right, float top, const std::string& text, int color, unsigned int handle)
{
	auto text_c = text.c_str();
	int width = DxLib::GetDrawStringWidthToHandle(text_c, std::strlen(text_c), handle);
	float left = right - width;
	draw(left, top, text, color, handle);

}

void Font::draw_centered(float top, const std::string& text, int color, unsigned int handle)
{
	auto text_c = text.c_str();
	int text_width = DxLib::GetDrawStringWidthToHandle(text_c, std::strlen(text_c), handle);
	int screen_width, screen_height;
	DxLib::GetScreenState(&screen_width, &screen_height, NULL);
	float left = static_cast<float>(screen_width) / 2 - text_width / 2;
	draw(left, top, text, color, handle);
}
