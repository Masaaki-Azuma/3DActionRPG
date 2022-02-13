#include "Game.h"

#include <DxLib.h>

#include "Util/MyRandom.h"
#include "Input.h"
#include "Image.h"

#include "TitleScene/TitleScene.h"
#include "BattleScene.h"
#include "MapScene/MapScene.h"
#include "MapScene/MapManager.h"


Game::Game()
{

}

Game::‾Game() 
{

}

void Game::Init() 
{	Input::init();
	MyRandom::Init();

	scene_manager_.add("TitleScene", new TitleScene{});
	scene_manager_.add("BattleScene", new BattleScene{});
	scene_manager_.add("MapScene", new MapScene{});
	scene_manager_.change("TitleScene");
	//scene_manager_.change("BattleScene");
	//scene_manager_.change("MapScene");
}

void Game::Update() 
{	Input::update();
	//TODO::経過時間を計測するよう変更
	float delta_time = 0.016667f;
	scene_manager_.update(delta_time);

}

void Game::Draw()
{
	scene_manager_.draw();
}

void Game::End()
{
	scene_manager_.end();
}
