#include "DxLib.h"
#include "Input.h"
#include "Game.h"
#include "SceneManager.h"
#include "Title.h"

 enum GameData
{
	End = 2,		  //終了
};

enum Scene
{
	//必要に応じて追加
	Title,
	Game_cpu,
	Game_cpu2,
	Game_human

};Scene scene;

extern int turn;

//タイトルシーンの初期化
void Init_Title()
{
	TitleGraph_Init();
}
//ゲーム本編の初期化
void Init_MainGame()
{
	Init_Graph();
	Init_Board();
	Init_Game();
}
//シーン管理
void ChangeScene()
{
	switch (scene)
	{
		//タイトルシーン----------------------
	case Title:
		Draw_Title();
		if (Key(KEY_INPUT_S) == 1)
		{
			//ゲームの初期化
			Init_MainGame();
			//タイトルのデータを削除
			Title_Fin();
			//シーンをゲーム本編(CPU)へ
			scene = Game_cpu;
		}
		if (Key(KEY_INPUT_A) == 1)
		{
			//ゲームの初期化
			Init_MainGame();
			//タイトルのデータを削除
			Title_Fin();
			//シーンをゲーム本編(CPU)へ
			scene = Game_cpu2;
		}
		if (Key(KEY_INPUT_W) == 1)
		{
			//ゲームの初期化
			Init_MainGame();
			//シーンをゲーム本編(対人)へ
			scene = Game_human;
		}
		break;
		//----------------------------------

		//ゲームシーン1----------------------
	case Game_cpu:
		//毎フレーム処理をする
		Main_Program(turn);		//対CPU
		//リトライ
		if (turn >= End && Key(KEY_INPUT_R) == 1)
		{
			//ゲームの初期化
			Init_MainGame();
		}
		//シャットダウン
		if (turn >= End && Key(KEY_INPUT_F) == 1)
		{
			InitGraph();	//すべての画像データを削除	
			DxLib_End();	//DXライブラリ終了処理
		}
		//タイトルに戻る
		if (turn >= End && Key(KEY_INPUT_T) == 1)
		{
			//タイトルの読み込み
			Init_Title();
			//ゲーム本編のデータを削除
			Game_Fin();
			scene = Title;
		}
		break;
		//----------------------------------

		//ゲームシーン2----------------------
	case Game_cpu2:
		//毎フレーム処理をする
		Main_Program3(turn);	//対CPU2
		//リトライ
		if (turn >= End && Key(KEY_INPUT_R) == 1)
		{
			//ゲームの初期化
			Init_MainGame();
		}
		//シャットダウン
		if (turn >= End && Key(KEY_INPUT_F) == 1)
		{
			InitGraph();	//すべての画像データを削除	
			DxLib_End();	//DXライブラリ終了処理
		}
		//タイトルに戻る
		if (turn >= End && Key(KEY_INPUT_T) == 1)
		{
			//タイトルの読み込み
			Init_Title();
			//ゲーム本編のデータを削除
			Game_Fin();
			scene = Title;
		}
		break;
		//----------------------------------

		//ゲームシーン3----------------------
	case Game_human:
		Main_Program2(turn);	//対人間
		//リトライ
		if (turn >= End && Key(KEY_INPUT_R) == 1)
		{
			//ゲームの初期化
			Init_MainGame();
		}
		//シャットダウン
		if (turn >= End && Key(KEY_INPUT_F) == 1)
		{
			InitGraph();		//すべての画像データを削除	
			DxLib_End();		//DXライブラリ終了処理
		}
		//タイトルに戻る
		if (turn >= End && Key(KEY_INPUT_T) == 1)
		{
			//タイトルの読み込み
			Init_Title();
			//ゲーム本編のデータを削除
			Game_Fin();
			scene = Title;
		}
		break;
		//-----------------------------------
	}
}
