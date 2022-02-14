#include "BattleScene.h"
#include "AssetsManager/PlayerDatabase.h"
#include "AssetsManager/EnemyDatabase.h"
#include "AssetsManager/Mesh.h"
#include "Util/Input.h"

#include "Actor/Camera.h"
#include "Actor/Player.h"
#include "Actor/Slime.h"
#include "Actor/Mimic.h"
#include "Actor/BlackKnight.h"

void BattleScene::start(void* data)
{
    is_end_ = false;

	Mesh::load();
    PlayerDatabase::GetInstance().load("Assets/Parameters/player_parameter.csv");
    EnemyDatabase::GetInstance().load("Assets/Parameters/EnemyParameter.csv");

	//ステージコライダーの衝突情報を取得できるよう準備
	DxLib::MV1SetupCollInfo(Mesh::stage_handle, -1, 8, 8, 8);
	//DxLib::SetGlobalAmbientLight(DxLib::GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	//DxLib::ChangeLightTypePoint(VGet(0.0f, 300.0f, 0.0f), 2000.0f, 0.0f, 0.0006f, 0.0f);
	DxLib::SetCameraNearFar(200.0f, 50000.0f);
	world_.add_actor(new Player{ &world_ });
	world_.add_camera(new Camera{ &world_ });

	//MapSceneから送られたデータを取得
	const std::string& enemy = *(static_cast<std::string*>(data));
	//敵を生成
	spawn_enemy(enemy);
	
}

void BattleScene::update(float delta_time)
{
	world_.update(delta_time);

	//ForDebug:シーン遷移チート
	if (Input::get_button(PAD_INPUT_4)) { //A
		is_end_ = true;
	}
}

void BattleScene::draw() const
{
	//ForDebug
	//スカイボックスの描画
	DxLib::SetUseLighting(FALSE);
	DxLib::MV1DrawModel(Mesh::skybox);
	DxLib::SetUseLighting(TRUE);

	//ステージの描画
	DxLib::MV1DrawModel(Mesh::stage_handle);
	//!ForDebug

	world_.draw();


	//ライトの設定
	//DxLib::SetLightPosition(VECTOR{ 0, 500, 0 });
	//DxLib::SetLightDirection(VECTOR{ -1, 1, 1 });
	VECTOR light_pos = DxLib::GetLightPosition();
	VECTOR light_dir = DxLib::GetLightDirection();
}

bool BattleScene::is_end() const
{
    return world_.count_actor_with_tag("EnemyTag") == 0;
}

std::string BattleScene::next() const
{
    return "MapScene";
}

void BattleScene::end()
{
	world_.clear();
	Mesh::clear();
}

void* BattleScene::data()
{
    return nullptr;
}

void BattleScene::spawn_enemy(const std::string& enemy)
{
	//HACK:せっかく敵の名前とそろえたので、もっと簡潔な記述にならないか？
	if (enemy == "Slime") {
		world_.add_actor(new Slime{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
		//world_.add_actor(new Slime{ &world_,  Vector3{ 100.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	}
	else if (enemy == "Mimic") {
		world_.add_actor(new Mimic{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	}
	else if (enemy == "BlackKnight") {
		world_.add_actor(new BlackKnight{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	}
}
