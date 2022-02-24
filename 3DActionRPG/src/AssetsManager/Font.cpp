#include "Font.h"

#include <DxLib.h>

int Font::japanese_font{ -1 };
int Font::english_font_35{ -1 };

void Font::load()
{
	//全フォントデータを取得
	japanese_font = DxLib::CreateFontToHandle("HGS創英ﾌﾟﾚｾﾞﾝｽEB", 30, -1);
	english_font_35 = DxLib::CreateFontToHandle("UD デジタル 教科書体 NK-B", 35, -1, DX_FONTTYPE_ANTIALIASING);
	//english_font = DxLib::CreateFontToHandle("Arial Bold", 30, -1, DX_FONTTYPE_ANTIALIASING);
}

void Font::clear()
{
	//全フォントメモリを解放
	DxLib::InitFontToHandle();
}
