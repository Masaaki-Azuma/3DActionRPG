#include "Application.h"

#include <DxLib.h>

#include "Stopwatch.h"
#include "Game.h"
#include "Screen.h"

float Application::currentFPS_{ 0.0f };

const float Application::CurrentFPS() {
    return currentFPS_;
}

void Application::Run() {
    const int TargetFPS = 60; // 目標のFPS(Frame Per Second, 1秒あたりのフレーム数)
    const bool EnableFrameSkip = true; // 高負荷時にフレームスキップするか（falseの場合は処理落ち（スロー））
    const double MaxAllowSkipTime = 0.2; // フレームスキップする最大間隔（秒）。これ以上の間隔が空いた場合はスキップせずに処理落ちにする。
    const LONGLONG IntervalTicks = static_cast<LONGLONG>(1.0 / TargetFPS * 10000000); // フレーム間のTick数。1Tick = 100ナノ秒 = 1/10000000秒
    const int MaxAllowSkipCount = static_cast<int>(TargetFPS * MaxAllowSkipTime);

    LONGLONG nextFrameTicks = IntervalTicks; // 次のフレームの目標時刻
    Stopwatch stopwatch; // FPS制御のために時間を計るための高精度タイマー
    LONGLONG fpsTicks = 0; // FPS計測のためのTicks。
    int fpsFrameCount = 0; // FPS計測のためのフレームカウント。60回数えるごとに、要した時間からFPSを算出する。

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST); // スレッドの優先度を上げておく

    // 画面リフレッシュレートと目標フレームレートが等しい場合は垂直同期を有効に、等しくない場合は垂直同期を無効にする
    SetWaitVSyncFlag(GetRefreshRate() == TargetFPS ? TRUE : FALSE);
    SetWindowText("ゲームタイトル"); // ウィンドウのタイトル
    SetGraphMode(Screen::Width, Screen::Height, 32); // ウィンドウサイズ（画面解像度）の指定
    ChangeWindowMode(TRUE); // ウィンドウモードにする（FALSEを指定するとフルスクリーンになる）
    SetAlwaysRunFlag(TRUE); // ウィンドウが非アクティブでも動作させる

    DxLib_Init(); // DXライブラリの初期化

    SetMouseDispFlag(TRUE); // マウスを表示する（FALSEを指定すると非表示になる）
    SetDrawScreen(DX_SCREEN_BACK); // 描画先を裏画面とする（ダブルバッファ）
    SetUseTransColor(FALSE); // 画像の指定色を透過する機能を無効化
    SetUseZBuffer3D(TRUE);  //描画時にZバッファを使用する
    SetWriteZBuffer3D(TRUE);  //Zバッファに書き込む
    SetUseBackCulling(TRUE);  //ポリゴン裏面を描画しない

    game_ = new Game();
    game_->Init();

    ScreenFlip();
    stopwatch.Start();

    while (ProcessMessage() == 0) // ウィンドウが閉じられるまで繰り返す
    {
        // FPSの計測
        fpsFrameCount++;
        if (fpsFrameCount >= 60)
        {
            LONGLONG elapsedTicks = stopwatch.ElapsedTicks() - fpsTicks;
            float elapsedSec = elapsedTicks / 10000000.0f;
            currentFPS_ = fpsFrameCount / elapsedSec;

            fpsFrameCount = 0;
            fpsTicks = stopwatch.ElapsedTicks();
        }

        game_->Update();

        if (GetWaitVSyncFlag() == TRUE) {
            if (EnableFrameSkip)
            {
                LONGLONG waitTicks = nextFrameTicks - stopwatch.ElapsedTicks(); // 余った時間

                if (waitTicks < 0) // 目標時刻をオーバーしている
                {
                    if (skipCount_ < MaxAllowSkipCount) // 連続フレームスキップ数が最大スキップ数を超えていなければ
                    {
                        skipCount_++; // フレームスキップ（描画処理を飛ばす）
                    }
                    else
                    {
                        // 最大スキップ数を超えてるので、フレームスキップしないで描画
                        nextFrameTicks = stopwatch.ElapsedTicks();
                        Draw();
                    }
                }
                else
                {
                    Draw();
                }

                nextFrameTicks += IntervalTicks;
            }
            else
            {
                Draw();
            }
        }
        else
        {
            LONGLONG waitTicks = nextFrameTicks - stopwatch.ElapsedTicks(); // 余った時間（待機が必要な時間）

            if (EnableFrameSkip && waitTicks < 0)
            {
                if (skipCount_ < MaxAllowSkipCount)
                {
                    skipCount_++; // フレームスキップ（描画処理を飛ばす）
                }
                else
                {
                    nextFrameTicks = stopwatch.ElapsedTicks();
                    Draw();
                }
            }
            else
            {
                if (waitTicks > 20000) // あと2ミリ秒以上待つ必要がある
                {
                    // Sleep()は指定した時間でピッタリ戻ってくるわけではないので、
                    // 余裕を持って、「待たなければならない時間 - 2ミリ秒」Sleepする
                    DWORD waitMillsec = static_cast<DWORD>(waitTicks / 10000LL) - 2L;
                    Sleep(waitMillsec);
                }

                // 時間が来るまで何もしないループを回して待機する
                while (stopwatch.ElapsedTicks() < nextFrameTicks)
                {
                }

                Draw();
            }
            nextFrameTicks += IntervalTicks;
        }
    }
    game_->End();
    delete game_;
    DxLib_End(); // DXライブラリ終了処理
}

void Application::Draw() {
    ClearDrawScreen(); // 描画先の内容をクリアする
    game_->Draw(); // ゲーム描画
    ScreenFlip(); // 裏画面と表画面を入れ替える
    skipCount_ = 0; // フレームスキップのカウントをリセット
}
