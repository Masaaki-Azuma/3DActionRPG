#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "Singleton.h"

#include <string>
#include <vector>
#include <memory>

class IScene;

class SceneManager : public Singleton<SceneManager>
{
public:
	//シーンを遷移
	void load(const std::string& scene_name, std::vector<std::string> options = { "" });

public:
	std::string selectStage = "Stage1"; //選択中のステージ
	int score_max{ -1 };  //シーンをまたぐスコア
	std::shared_ptr<IScene> prev_scene{ nullptr };
	std::shared_ptr<IScene> current_scene{ nullptr };

private:
	SceneManager() = default;
	‾SceneManager() = default;
private:
	friend class Singleton<SceneManager>;
};
#endif//!SCENE_MANAGER_H_

