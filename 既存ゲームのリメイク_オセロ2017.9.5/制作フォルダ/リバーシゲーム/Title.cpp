#include "DxLib.h"
#include "SceneManager.h"
#include "Title.h"

static const int 
	SCREEN_WIDIH = 840,
	SCREEN_HEIGHT = 640,
	CELLSIZE = 80;	  //マス一つ分の大きさ
int TitleGraph;

//タイトルシーンに使う画像の初期化
void TitleGraph_Init()
{
	TitleGraph = LoadGraph("Graph/ボード.bmp");
}
void Draw_Title()
{
	DrawFormatString(670, 300, GetColor(255, 255, 255), "Sで対CPU戦(弱い)");
	DrawFormatString(670, 320, GetColor(255, 255, 255), "Aで対CPU戦2(超弱い)");
	DrawFormatString(670, 340, GetColor(255, 255, 255), "Wで対人戦");
	DrawGraph(0, 0, TitleGraph, false);
	//グリッド線を引く
	for (int x = 0; x < 9; ++x)
	{
		//縦線
		DrawLineAA((float)CELLSIZE * x, 0.f, (float)CELLSIZE * x, (float)SCREEN_HEIGHT, GetColor(0, 0, 0), 1.4f);
	}
	for (int y = 0; y < 9; ++y)
	{
		//横線
		DrawLineAA(0.f, (float)CELLSIZE * y, (float)SCREEN_HEIGHT, (float)CELLSIZE * y, GetColor(0, 0, 0), 1.4f);
	}
}
//タイトルシーンのデータを削除
void Title_Fin()
{
	DeleteGraph(TitleGraph);	//タイトル画像を削除
}