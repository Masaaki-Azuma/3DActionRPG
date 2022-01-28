#ifndef PLAY_SCENE_H_
#define PLAY_SCENE_H_

#include "IScene.h"

class SceneManager;
class GameManager;

class PlayScene : public IScene
{
public:
    PlayScene();
    virtual void start() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void end() override;

private:
    SceneManager& sm;
    GameManager& gm;
    int score_{ 0 };
};
#endif//!PLAY_SCENE_H_

