#include "Game.h"

#include <DxLib.h>

#include "Mesh.h"
#include "Input.h"
#include "Player.h"
#include "Enemy.h"
#include "Slime.h"
#include "Camera.h"
#include "AttackCollider.h"

Game::Game()
{

}

Game::‾Game() {

}

void Game::Init() {
	Input::init();
	Mesh::load();

	//ForDebug
	//DxLib::SetGlobalAmbientLight(DxLib::GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	//DxLib::ChangeLightTypePoint(VGet(0.0f, 300.0f, 0.0f), 2000.0f, 0.0f, 0.0006f, 0.0f);
	DxLib::SetCameraNearFar(200.0f, 50000.0f);
	world_.add_camera(new Camera{&world_});
	world_.add_actor(new Player{&world_});
	//world_.add_actor(new Enemy{ &world_ });
	world_.add_actor(new Slime{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	DxLib::MV1SetupCollInfo(Mesh::stage_handle, -1, 8, 8, 8);

}

void Game::Update() {
	float delta_time = 0.016667f;
	Input::update();
	world_.update(delta_time);
	//player_.update(delta_time);

}

void Game::Draw() {

	//ForDebug
	//スカイボックスの描画
	DxLib::SetUseLighting(FALSE);
	DxLib::MV1DrawModel(Mesh::skybox);
	DxLib::SetUseLighting(TRUE);
	//ステージの描画
	DxLib::MV1DrawModel(Mesh::stage_handle);
	world_.draw();
	//camera_.draw();

	//player_.draw();

	//ライトの設定
	//DxLib::SetLightPosition(VECTOR{ 0, 500, 0 });
	//DxLib::SetLightDirection(VECTOR{ -1, 1, 1 });
	VECTOR light_pos = DxLib::GetLightPosition();
	VECTOR light_dir = DxLib::GetLightDirection();
}

void Game::End()
{
	Mesh::clear();
}
