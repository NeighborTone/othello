#pragma once
//ゲーム情報
struct Obj
{
	int Handle,		//画像データ格納用
		x,			//カーソル用
		y,
		num;		//コマを数を格納
};
//画像のロード
void Init_Graph();
//ゲーム情報の初期化
void Init_Game();
//盤面の初期化
void Init_Board();
//カーソルの移動
void Updata_cursor();
//vecで指定された向きについてひっくり返るコマがあるか確認する(8方向のチェック)
int CheckFlip(int y, int x, int turn, int vec);
//その場所に置くことができるかを確認
int PutCheck(int y, int x, int turn);
//実際に裏返す処理
void Flip(int y, int x, int turn, int vec);
//入力を受けて裏返せるか確かめる関数
int Put(int y, int x, int turn);
//パスと終了判定
int CheckPass(int turn);
//コマを置く処理(対CPU用)
void Me_PutPice(int by, int bx, int &turn);
//コマを置く処理(2p対戦用)
void You_PutPice(int by, int bx, int &turn);
//CPU置ける所に適当に置く
void AI_PutPice(int by, int bx, int &turn);
//CPU優先順位の高いところに置く
void AI2_PutPice(int by, int bx, int &turn);
//コマを数える
void CountPice();
//1フレームだけ処理をする
int Main_Program(int &turn);	//対CPU
int Main_Program2(int &turn);	//対人間
int Main_Program3(int &turn);	//対CPU2
//盤面の表示
void Draw_Game();
//終了処理
void Game_Fin();

