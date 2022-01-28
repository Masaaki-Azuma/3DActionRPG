#include "GameOverScene.h"

#include <DxLib.h>

#include "Input.h"
#include "SceneManager.h"
#include "GameManager.h"

GameOverScene::GameOverScene():
	sm{SceneManager::GetInstance()},
	gm{GameManager::GetInstance()}
{
	name_ = "GameOverScene";
}

void GameOverScene::start()
{
}

void GameOverScene::update()
{
    if (Input::get_button_down(PAD_INPUT_1))         {
        sm.load("TitleScene"); //シーン遷移
        return;
    }
}

void GameOverScene::draw()
{
    DrawString(0, 0, "GameOver....ボタン押下でPlaySceneへ", GetColor(255, 255, 255));
}

void GameOverScene::end()
{
}
