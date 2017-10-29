#include "DxLib.h"
#include "Input.h"
#include "Game.h"
#include "SceneManager.h"

//確定的な値
static const int 
SCREEN_WIDIH = 840,				 
SCREEN_HEIGHT = 640,	
//8方向の判定用
vec_y[] = { -1,-1,0,1,1,1,0,-1 },
vec_x[] = { 0,1,1,1,0,-1,-1,-1 };

//途中で変更する可能性のある値
enum GameData
{
	Me = 0,			  //自分のターン
	You = 1,		  //相手のターン
	End = 2,		  //終了
	EMPTY = 0,		  //空きマス
	BLACK = 1,		  //黒コマ
	WHITE = -1,		  //白コマ
	BOARDSIZE = 8,	  //ボードの大きさ,8 * 8
	CELLSIZE = 80,	  //マス一つ分の大きさ
	CURSOR_MAX = 480  //カーソルの移動できる最大値
};


Obj BoardGraph;
Obj Black;
Obj White;
Obj cursor;
int board_x;		//引数にボードの大きさを格納するために用意
int board_y;
int turn;
int turncount;		//経過ターンを格納
bool Gameflag;		//更新処理内で1フレームだけ処理を行う場合に使う
int Board[BOARDSIZE][BOARDSIZE];
//画像の初期化
void Init_Graph()
{
	BoardGraph.Handle = LoadGraph("Graph/ボード.bmp");
	cursor.Handle = LoadGraph("Graph/カーソル.png");
	Black.Handle = LoadGraph("Graph/黒.png");
	White.Handle = LoadGraph("Graph/白.png");
}
//ゲーム情報の初期化
void Init_Game()
{
	turn = Me;	//ターン、Meは黒が先手
	Gameflag = false;
	board_x = BOARDSIZE,
	board_y = BOARDSIZE;
	Black.num = 0;
	White.num = 0;
	cursor.x = 0;
	cursor.y = 0;
	turncount = 0;
}
//ボードの初期化
void Init_Board()
{

	for (int y = 0; y < BOARDSIZE; ++y)
	{
		for (int x = 0; x < BOARDSIZE; ++x)
		{
			Board[y][x] = EMPTY;
		}
	}
	Board[BOARDSIZE / 2 - 1][BOARDSIZE / 2] = BLACK;		//[3][4]
	Board[BOARDSIZE / 2][BOARDSIZE / 2 - 1] = BLACK;		//[4][3]
	Board[BOARDSIZE / 2][BOARDSIZE / 2] = WHITE;			//[4][4]
	Board[BOARDSIZE / 2 - 1][BOARDSIZE / 2 - 1] = WHITE;	//[3][3]

#if _DEBUG
	//デバッグ用
	/*Board[0][0] = WHITE;
	Board[0][1] = WHITE;
	Board[0][2] = WHITE;
	Board[0][3] = WHITE;
	Board[0][4] = WHITE;
	Board[0][5] = WHITE;
	Board[0][6] = BLACK;
	Board[0][7] = BLACK;
	Board[1][0] = WHITE;
	Board[1][1] = WHITE;
	Board[1][2] = WHITE;
	Board[1][3] = WHITE;
	Board[1][4] = WHITE;
	Board[1][5] = WHITE;
	Board[1][6] = BLACK;
	Board[1][7] = BLACK;
	Board[2][0] = WHITE;
	Board[2][1] = WHITE;
	Board[2][2] = WHITE;
	Board[2][3] = WHITE;
	Board[2][4] = WHITE;
	Board[2][5] = WHITE;
	Board[2][6] = BLACK;
	Board[2][7] = BLACK;
	Board[3][0] = WHITE;
	Board[3][1] = WHITE;
	Board[3][2] = WHITE;
	Board[3][3] = WHITE;
	Board[3][4] = WHITE;
	Board[3][5] = WHITE;
	Board[3][6] = BLACK;
	Board[3][7] = BLACK;
	Board[4][0] = WHITE;
	Board[4][1] = WHITE;
	Board[4][2] = WHITE;
	Board[4][3] = WHITE;
	Board[4][4] = WHITE;
	Board[4][5] = WHITE;
	Board[4][6] = BLACK;
	Board[4][7] = BLACK;
	Board[5][0] = WHITE;
	Board[5][1] = WHITE;
	Board[5][2] = WHITE;
	Board[5][3] = WHITE;
	Board[5][4] = WHITE;
	Board[5][5] = WHITE;
	Board[5][6] = WHITE;
	Board[5][7] = BLACK;
	Board[6][0] = WHITE;
	Board[6][1] = WHITE;
	Board[6][2] = WHITE;
	Board[6][3] = WHITE;
	Board[6][4] = WHITE;
	Board[6][5] = WHITE;
	Board[6][6] = WHITE;
	Board[6][7] = BLACK;
	Board[7][0] = BLACK;
	Board[7][1] = BLACK;
	Board[7][2] = BLACK;
	Board[7][3] = BLACK;
	Board[7][4] = BLACK;
	Board[7][5] = BLACK;
	Board[7][6] = EMPTY;
	Board[7][7] = BLACK;*/


#else
	Board[BOARDSIZE / 2 - 1][BOARDSIZE / 2] = BLACK;
	Board[BOARDSIZE / 2][BOARDSIZE / 2 - 1] = BLACK;
	Board[BOARDSIZE / 2][BOARDSIZE / 2] = WHITE;    
	Board[BOARDSIZE / 2 - 1][BOARDSIZE / 2 - 1] = WHITE;
#endif

}
//カーソルの移動
void Updata_cursor()
{
	if (Key(KEY_INPUT_RIGHT) == 1 && cursor.x <= CURSOR_MAX)
	{
		cursor.x += CELLSIZE;
	}
	if (Key(KEY_INPUT_LEFT) == 1 && cursor.x > 0)
	{
		cursor.x -= CELLSIZE;
	}
	if (Key(KEY_INPUT_UP) == 1 && cursor.y > 0)
	{
		cursor.y -= CELLSIZE;
	}
	if (Key(KEY_INPUT_DOWN) == 1 && cursor.y <= CURSOR_MAX)
	{
		cursor.y += CELLSIZE;
	}
}
//vecで指定された向きについてひっくり返るコマがあるか確認する(8方向のチェック)
int CheckFlip(int y, int x, int turn, int vec)	//確認出来たらTRUEを返す(ひっくり返せるコマがある)
{
	int flag = FALSE;
	while (TRUE)
	{
		//8方向の走査
		y += vec_y[vec];
		x += vec_x[vec];

		//盤面の外に出ていたら終了
		if (x < 0 || y < 0 || x > BOARDSIZE - 1 || y > BOARDSIZE - 1)
		{
			return FALSE;
		}
		//空きマスだったら終了
		if (Board[y][x] == EMPTY)
		{
			return FALSE;
		}
		//相手のコマがあったらフラグを立てる
		if (Board[y][x] == (turn ? BLACK : WHITE))
		{
			flag = TRUE;
		}
		//自分のコマがあった場合にフラグが立っていれば置ける
		if (Board[y][x] == (turn ? WHITE : BLACK))
		{
			//置ける
			if (flag == TRUE)
			{
				return TRUE;
			}
			//置けない
			else
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
//その場所に置くことができるかを確認
int PutCheck(int y, int x, int turn)	//置ければTRUEを返す
{
	int vec;
	//どれか1方向でもひっくり返せるか確認
	for (vec = 0; vec < BOARDSIZE; ++vec)
	{
		if (CheckFlip(y, x, turn, vec) == TRUE)
		{
			return TRUE;	//ひっくり返せる
		}
	}
	return FALSE;
}
//実際に裏返す処理
void Flip(int y, int x, int turn, int vec)
{
	while (TRUE)
	{
		y += vec_y[vec];
		x += vec_x[vec];

		//自分のコマがあったら終了	
		if (Board[y][x] == (turn ? WHITE : BLACK))
		{
			break;
		}
		//それ以外なら自分のコマで塗りつぶす
		Board[y][x] = (turn ? WHITE : BLACK);
	}
}
//入力を受けて裏返せるか確かめる関数
int Put(int y, int x, int turn)
{
	int vec,
		flag = FALSE;
	//空きマスでなければ終了
	if (Board[y][x] != EMPTY)
	{
		return FALSE;
	}
	//全方向について確認
	for (vec = 0; vec < BOARDSIZE; ++vec)
	{
		if (CheckFlip(y, x, turn, vec) == TRUE)
		{
			//裏返す
			Flip(y, x, turn, vec);
			flag = TRUE;
		}
	}
	if (flag == TRUE)
	{
		//この場所にコマを置く
		Board[y][x] = (turn ? WHITE : BLACK);
		return TRUE;
	}
	return FALSE;
}
//パスと終了判定(ターンチェンジ)
int CheckPass(int turn)
{
	int y,
		x;
	//おける場所があるか確認
	for (y = 0; y < BOARDSIZE; ++y)
	{
		for (x = 0; x < BOARDSIZE; ++x)
		{
			//あれば普通に続行
			//自分の手番
			if (Board[y][x] == EMPTY && PutCheck(y, x, turn) == TRUE)
			{
				return (turn == Me ? Me : You);			//交互に置く
			}
		}
	}
	//場所がなかったので交替して探す
	//相手の手番
	if (PutCheck(y, x, turn) == FALSE && turn == You)	//(自分が置けない。かつ相手の手番である)
	{
		return Me;					//パス
	}
	if (PutCheck(y, x, turn) == TRUE && turn == You)    //相手が置けない。かつ自分が置けるとき
	{
		return Me;
	}
	//なかったのでゲーム終了
	return End;
}
//コマを置く処理
void Me_PutPice(int by, int bx, int &turn)
{
	for (int y = 0; y < BOARDSIZE; ++y)
	{
		for (int x = 0; x < BOARDSIZE; ++x)
		{
			if (Key(KEY_INPUT_Z) == 1 && CELLSIZE * x == cursor.x && CELLSIZE * y == cursor.y && turn == Me)
			{
				if (Put(y, x, turn) == TRUE)
				{
					Board[by][bx] = BLACK;
					turn = You;
				}
			}
		}
	}
}
//CPU1,置ける所に適当に置く
void AI_PutPice(int by, int bx, int &turn)
{
	for (int y = 0; y < BOARDSIZE; ++y)
	{
		for (int x = 0; x < BOARDSIZE; ++x)
		{
			if  (turn == You)
			{
				if (Put(y, x, turn) == TRUE)
				{
					Board[by][bx] = WHITE;
					turn = Me;
				}
			}
		}
	}
}
//CPU2,ターン経過で置く場所を増やす
void AI2_PutPice(int by, int bx, int &turn)
{
	//盤面の評価値
	int priority[BOARDSIZE][BOARDSIZE] =
	{
		{ 120, 20, 20, 5, 5, 20, 20, 120 },
		{ 20,  40, 5,  5, 5, 5,  40, 20 },
		{ 20,  5,  15, 3, 3, 15, 5,  20 },
		{ 5,   5,  3,  3, 3, 3,  5,  5 },
		{ 5,   5,  3,  3, 3, 3,  5,  5 },
		{ 20,  40, 5,  5, 5, 5,  40, 20 },
		{ 120, 20, 20, 5, 5, 20, 20, 120 },
	};
	for (int y = 0; y < BOARDSIZE; ++y)
	{
		for (int x = 0; x < BOARDSIZE; ++x)
		{
			if (turn == You)
			{
				//序盤は評価値が5以下のところから置く
				if (priority[y][x] <= 5)
				{
					if (Put(y, x, turn) == TRUE)
					{
						Board[by][bx] = WHITE;
						++turncount;
						turn = Me;
					}
				}
				//経過ターン数が3以上になったら評価値15以下のところにも置く
				else if(turncount >= 3 && priority[y][x] <= 15)
				{
					if (Put(y, x, turn) == TRUE)
					{
						Board[by][bx] = WHITE;
						++turncount;
						turn = Me;
					}
				}
				//経過ターン数が6以上になったら評価値40以下のところにも置く
				else if (turncount >= 6 && priority[y][x] <= 40)
				{
					if (Put(y, x, turn) == TRUE)
					{
						Board[by][bx] = WHITE;
						++turncount;
						turn = Me;
					}
				}
				//経過ターン数が8以上になったら評価値120以下のところにも置く
				else if (turncount >= 8 && priority[y][x] <= 120)
				{
					if (Put(y, x, turn) == TRUE)
					{
						Board[by][bx] = WHITE;
						turn = Me;
					}
				}
			}	
		}
	}
}
//対人
void You_PutPice(int by, int bx, int &turn)
{
	for (int y = 0; y < BOARDSIZE; ++y)
	{
		for (int x = 0; x < BOARDSIZE; ++x)
		{
			if (Key(KEY_INPUT_Z) == 1 && CELLSIZE * x == cursor.x && CELLSIZE * y == cursor.y && turn == You)
			{
				if (Put(y, x, turn) == TRUE)
				{
					Board[by][bx] = WHITE;
					turn = Me;
				}
			}
		}
	}
}
//コマを数える
void CountPice()
{
	if (turn == End)
	{
		for (int y = 0; y < BOARDSIZE; ++y)
		{
			for (int x = 0; x < BOARDSIZE; ++x)
			{
				switch (Board[y][x])
				{
				case BLACK:
					++Black.num;
					break;
				case WHITE:
					++White.num;
					break;
				default:
					break;
				}
			}
		}
	}
}
//ゲーム情報の表示
void Draw_Game()
{
	//盤面
	DrawGraph(0, 0, BoardGraph.Handle, false);
	//文字
	if (turn == Me || turn == You)
	{
		DrawFormatString(680, 300, GetColor(255, 255, 255), "十字キーで移動");
		DrawFormatString(680, 320, GetColor(255, 255, 255), "Zで置きます");
	}
	if (turn == Me)
	{
		DrawFormatString(680, 50, GetColor(255, 255, 255), "黒(自分)のターン");
	}
	if (turn == You)
	{
		DrawFormatString(680, 50, GetColor(255, 255, 255), "白(相手)のターン");
	}
	if (turn == End)
	{
		DrawFormatString(680, 300, GetColor(255, 255, 255), "Rでリトライ");
		DrawFormatString(680, 320, GetColor(255, 255, 255), "Tでタイトル");
		DrawFormatString(680, 340, GetColor(255, 255, 255), "Fでシャットダウン");

		if (Black.num > White.num)
		{
			DrawFormatString(680, 100, GetColor(255, 255, 255), "黒の勝ちです");
		}
		else if (Black.num < White.num)
		{
			DrawFormatString(680, 100, GetColor(255, 255, 255), "白の勝ちです");
		}
		else
		{
			DrawFormatString(680, 100, GetColor(255, 255, 255), "引き分けです");
		}
	}
	//コマ
	for (int y = 0; y < BOARDSIZE; ++y)
	{
		for (int x = 0; x < BOARDSIZE; ++x)
		{
			//ボードの情報がWHITEなら白コマが置かれる
			if (Board[y][x] == WHITE)
			{
				DrawGraph(CELLSIZE * x, CELLSIZE * y, White.Handle, true);
			}
			//ボードの情報がBLACKなら黒コマが置かれる
			if (Board[y][x] == BLACK)
			{
				DrawGraph(CELLSIZE * x, CELLSIZE * y, Black.Handle, true);
			}
		}
	}
	//グリッド線を引く
	for (int x = 0; x < 9; ++x)
	{
		//縦線
		DrawLineAA((float)CELLSIZE * x, 0.f, (float)CELLSIZE * x, (float)SCREEN_HEIGHT, GetColor(0, 0, 0), 1.4f);
	}
	for (int y = 0; y < 9; ++y)
	{
		//横線
		DrawLineAA(0.f, (float)CELLSIZE * y, (float)SCREEN_HEIGHT, (float)CELLSIZE * y, GetColor(0 ,0 ,0), 1.4f);
	}
	//カーソルの表示
	DrawGraph(cursor.x, cursor.y, cursor.Handle, true);
}
//終了処理
void Game_Fin()
{
	//画像などのデータを削除(タイトルに戻るときに呼び出す)
	DeleteGraph(Black.Handle);
	DeleteGraph(White.Handle);
	DeleteGraph(cursor.Handle);
	DeleteGraph(BoardGraph.Handle);
}
//以下は上記の処理をまとめたもの----------------------------------------------------------------------------------
//毎フレームごとに処理を行う
int Main_Program(int &turn)		//対CPU
{
	while(Gameflag == false)
	{
		Updata_cursor();
		Me_PutPice(board_y, board_x, turn);
		AI_PutPice(board_y, board_x, turn);
		turn = CheckPass(turn);				//CheckPassの戻り値をturnに入れる(ターンの切り替え)
		Gameflag = true;					//処理を行ったたらすぐにtrueにする(これで1フレームだけ処理を行う)
		if (Gameflag == true)
		{
			break;
		}	
	}
	CountPice();
	Draw_Game();
	Gameflag = false;
	return turn;
}

int Main_Program2(int &turn)	//対人間
{
	while (Gameflag == false)
	{
		Updata_cursor();
		Me_PutPice(board_y, board_x, turn);
		You_PutPice(board_y, board_x, turn);
		turn = CheckPass(turn);				//CheckPassの戻り値をturnに入れる(ターンの切り替え)
		Gameflag = true;					//処理を行ったたらすぐにtrueにする(これで1フレームだけ処理を行う)
		if (Gameflag == true)
		{
			break;
		}
	}
	CountPice();
	Draw_Game();
	Gameflag = false;
	return turn;
}

int Main_Program3(int &turn)		//対CPU2
{
	while (Gameflag == false)
	{
		Updata_cursor();
		Me_PutPice(board_y, board_x, turn);
		AI2_PutPice(board_y, board_x, turn);
		turn = CheckPass(turn);				//CheckPassの戻り値をturnに入れる(ターンの切り替え)
		Gameflag = true;					//処理を行ったたらすぐにtrueにする(これで1フレームだけ処理を行う)
		if (Gameflag == true)
		{
			break;
		}
	}
	CountPice();
	Draw_Game();
	Gameflag = false;
	return turn;
}
//------------------------------------------------------------------------------------------------------------