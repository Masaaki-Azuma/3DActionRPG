#include "SceneManager.h"

#include <memory>

#include "IScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "GameOverScene.h"

void SceneManager::load(const std::string& scene_name, std::vector<std::string> options)
{
	//現在のシーンを終了
	if (current_scene != nullptr) current_scene->end();

	//HACK:if-elseを削れないか？
	if (scene_name == "TitleScene") {
		current_scene = std::make_shared<TitleScene>();
	}
	else if (scene_name == "PlayScene") {
		//オプションが有効なら選択ステージに反映
		if (options.size() > 0 && options[0] != "") {
			selectStage = options[0];
		}
		current_scene = std::make_shared<PlayScene>();
	}
	else if (scene_name == "GameOverScene") {
		current_scene = std::make_shared<GameOverScene>();
	}

	//次のシーンを開始
	current_scene->start();
}
