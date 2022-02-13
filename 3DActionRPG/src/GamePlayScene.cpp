#include "GamePlayScene.h"
#include "Player.h"

void GamePlayScene::start(void* data)
{
    
    //終了フラグを初期化
    is_end_ = false;
    
}

void GamePlayScene::update(float delta_time)
{
    world_.update(delta_time);
}

void GamePlayScene::draw() const
{
    world_.draw();
}

bool GamePlayScene::is_end() const
{
    return is_end_;
    ////プレイヤーが死亡したらシーン終了
    //return world_.is_game_over() || world_.is_game_clear();
}

std::string GamePlayScene::next() const
{
    return "ResultScene";
}

void GamePlayScene::end()
{
    //消去
    world_.clear();
    // テクスチャの削除
    
}

void* GamePlayScene::data()
{
    return nullptr;
}
