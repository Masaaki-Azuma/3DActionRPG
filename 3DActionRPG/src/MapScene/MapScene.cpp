#include "MapScene.h"
#include "MapManager.h"
#include "Image.h"
#include "Util/MyRandom.h"
#include "Input.h"

void MapScene::start(void* data)
{
    is_end_ = false;
    Image::load();
    MyRandom::Init();
    //MapManager::GetInstance().generate();

    //ForDebug
    MapManager::GetInstance().make_node_old();
}

void MapScene::update(float delta_time)
{
    MapManager::GetInstance().update(delta_time);
    if (MapManager::GetInstance().is_picked()) {
        is_end_ = true;
    }

    //ForDebug:シーン遷移チート
    if (Input::get_button(PAD_INPUT_4)) { //A
        is_end_ = true;
    }
}

void MapScene::draw() const
{
    MapManager::GetInstance().draw();

}

bool MapScene::is_end() const
{
    return is_end_;
}

std::string MapScene::next() const
{
    return "BattleScene";
}

void MapScene::end()
{
    Image::clear();
    //MapManager::GetInstance().clear();
}

void* MapScene::data()
{
    return nullptr;
}
