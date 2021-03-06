#ifndef SOUND_H_
#define SOUND_H_

#include "Singleton.h"

#include <string>
#include <vector>

//サウンドID
enum
{
	SE_CursorMove,
	SE_Select,
	SE_Cancel,
	SE_Buzzer,
	SE_Enhance,
	SE_Encount,
	SE_Heal,
	SE_BattleStart,
	SE_BattleWin,
	SE_BattleLose,
	SE_ResultAppear01,
	SE_ResultAppear02,
	SE_GameResult,

	SE_SwordAttack01,
	SE_Avoid,
	SE_Damage,
	SE_GroundWalk,
	SE_MonsterDamage,

	SE_SlimeAttack,
	SE_SlimeBounce,
	SE_SkeletonAttack,
	SE_SkeletonWalk,
	SE_SkeletonRun,
	SE_SkeletonGuard,
	SE_MimicLongAttack,
	SE_MimicRage,
	SE_BlackKnightSlash,
	SE_BlackKnightSwingDown,
	SE_BlackKnightTackle,
	SE_Crack,

};

enum
{
	BGM_Title,
	BGM_Map,
	BGM_Battle,
};

enum class PlayMode
{
	Single,
	Loop,
};

class Sound : public Singleton<Sound>
{
public:
	//指定シーンに使うテクスチャを読み込み
	void load(const std::string& scene = "");
	//全テクスチャメモリを解放
	void clear(const std::string& scene = "");
	//dxlib由来のテクスチャハンドルを取得する
	const int sound_handle(int texture_id) const;
	//SEを再生
	void play_SE(int SE_id, PlayMode mode = PlayMode::Single);
	//SEを停止
	void stop_SE(int SE_id);
	//SEが再生中か？
	bool check_SE_playing(int SE_id) const;
	//BGMをループ再生
	void play_BGM(int BGM_id);
	//0〜1（デフォルト）でBGM音量を調整
	void set_BGM_volume(float volume);
	//現在のBGMを停止
	void stop_BGM();

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

