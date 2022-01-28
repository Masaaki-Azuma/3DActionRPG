#ifndef GAME_OVER_SCENE_H_
#define GAME_OVER_SCENE_H_

#include "IScene.h"

class SceneManager;
class GameManager;

class GameOverScene : public IScene
{
public:
    GameOverScene();
    virtual void start() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void end() override;

private:
    SceneManager& sm;
    GameManager& gm;

};
#endif//!GAME_OVER_SCENE_H_
