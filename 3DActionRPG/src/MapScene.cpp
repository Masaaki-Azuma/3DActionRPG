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
    MapManager::GetInstance().generate();
}

void MapScene::update(float delta_time)
{
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
    MapManager::GetInstance().clear();
}

void* MapScene::data()
{
    return nullptr;
}
