#include <DxLib.h>
#include "Screen.h"
#include "Image.h"
#include "Game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    // 画面モードの設定
    SetMainWindowText("ゲームのウィンドウ名を変えるときはここ");//この行でエラーになったら【設定】マルチバイト文字セットが間違ってるかも
    SetGraphMode(Screen::Width, Screen::Height, 32); // カラービット数32ビットで起動
    SetWindowSize(Screen::Width, Screen::Height);// ウィンドウサイズ(こことSetGraphModeのサイズが異なると画像がゆがむ)
    ChangeWindowMode(TRUE);//フルスクリーン表示かウィンドウ表示か
    SetAlwaysRunFlag(TRUE); // ウィンドウが非アクティブでも動作させる
    SetMouseDispFlag(TRUE);// ここをFALSEにするとマウスカーソル非表示

    //↑ここまでの設定は↓下のDXライブラリ初期化より先にやらないとDxLib_Init()中は画面がフルスクリーンになって終わってからウィンドウサイズが変更になり見苦しい

    // ＤＸライブラリの初期化
    if (DxLib_Init() < 0)   {
        // DXの初期化にエラーが発生したらプログラム自体を終了(returnで)
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    Game game;
    /*初期化処理*/
    game.init();
 
    ScreenFlip();

    while (ProcessMessage() == 0) {
        ClearDrawScreen();

        /*更新処理*/
        game.update();

        /*描画処理*/
        game.draw();

        ScreenFlip();
    }
    /*終了処理*/
    game.end();
    // キー入力待ちをする
    WaitKey();
    // ＤＸライブラリの後始末
    DxLib_End();
    // ソフトの終了
    return 0;
}