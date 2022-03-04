#include "BattleScene.h"

#include "AssetsManager/Image.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/Font.h"
#include "Util/Input.h"
#include "AssetsManager/PlayerDatabase.h"

#include "Actor/Camera.h"
#include "Actor/Player.h"
#include "Actor/Enemy/Slime.h"
#include "Actor/Enemy/Skeleton.h"
#include "Actor/Enemy/Mage.h"
#include "Actor/Enemy/Mimic.h"
#include "Actor/Enemy/BlackKnight.h"


BattleScene::BattleScene():
	p_DB_{PlayerDatabase::GetInstance()}
{
}

void BattleScene::start(void* data)
{
    is_end_ = false;

	//リソースの読み込み
	Image::load("BattleScene");
	Mesh::load();

	//ライト設定
	light_.add_directional_light(Vector3{ 1, -1, 1 });
	light_.add_directional_light(Vector3{ -1, -1, -1 });

	DxLib::SetCameraNearFar(200.0f, 50000.0f);

	//HPゲージ
	hp_gauge_ = ExtendableBarGauge{ 150, 100, 540, 40, Texture_GaugeFrame, Texture_GaugeBarGreen, Texture_GaugeBarGray};
	hp_gauge_.extend(p_DB_.get_master_parameter().hp, p_DB_.limit_hp());
	hp_gauge_.set_edge_width(10);

	//タイマー
	timer_.set_color(DxLib::GetColor(200, 200, 200));
	timer_.set_font(Font::japanese_font_60_edge);
	timer_.reset();

	world_.add_actor(new Player{ &world_ });
	world_.add_field(new Field{Mesh::ground_handle, Mesh::stage_collider_handle, Mesh::skybox_handle });
	//アクター追加
	world_.add_camera(new Camera{ &world_ });

	//MapSceneから送られたデータを取得
	const std::string& enemy = *(static_cast<std::string*>(data));
	//敵を生成
	spawn_enemy(enemy);
	
}

void BattleScene::update(float delta_time)
{
	/*バトルリザルトシーン*/
	//バトルリザルトシーンが続いている間、バトルシーンは更新しない
	if (!result_scene_.is_end()) {
		result_scene_.update(delta_time);
		return;
	}

	/*バトルシーン*/
	world_.update(delta_time);

	//タイマー更新
	timer_.update(delta_time);

	//戦闘が終了したらリザルトシーンへ
	if (is_settled()) {
		//バトルシーンは実質終了
		is_end_ = true;
		result_scene_.start(data());
		return;
	}

	//ForDebug:シーン遷移チート
	if (Input::get_button(PAD_INPUT_4)) { //A
		is_end_ = true;
		result_scene_.start(data());
		return;
	}
}

void BattleScene::draw() const
{
	//アクターの描画
	world_.draw();
	//HPゲージ描画
	hp_gauge_.draw_gui(static_cast<float>(p_DB_.get_current_parameter().hp));
	//タイマー描画
	timer_.draw();

	//バトル終了状態ではバトルリザルトシーンを描画
	if(is_end_)result_scene_.draw();
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
	Mesh::clear();
	Image::clear();
}

void* BattleScene::data()
{
	//バトル勝敗を保持
	if      (world_.is_battle_win())  result_.battle_result = "Win";
	else if (world_.is_battle_lose()) result_.battle_result = "Lose";
	//敵ごとの討伐数を保持
	result_.basterd_list = world_.basterd_list();
    return &result_;
}

bool BattleScene::is_settled() const
{
	return world_.is_battle_end();
}

void BattleScene::spawn_enemy(const std::string& enemy)
{
	//HACK:せっかく敵の名前とそろえたので、もっと簡潔な記述にならないか？
	if (enemy == "Slime") {
		world_.add_actor(new Slime{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
		world_.add_actor(new Slime{ &world_,  Vector3{ 500.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
		world_.add_actor(new Mimic{ &world_,  Vector3{ -500.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	}
	else if (enemy == "Skeleton") {
		world_.add_actor(new Skeleton{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	}
	else if (enemy == "Mage") {
		world_.add_actor(new Mage{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	}
	else if (enemy == "Mimic") {
		world_.add_actor(new Mimic{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	}
	else if (enemy == "BlackKnight") {
		world_.add_actor(new BlackKnight{ &world_,  Vector3{ 0.0f, 0.0f, 500.0f }, Vector3{ 0.0f, 180.0f, 0.0f } });
	}
	else if (enemy == "None") {
		/*処理の都合上、スタート地点を表すNoneが渡されることはないので、バグである*/
	}
}
