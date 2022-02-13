#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <string>
#include <unordered_map>
#include "Fade.h"

class IScene;

class SceneManager
{
public:
	SceneManager();
	‾SceneManager();
	//更新
	void update(float delta_time);
	//描画
	void draw() const;
	//終了
	void end();  //private?
	//シーンの追加
	void add(const std::string& name, IScene* scene);
	//シーンの変更
	void change(const std::string& name);  //private?
	//シーンの消去
	void clear();  //private?

	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;

private:
	//シーン
	std::unordered_map<std::string, IScene*> scenes_;
	//現在のシーン
	IScene* current_scene_;
	//フェード演出
	Fade fade_;

};
#endif//!SCENE_MANAGER_H_
