#ifndef FONT_H_
#define FONT_H_

class Font
{
public:
	static void load();
	static void clear();

public:
	static int japanese_font;
	static int english_font;

private:
	Font() = default;
	‾Font() = default;
};
#endif//!FONT_H_

