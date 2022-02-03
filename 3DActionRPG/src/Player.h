#pragma once

#include "Actor.h"

class GameManager;

//HACK:動作確認クラス
class Player : public Actor
{
public:
	Player(float x, float y);
	‾Player() {};
	void update();
	void draw();

private:
	GameManager& gm;
};

