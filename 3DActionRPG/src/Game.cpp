#include "Game.h"

#include <DxLib.h>

#include "Util/MyRandom.h"
#include "Mesh.h"
#include "Input.h"
#include "Image.h"

#include "BattleScene.h"
#include "MapScene/MapScene.h"
#include "MapScene/MapManager.h"


Game::Game()
{

}

Game::‾Game() {

}

void Game::Init() {
	Input::init();
	//Mesh::load();
	/*Image::load();
	MyRandom::Init();
	MapManager::GetInstance().generate();*/

	scene_manager_.add("BattleScene", new BattleScene{});
	scene_manager_.add("MapScene", new MapScene{});
	//scene_manager_.change("BattleScene");
	scene_manager_.change("MapScene");
	MapManager::GetInstance().generate();

	//ForDebug
	/*PlayerDatabase::GetInstance().load("Assets/Parameters/player_parameter.csv");
	EnemyDatabase::GetInstance().load("Assets/Parameters/EnemyParameter.csv");*/

	//DxLib::SetGlobalAmbientLight(DxLib::GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	//DxLib::ChangeLightTypePoint(VGet(0.0f, 300.0f, 0.0f), 2000.0f, 0.0f, 0.0006f, 0.0f);
	//DxLib::SetCameraNearFar(200.0f, 50000.0f);
	//world_.add_camera(new Camera{&world_});
	//world_.add_actor(new Player{&world_});
	////world_.add_actor(new Enemy{ &world_ });
	//world_.add_actor(new Slime{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	//DxLib::MV1SetupCollInfo(Mesh::stage_handle, -1, 8, 8, 8);

}

void Game::Update() {
	Input::update();
	//TODO::経過時間を計測するよう変更
	float delta_time = 0.016667f;
	scene_manager_.update(delta_time);
	//world_.update(delta_time);

}

void Game::Draw() {

	//MapManager::GetInstance().draw();
	scene_manager_.draw();
	////ForDebug
	////スカイボックスの描画
	//DxLib::SetUseLighting(FALSE);
	//DxLib::MV1DrawModel(Mesh::skybox);
	//DxLib::SetUseLighting(TRUE);
	////ステージの描画
	//DxLib::MV1DrawModel(Mesh::stage_handle);
	//world_.draw();


	////ライトの設定
	////DxLib::SetLightPosition(VECTOR{ 0, 500, 0 });
	////DxLib::SetLightDirection(VECTOR{ -1, 1, 1 });
	//VECTOR light_pos = DxLib::GetLightPosition();
	//VECTOR light_dir = DxLib::GetLightDirection();
}

void Game::End()
{
	scene_manager_.end();
	/*Mesh::clear();*/
	/*Image::clear();
	MapManager::GetInstance().clear();*/

}
