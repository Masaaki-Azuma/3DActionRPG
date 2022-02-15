#include "GameResultScene.h"

#include <DxLib.h>

#include "Util/Input.h"
#include "AssetsManager/PlayerDatabase.h"
#include "MapScene/MapManager.h"

void GameResultScene::start(void* data)
{
    is_end_ = false;
}

void GameResultScene::update(float delta_time)
{
    //ForDebug:シーンを抜け出す仮動作
    if (Input::get_button_down(PAD_INPUT_1))     {
        is_end_ = true;
    }
}

void GameResultScene::draw() const
{
    //ForDebug:仮シーン表示
    DxLib::DrawString(0, 0, "ゲームリザルトシーン", GetColor(255, 255, 255));
    //ForDebug:表示するジェム数を取得できているか確認
    int gem = PlayerDatabase::GetInstance().get_current_parameter().total_gem;
    DxLib::DrawFormatString(0, 20, GetColor(255, 255, 255), "totalgem: %d", gem);
    int master = PlayerDatabase::GetInstance().get_master_parameter().total_gem + gem;
    DxLib::DrawFormatString(0, 40, GetColor(255, 255, 255), "mastergem: %d", master);
}

bool GameResultScene::is_end() const
{
    return is_end_;
}

std::string GameResultScene::next() const
{
    return "TitleScene";
}

void GameResultScene::end()
{
    //1プレイで得たジェム数を取得
    int gained_gem = p_DB_.get_current_parameter().total_gem;
    //セーブデータに取得ジェムを追加
    p_DB_.add_possessed_jem(gained_gem);
    //マップデータを削除
    MapManager::GetInstance().clear();
}

void* GameResultScene::data()
{
    return nullptr;
}
