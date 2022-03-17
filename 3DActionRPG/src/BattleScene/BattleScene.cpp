#include "BattleScene.h"

#include "AssetsManager/Image.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/Sound.h"
#include "AssetsManager/Font.h"
#include "AssetsManager/PlayerDatabase.h"
#include "Util/PadInput.h"
#include "Actor/ControllUI.h"
#include "Actor/Camera.h"
#include "Actor/Player.h"

enum
{
	Scene_Start,
	Scene_Battle,
	Scene_BattleResult,
};

BattleScene::BattleScene():
	enemy_spawner_{ world_ },
	p_DB_{PlayerDatabase::GetInstance()},
	start_text_{ "Battle Start!", Font::japanese_font_120_edge, DxLib::GetColor(207, 205, 175), Vector3{0.0f, 200.0f, 0.0f}, 0.5f}
{
}

void BattleScene::start(void* data)
{
    is_end_ = false;

	//リソースの読み込み
	Image::GetInstance().load("BattleScene");
	Mesh::GetInstance().load("BattleScene");
	Sound::GetInstance().load("BattleScene");
	//ライト設定
	light_.add_directional_light(Vector3{ 1, -1, 1 });
	light_.add_directional_light(Vector3{ -1, -1, -1 });

	DxLib::SetCameraNearFar(200.0f, 50000.0f);

	//HPゲージ
	hp_gauge_ = ExtendableBarGauge{ 150, 100, 540, 40, Texture_GaugeFrame, Texture_GaugeBarGreen, Texture_GaugeBarGray};
	hp_gauge_.extend(p_DB_.get_master_parameter().hp, p_DB_.limit_hp());
	hp_gauge_.set_edge_width(3);
	//タイマー
	timer_.set_color(DxLib::GetColor(200, 200, 200));
	timer_.set_font(Font::japanese_font_60_edge);
	timer_.reset();
	//バトル開始テキストをアクティブに
	start_text_.reset();
	start_text_.start();
	//シーン状態をスタート状態に
	scene_state_ = Scene_Start;

	//アクター追加
	world_.add_actor(std::make_shared<ControllUI>(Vector3{ 1700, 900 }));
	int stage_mesh = Mesh::GetInstance().mesh_handle(Mesh_StageMesh);
	int stage_collider = Mesh::GetInstance().mesh_handle(Mesh_StageCollider);
	int skybox = Mesh::GetInstance().mesh_handle(Mesh_Skybox);
	world_.add_field(std::make_shared<Field>(stage_mesh, stage_collider, skybox));
	world_.add_actor(std::make_shared<Player>(&world_, Vector3{ 0.0f, 0.0f, -500.0f }));
	world_.add_camera(std::make_shared<Camera>(&world_));

	//MapSceneから送られたデータを取得
	const std::string& enemy = *(static_cast<std::string*>(data));
	//敵を生成
	spawn_enemy(enemy);
	
	Sound::GetInstance().play_BGM(BGM_Battle);
	Sound::GetInstance().set_BGM_volume(0.8f);
}

void BattleScene::update(float delta_time)
{
	//シーン内の状態で分岐
	//それぞれ別シーンのクラスとして作ることもできる
	switch (scene_state_) {
	case Scene_Start:        update_start(delta_time);         break;
	case Scene_Battle:       update_battle(delta_time);        break;
	case Scene_BattleResult: update_battle_result(delta_time); break;
	}
}

void BattleScene::draw() const
{
	//アクターの描画
	world_.draw();
	//HPゲージ描画
	hp_gauge_.draw_gui(static_cast<float>(p_DB_.get_current_parameter().hp));
	//タイマー描画
	timer_.draw_center(40.0f);

	//バトル開始テキスト
	if(scene_state_ == Scene_Start) start_text_.draw();
	//バトル終了状態ではバトルリザルトシーンを描画
	if(scene_state_ == Scene_BattleResult)result_scene_.draw();
}

bool BattleScene::is_end() const
{
	//敵が全滅するか、プレイヤーが死亡したら終了
	return  is_end_ && result_scene_.is_end();
}

std::string BattleScene::next() const
{
    return result_scene_.next();
}

void BattleScene::end()
{
	result_scene_.end();
	world_.clear();

	light_.clear();

	//リソースの破棄
	Sound::GetInstance().clear();
	Mesh::GetInstance().clear();
	Image::GetInstance().clear();
}

void* BattleScene::data()
{
	//バトル勝敗を保持
	if      (world_.is_battle_win())  result_.battle_result = "Win";
	else if (world_.is_battle_lose()) result_.battle_result = "Lose";
	//敵ごとの討伐数を保持
	result_.basterd_list = world_.basterd_list();
	result_.time = timer_.get_sec();
    return &result_;
}

void BattleScene::update_start(float delta_time)
{
	PadInput& input = PadInput::GetInstance();
	/*スタートシーン*/
	start_text_.update(delta_time);
	if (start_text_.is_end() && input.GetButtonDown(XINPUT_BUTTON_B)) {
		scene_state_ = Scene_Battle;
	}
}

void BattleScene::update_battle(float delta_time)
{
	/*バトルシーン*/
	world_.update(delta_time);

	//タイマー更新
	timer_.update(delta_time);

	//戦闘が終了したらリザルトシーンへ
	if (is_settled()) {
		//バトルシーンは実質終了
		is_end_ = true;
		scene_state_ = Scene_BattleResult;
		result_scene_.start(data());
	}
}

void BattleScene::update_battle_result(float delta_time)
{
	/*バトルリザルトシーン*/
	result_scene_.update(delta_time);
}

bool BattleScene::is_settled() const
{
	return world_.is_battle_end();
}

void BattleScene::spawn_enemy(const std::string& enemy)
{
	enemy_spawner_.spawn(enemy);

	
}
