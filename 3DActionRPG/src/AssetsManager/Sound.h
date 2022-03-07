#ifndef SOUND_H_
#define SOUND_H_

#include "Singleton.h"

#include <string>
#include <vector>

//サウンドID
enum
{
	SE_BattleStart
};

enum
{
	BGM_Battle,
};
class Sound : public Singleton<Sound>
{
public:
	//指定シーンに使うテクスチャを読み込み
	void load(const std::string& scene = "");
	//全テクスチャメモリを解放
	void clear(const std::string& scene = "");
	//dxlib由来のテクスチャハンドルを取得する
	const int sound_handle(int texture_id);
	//SEを一度再生
	void play_SE(int SE_id);
	//BGMをループ再生
	void play_BGM(int BGM_id);

private:
	//SEを読み込み
	int load_a_SE(const char* file_name, int sound_id, const std::string& scene = "", std::vector<std::string> use_scenes = { "" });
	//BGMのファイルパスを登録（メモリ確保はしない）
	void register_a_BGM(const char* file_name, int BGM_id);

private:
	Sound() = default;
	‾Sound() = default;
private:
	friend class Singleton<Sound>;
};
#endif//!SOUND_H_

