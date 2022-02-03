#ifndef SCENE_H_
#define SCENE_H_

#include <string>

//シーン抽象インターフェース
class IScene
{
public:
	IScene() = default;
	virtual ‾IScene() = default;
	//開始処理
	virtual void start() = 0;
	//更新処理
	virtual void update() = 0;
	//描画処理
	virtual void draw() = 0;
	//終了処理
	virtual void end() = 0;

protected:
	std::string name_;
};
#endif//!SCENE_H_

