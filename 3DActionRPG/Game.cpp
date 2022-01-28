#include "Game.h"

#include <memory>

#include "Image.h"
#include "Screen.h"
#include "Input.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "PlayerParameter.h"
#include "IScene.h"
#include "Player.h"
#include "PlayerBullet.h"

Game::Game():
	sm{SceneManager::GetInstance()},
	gm{GameManager::GetInstance()},
	player_paremeter{PlayerParameter::GetInstance()}
{
}

void Game::init()
{
	Image::load();
	Input::init();
	player_paremeter.load();
	sm.load("TitleScene");
}

void Game::update()
{
	Input::update();

	sm.current_scene->update();
}

void Game::draw()
{
	sm.current_scene->draw();
}

void Game::end()
{
	player_paremeter.save();
}
