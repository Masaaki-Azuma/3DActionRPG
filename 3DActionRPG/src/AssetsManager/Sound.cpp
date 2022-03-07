#include "Sound.h"

#include <unordered_map>
#include <cassert>
#include <DxLib.h>

static std::vector<int> SE_id_list(256, -1);
static std::unordered_map<int, std::string> BGM_file_name_list;

void Sound::load(const std::string& scene)
{
    load_a_SE("Assets/Sound/SE/battle_result.mp3", SE_BattleStart, scene, { "PlayScene" });

    register_a_BGM("Assets/Sound/BGM/bravely,_in_the_battlefield.mp3", BGM_Battle);
}

void Sound::clear(const std::string& scene)
{
    for (auto& handle : SE_id_list) {
        DxLib::DeleteSoundMem(handle);
        handle = -1;
    }
}

const int Sound::sound_handle(int texture_id)
{
    return SE_id_list[texture_id];
}

void Sound::play_SE(int SE_id)
{
    DxLib::PlaySoundMem(sound_handle(SE_id), DX_PLAYTYPE_BACK);
}

void Sound::play_BGM(int BGM_id)
{
    DxLib::PlayMusic(BGM_file_name_list.at(BGM_id).c_str(), DX_PLAYTYPE_LOOP);
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