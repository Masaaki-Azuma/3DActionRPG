#include "MapScene.h"

#include "Util/MyRandom.h"
#include "Util/Input.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/PlayerDatabase.h"
#include "MapManager.h"

MapScene::MapScene():
    map_{MapManager::GetInstance()}
{
}

void MapScene::start(void* data)
{
    Image::load("MapScene");

    is_end_ = false;
    Image::load();
    MyRandom::Init();

    //バトルシーンから帰ってきたときの更新処理
    map_.make_node_old();

    //ForDebug:
}

void MapScene::update(float delta_time)
{

    if (map_.is_final_area()) {
        is_end_ = true;
    }
    map_.update(delta_time);
    if (map_.is_picked()) {
        is_end_ = true;
    }

    //ForDebug:エリアを動かずにシーン遷移チート
    //restart_area();
}

void MapScene::draw() const
{
    map_.draw();

}

bool MapScene::is_end() const
{
    return is_end_;
}

std::string MapScene::next() const
{
    if (map_.is_final_area()) return "GameResultScene";
    else                      return "BattleScene";
}

void MapScene::end()
{
    Image::clear();
}

void* MapScene::data()
{
    //次に出現する敵の名前を送信
    return &map_.selected_enemy();
}

void MapScene::restart_area()
{
    if (Input::get_button(PAD_INPUT_4)) { //A
        is_end_ = true;
    }
}
