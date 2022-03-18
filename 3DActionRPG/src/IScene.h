#ifndef ISCENE_H_
#define ISCENE_H_

#include <string>

//シーン抽象インターフェース
class IScene
{
public:
	virtual ‾IScene() = default;
	//開始
	virtual void start(void* data = nullptr) = 0;
	//更新
	virtual void update(float delta_time) = 0;
	//描画
	virtual void draw() const = 0;
	//終了しているか？
	virtual bool is_end() const = 0;
	//次のシーンを返す
	virtual std::string next() const = 0;
	//終了
	virtual void end() = 0;
	//シーンをまたぐデータを取得
	virtual void* data() = 0;
};
#endif//!ISCENE_H_
