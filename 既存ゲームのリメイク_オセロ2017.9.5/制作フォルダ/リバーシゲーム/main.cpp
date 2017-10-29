//--------------------作業開始日--------------------
//--------------------2017年8月2日------------------
//--------------------一通りの作業終了---------------
//--------------------2017年8月2日------------------
//--------------------AI2を追加---------------------
//--------------------2017年8月4日------------------
//--------------------パス判定を修正-----------------
//--------------------2017年8月16日-----------------
//--------------------パスの判定を修正---------------
//--------------------2017年9月05日-----------------

#include "DxLib.h"
#include "SceneManager.h"
#include "Input.h"

static const int 
SCREEN_WIDIH = 840,
SCREEN_HEIGHT = 640;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ウィンドウモード変更と初期化と裏画面設定
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	//画面サイズを840*640に変更
	SetGraphMode(SCREEN_WIDIH, SCREEN_HEIGHT, 32);
	//タイトルの読み込み
	Init_Title();

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		Updata_Key();			//キー入力を有効にする
		ChangeScene();			//シーンの切り替え
	}
	DxLib_End();			// DXライブラリ終了処理
	return 0;
}
