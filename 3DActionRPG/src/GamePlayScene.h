#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include "IScene.h"
#include "World.h"

class GamePlayScene :
    public IScene
{
	//開始
	virtual void start(void* data = nullptr) override;
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//終了しているか？
	virtual bool is_end() const override;
	//次のシーンを返す
	virtual std::string next() const override;
	//終了
	virtual void end() override;
	//シーンをまたぐデータを取得
	virtual void* data() override;

private:
	
	//
	bool is_end_;
};
#endif//!GAME_PLAY_SCENE_H_
