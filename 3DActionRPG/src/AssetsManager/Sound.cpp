#include "Sound.h"

#include <unordered_map>
#include <cassert>
#include <DxLib.h>

static std::vector<int> SE_id_list(256, -1);
static std::unordered_map<int, std::string> BGM_file_name_list;

void Sound::load(const std::string& scene)
{
    load_a_SE("Assets/Sound/SE/cursor_move.mp3", SE_CursorMove, scene, { "TitleScene", "MapScene", "ParameterScene" });
    load_a_SE("Assets/Sound/SE/menu_select.mp3", SE_Select, scene, { "TitleScene", "MapScene", "ParameterScene", "BattleResultScene", "GameResultScene" });
    load_a_SE("Assets/Sound/SE/cancel.mp3", SE_Cancel, scene, { "ParameterScene" });
    load_a_SE("Assets/Sound/SE/encount.mp3", SE_Encount, scene, { "MapScene" });
    load_a_SE("Assets/Sound/SE/buzzer.mp3", SE_Buzzer, scene, { "ParameterScene" });
    load_a_SE("Assets/Sound/SE/enhance.mp3", SE_Enhance, scene, { "ParameterScene" });
    load_a_SE("Assets/Sound/SE/heal.mp3", SE_Heal, scene, { "MapScene" });

    load_a_SE("Assets/Sound/SE/battle_start.mp3", SE_BattleStart, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/battle_result.mp3", SE_BattleResult, scene, { "BattleResultScene" });
    load_a_SE("Assets/Sound/SE/result_appear01.mp3", SE_ResultAppear01, scene, { "BattleResultScene" });
    load_a_SE("Assets/Sound/SE/result_appear02.mp3", SE_ResultAppear02, scene, { "BattleResultScene" });
    load_a_SE("Assets/Sound/SE/game_result.mp3", SE_GameResult, scene, { "GameResultScene" });

    load_a_SE("Assets/Sound/SE/avoid.mp3", SE_Avoid, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/sword_attack01.mp3", SE_SwordAttack01, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/sword_attack02.mp3", SE_SwordAttack02, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/sword_attack03.mp3", SE_SwordAttack03, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/monster_damage.mp3", SE_MonsterDamage, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/slime_attack.mp3", SE_SlimeAttack, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/slime_bound.mp3", SE_SlimeBounce, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/skeleton_sword.mp3", SE_SkeletonAttack, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/skeleton_run.mp3", SE_SkeletonRun, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/shieldguard.mp3", SE_SkeletonGuard, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/mimic_longAttack.mp3", SE_MimicLongAttack, scene, { "BattleScene" });
    load_a_SE("Assets/Sound/SE/mimic_rage.mp3", SE_MimicRage, scene, { "BattleScene" });

    register_a_BGM("Assets/Sound/BGM/bgm_title.mp3", BGM_Title);
    register_a_BGM("Assets/Sound/BGM/bgm_battle.mp3", BGM_Battle);
    register_a_BGM("Assets/Sound/BGM/bgm_map01.mp3", BGM_Map);
}

void Sound::clear(const std::string& scene)
{
    for (auto& handle : SE_id_list) {
        DxLib::DeleteSoundMem(handle);
        handle = -1;
    }
}

const int Sound::sound_handle(int texture_id) const
{
    return SE_id_list[texture_id];
}

void Sound::play_SE(int SE_id, PlayMode mode)
{
    switch (mode) {
    case PlayMode::Single: DxLib::PlaySoundMem(sound_handle(SE_id), DX_PLAYTYPE_BACK); break;
    case PlayMode::Loop:   DxLib::PlaySoundMem(sound_handle(SE_id), DX_PLAYTYPE_LOOP); break;
    }
}

void Sound::stop_SE(int SE_id)
{
    DxLib::StopSoundMem(sound_handle(SE_id));
}

bool Sound::check_SE_playing(int SE_id) const
{
    return DxLib::CheckSoundMem(sound_handle(SE_id));
}

void Sound::play_BGM(int BGM_id)
{
    stop_BGM();
    DxLib::PlayMusic(BGM_file_name_list.at(BGM_id).c_str(), DX_PLAYTYPE_LOOP);
}

void Sound::stop_BGM()
{
    DxLib::StopMusic();
}

int Sound::load_a_SE(const char* file_name, int sound_id, const std::string& scene, std::vector<std::string> use_scenes)
{
    /*ロードが必要なシーンか判定*/
    auto scene_itr = std::find(use_scenes.begin(), use_scenes.end(), scene);
    if (scene_itr == use_scenes.end()) return -1; //読み込むシーンではない

    if (SE_id_list[sound_id] != -1) return SE_id_list[sound_id]; //既に読み込み済みなのでパス

    /*実際のロード処理*/
    int handle = DxLib::LoadSoundMem(file_name);
    //画像が正しく読み込まれていなければ停止
    assert(handle != -1);
    //ハンドル配列メモリが不足したら2倍に拡張
    if (sound_id >= static_cast<int>(SE_id_list.size())) {
        SE_id_list.resize(SE_id_list.size() * 2);
    }
    //画像IDをインデックスとして画像ハンドルを格納
    SE_id_list[sound_id] = handle;
    return handle;
}

void Sound::register_a_BGM(const char* file_name, int BGM_id)
{
    BGM_file_name_list[BGM_id] = file_name;
}