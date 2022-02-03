#include "PlayScene.h"

#include "Input.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "PlayerParameter.h"
#include "Player.h"
#include "PlayerBullet.h"

PlayScene::PlayScene():
	sm{SceneManager::GetInstance()},
	gm{GameManager::GetInstance()}
{
	name_ = "PlayScene";
}

void PlayScene::start()
{
	gm.player = std::make_shared<Player>(100, Screen::Height / 2);
}

void PlayScene::update()
{
	//アクターの更新
	gm.player->update();
	for (auto& b : gm.player_bullets) {
		b->update();
	}
	//!アクターの更新

	//アクター間の衝突判定
	gm.erase_remove_if(gm.player_bullets,
		[](std::shared_ptr<PlayerBullet>& ptr) {return ptr->isDead; });
	//!アクター間の衝突判定

	//シーン終了判定
	bool is_game_over = (gm.player == nullptr);
	if (!is_game_over && gm.player->is_dead) is_game_over = true;
	if (Input::get_button_down(PAD_INPUT_1)) {
		sm.load("GameOverScene");
		return;
	}
}

void PlayScene::draw()
{
	//アクターの描画
	gm.player->draw();
	for (const auto& b : gm.player_bullets) {
		b->draw();
	}
	//!アクターの描画
}

void PlayScene::end()
{
	//ここにシーン内のメモリ解放処理を記述
	gm.player = nullptr;
	gm.player_bullets.clear();

	//シーンをまたぐデータを記録
	if (score_ > sm.score_max) sm.score_max = score_;
}
