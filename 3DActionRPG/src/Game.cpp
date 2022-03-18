#include "Game.h"

#include <DxLib.h>

#include "Util/MyRandom.h"
#include "Util/PadInput.h"
#include "AssetsManager/Font.h"
#include "AssetsManager/Image.h"
#include "AssetsManager/PlayerDatabase.h"
#include "AssetsManager/EnemyDatabase.h"
#include "TitleScene/TitleScene.h"
#include "BattleScene/BattleScene.h"
#include "MapScene/MapScene.h"
#include "GameResultScene/GameResultScene.h"
#include "ParameterScene/ParameterScene.h"


Game::Game()
{

}

Game::‾Game() 
{

}

void Game::Init() 
{
	Font::load();
	PadInput::GetInstance().Init();
	MyRandom::Init();
	//セーブデータをロード
	PlayerDatabase& p_DB = PlayerDatabase::GetInstance();
	p_DB.set_dst_file("Assets/Parameters/player_parameter.csv");
	p_DB.load_master_data();
	//敵のパラメーターをロード
	EnemyDatabase::GetInstance().load("Assets/Parameters/EnemyParameter.csv");
	//シーンを登録
	scene_manager_.add("TitleScene", new TitleScene{});
	scene_manager_.add("BattleScene", new BattleScene{});
	scene_manager_.add("MapScene", new MapScene{});
	scene_manager_.add("GameResultScene", new GameResultScene{});
	scene_manager_.add("ParameterScene", new ParameterScene{});
	scene_manager_.change("TitleScene");
}

void Game::Update() 
{
	PadInput::GetInstance().Update();
    //60fps固定
	static const float delta_time = 1 / 60.0f;
	scene_manager_.update(delta_time);

}

void Game::Draw()
{
	scene_manager_.draw();
}

void Game::End()
{
	scene_manager_.end();
	//データをセーブ
	PlayerDatabase::GetInstance().save_master_data();
	Font::clear();
}
