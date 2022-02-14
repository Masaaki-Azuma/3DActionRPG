#include "MapScene.h"

#include "Util/MyRandom.h"
#include "Util/Input.h"
#include "AssetsManager/Image.h"
#include "MapManager.h"

MapScene::MapScene():
    map_{MapManager::GetInstance()}
{
}

void MapScene::start(void* data)
{
    is_end_ = false;
    Image::load();
    MyRandom::Init();

    //ForDebug
    map_.make_node_old();
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

    //ForDebug:シーン遷移チート
    if (Input::get_button(PAD_INPUT_4)) { //A
        is_end_ = true;
    }
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
    return &map_.selected_enemy();
}
