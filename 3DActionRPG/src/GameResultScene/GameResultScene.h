#ifndef GAME_RESULT_SCENE_H_
#define GAME_RESULT_SCENE_H_

#include "IScene.h"

#include "AssetsManager/PlayerDatabase.h"

class Image;

//プレイ1周分が終了した後の結果表示シーン
class GameResultScene :
    public IScene
{
public:
    GameResultScene();
    virtual void start(void* data = nullptr) override;
    virtual void update(float delta_time) override;
    virtual void draw() const override;
    virtual bool is_end() const override;
    virtual std::string next() const override;
    virtual void end() override;
    virtual void* data() override;

private:
    bool is_end_{ false };
    PlayerDatabase& p_DB_{ PlayerDatabase::GetInstance() };
    Image& image_;
};
#endif//GAME_RESULT_SCENE_H_

