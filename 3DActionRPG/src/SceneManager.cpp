#include "SceneManager.h"

#include <cassert>

#include "SceneNull.h"

static SceneNull scene_null; //シーンマネージャーの処理の単純化にのみ必要

SceneManager::SceneManager():
	current_scene_{&scene_null}
{
}

SceneManager::‾SceneManager()
{
	clear();
}

void SceneManager::update(float delta_time)
{
	//フェード中ならシーンの更新を止める
	if (fade_.IsRunning()) {
		fade_.Update();
		if (fade_.IsClosed()) {
			//シーンを変更する
			change(current_scene_->next());
		}
		return;
	}

	//シーンの更新
	current_scene_->update(delta_time);
	//シーンが終了しているか?
	if (current_scene_->is_end()) {
		fade_.Start(Fade::State::FADEOUT);
	}
}

void SceneManager::draw() const
{
	//現在のシーンを描画
	current_scene_->draw();
	//フェード演出を描画
	fade_.Draw();
}

void SceneManager::end()
{
	//現在のシーンを終了
	current_scene_->end();
	//nullシーンにしておく
	current_scene_ = &scene_null;
}

void SceneManager::add(const std::string& name, IScene* scene)
{
	scenes_[name] = scene;
}

void SceneManager::change(const std::string& name)
{
	assert(scenes_.count(name) != 0 && "無効なシーン名が渡されました");
	//前シーンから引き継ぐデータを取得
	void* data = current_scene_->data();
	//現在のシーンを終了
	end();
	//現在のシーンを変更
	current_scene_ = scenes_[name];
	//現在のシーンを開始
	current_scene_->start(data);
	//
	fade_.Start(Fade::State::FADEIN);
}

void SceneManager::clear()
{
	//シーンを終了
	end();
	//シーンを全て削除
	for (auto& pair : scenes_) {
		delete pair.second;  //シーンの削除
	}
	//unordered_mapを削除
	scenes_.clear();  //ダングリングポインタへのアクセスを防ぐ
}
