#ifndef SCENE_NULL_H_
#define SCENE_NULL_H_

#include "IScene.h"

class SceneNull :
    public IScene
{
	//開始
	virtual void start(void* data = nullptr) override{}
	//更新
	virtual void update(float delta_time) override{}
	//描画
	virtual void draw() const override{}
	//終了しているか？
	virtual bool is_end() const override { return false; }
	//次のシーンを返す
	virtual std::string next() const override { return "SceneNull"; }
	//終了
	virtual void end() override {}
	//シーンをまたぐデータを取得
	virtual void* data() override { return nullptr; }
};
#endif//!SCENE_NULL_H_

