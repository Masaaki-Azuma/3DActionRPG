#include "TitleScene.h"

#include <DxLib.h>

#include "Input.h"
#include "GameManager.h"
#include "SceneManager.h"

TitleScene::TitleScene():
	gm{GameManager::GetInstance()},
	sm{SceneManager::GetInstance()}
{
	name_ = "TitleScene";
}

void TitleScene::start()
{

}

void TitleScene::update()
{
	if (Input::get_button_down(PAD_INPUT_1)) {
		sm.load("PlayScene", { "stage1" });
		return;
	}

}

void TitleScene::draw()
{
	DrawString(0, 0, "TitleSceneです。ボタン押下でPlaySceneへ。", GetColor(255, 255, 255));
}

void TitleScene::end()
{
}
