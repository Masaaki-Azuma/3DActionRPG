#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "IScene.h"

class SceneManager;
class GameManager;

class TitleScene : public IScene
{
public:
	TitleScene();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() override;
	virtual void end() override;

private:
	SceneManager& sm;
	GameManager& gm;
};
#endif//!TITLE_SCENE_H

