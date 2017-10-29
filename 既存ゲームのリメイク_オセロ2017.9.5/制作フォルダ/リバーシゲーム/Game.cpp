#include "DxLib.h"
#include "Input.h"
#include "Game.h"
#include "SceneManager.h"

//�m��I�Ȓl
static const int 
SCREEN_WIDIH = 840,				 
SCREEN_HEIGHT = 640,	
//8�����̔���p
vec_y[] = { -1,-1,0,1,1,1,0,-1 },
vec_x[] = { 0,1,1,1,0,-1,-1,-1 };

//�r���ŕύX����\���̂���l
enum GameData
{
	Me = 0,			  //�����̃^�[��
	You = 1,		  //����̃^�[��
	End = 2,		  //�I��
	EMPTY = 0,		  //�󂫃}�X
	BLACK = 1,		  //���R�}
	WHITE = -1,		  //���R�}
	BOARDSIZE = 8,	  //�{�[�h�̑傫��,8 * 8
	CELLSIZE = 80,	  //�}�X����̑傫��
	CURSOR_MAX = 480  //�J�[�\���̈ړ��ł���ő�l
};


Obj BoardGraph;
Obj Black;
Obj White;
Obj cursor;
int board_x;		//�����Ƀ{�[�h�̑傫�����i�[���邽�߂ɗp��
int board_y;
int turn;
int turncount;		//�o�߃^�[�����i�[
bool Gameflag;		//�X�V��������1�t���[�������������s���ꍇ�Ɏg��
int Board[BOARDSIZE][BOARDSIZE];
//�摜�̏�����
void Init_Graph()
{
	BoardGraph.Handle = LoadGraph("Graph/�{�[�h.bmp");
	cursor.Handle = LoadGraph("Graph/�J�[�\��.png");
	Black.Handle = LoadGraph("Graph/��.png");
	White.Handle = LoadGraph("Graph/��.png");
}
//�Q�[�����̏�����
void Init_Game()
{
	turn = Me;	//�^�[���AMe�͍������
	Gameflag = false;
	board_x = BOARDSIZE,
	board_y = BOARDSIZE;
	Black.num = 0;
	White.num = 0;
	cursor.x = 0;
	cursor.y = 0;
	turncount = 0;
}
//�{�[�h�̏�����
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
	//�f�o�b�O�p
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
//�J�[�\���̈ړ�
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
//vec�Ŏw�肳�ꂽ�����ɂ��ĂЂ�����Ԃ�R�}�����邩�m�F����(8�����̃`�F�b�N)
int CheckFlip(int y, int x, int turn, int vec)	//�m�F�o������TRUE��Ԃ�(�Ђ�����Ԃ���R�}������)
{
	int flag = FALSE;
	while (TRUE)
	{
		//8�����̑���
		y += vec_y[vec];
		x += vec_x[vec];

		//�Ֆʂ̊O�ɏo�Ă�����I��
		if (x < 0 || y < 0 || x > BOARDSIZE - 1 || y > BOARDSIZE - 1)
		{
			return FALSE;
		}
		//�󂫃}�X��������I��
		if (Board[y][x] == EMPTY)
		{
			return FALSE;
		}
		//����̃R�}����������t���O�𗧂Ă�
		if (Board[y][x] == (turn ? BLACK : WHITE))
		{
			flag = TRUE;
		}
		//�����̃R�}���������ꍇ�Ƀt���O�������Ă���Βu����
		if (Board[y][x] == (turn ? WHITE : BLACK))
		{
			//�u����
			if (flag == TRUE)
			{
				return TRUE;
			}
			//�u���Ȃ�
			else
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
//���̏ꏊ�ɒu�����Ƃ��ł��邩���m�F
int PutCheck(int y, int x, int turn)	//�u�����TRUE��Ԃ�
{
	int vec;
	//�ǂꂩ1�����ł��Ђ�����Ԃ��邩�m�F
	for (vec = 0; vec < BOARDSIZE; ++vec)
	{
		if (CheckFlip(y, x, turn, vec) == TRUE)
		{
			return TRUE;	//�Ђ�����Ԃ���
		}
	}
	return FALSE;
}
//���ۂɗ��Ԃ�����
void Flip(int y, int x, int turn, int vec)
{
	while (TRUE)
	{
		y += vec_y[vec];
		x += vec_x[vec];

		//�����̃R�}����������I��	
		if (Board[y][x] == (turn ? WHITE : BLACK))
		{
			break;
		}
		//����ȊO�Ȃ玩���̃R�}�œh��Ԃ�
		Board[y][x] = (turn ? WHITE : BLACK);
	}
}
//���͂��󂯂ė��Ԃ��邩�m���߂�֐�
int Put(int y, int x, int turn)
{
	int vec,
		flag = FALSE;
	//�󂫃}�X�łȂ���ΏI��
	if (Board[y][x] != EMPTY)
	{
		return FALSE;
	}
	//�S�����ɂ��Ċm�F
	for (vec = 0; vec < BOARDSIZE; ++vec)
	{
		if (CheckFlip(y, x, turn, vec) == TRUE)
		{
			//���Ԃ�
			Flip(y, x, turn, vec);
			flag = TRUE;
		}
	}
	if (flag == TRUE)
	{
		//���̏ꏊ�ɃR�}��u��
		Board[y][x] = (turn ? WHITE : BLACK);
		return TRUE;
	}
	return FALSE;
}
//�p�X�ƏI������(�^�[���`�F���W)
int CheckPass(int turn)
{
	int y,
		x;
	//������ꏊ�����邩�m�F
	for (y = 0; y < BOARDSIZE; ++y)
	{
		for (x = 0; x < BOARDSIZE; ++x)
		{
			//����Ε��ʂɑ��s
			//�����̎��
			if (Board[y][x] == EMPTY && PutCheck(y, x, turn) == TRUE)
			{
				return (turn == Me ? Me : You);			//���݂ɒu��
			}
		}
	}
	//�ꏊ���Ȃ������̂Ō�ւ��ĒT��
	//����̎��
	if (PutCheck(y, x, turn) == FALSE && turn == You)	//(�������u���Ȃ��B������̎�Ԃł���)
	{
		return Me;					//�p�X
	}
	if (PutCheck(y, x, turn) == TRUE && turn == You)    //���肪�u���Ȃ��B���������u����Ƃ�
	{
		return Me;
	}
	//�Ȃ������̂ŃQ�[���I��
	return End;
}
//�R�}��u������
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
//CPU1,�u���鏊�ɓK���ɒu��
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
//CPU2,�^�[���o�߂Œu���ꏊ�𑝂₷
void AI2_PutPice(int by, int bx, int &turn)
{
	//�Ֆʂ̕]���l
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
				//���Ղ͕]���l��5�ȉ��̂Ƃ��납��u��
				if (priority[y][x] <= 5)
				{
					if (Put(y, x, turn) == TRUE)
					{
						Board[by][bx] = WHITE;
						++turncount;
						turn = Me;
					}
				}
				//�o�߃^�[������3�ȏ�ɂȂ�����]���l15�ȉ��̂Ƃ���ɂ��u��
				else if(turncount >= 3 && priority[y][x] <= 15)
				{
					if (Put(y, x, turn) == TRUE)
					{
						Board[by][bx] = WHITE;
						++turncount;
						turn = Me;
					}
				}
				//�o�߃^�[������6�ȏ�ɂȂ�����]���l40�ȉ��̂Ƃ���ɂ��u��
				else if (turncount >= 6 && priority[y][x] <= 40)
				{
					if (Put(y, x, turn) == TRUE)
					{
						Board[by][bx] = WHITE;
						++turncount;
						turn = Me;
					}
				}
				//�o�߃^�[������8�ȏ�ɂȂ�����]���l120�ȉ��̂Ƃ���ɂ��u��
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
//�ΐl
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
//�R�}�𐔂���
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
//�Q�[�����̕\��
void Draw_Game()
{
	//�Ֆ�
	DrawGraph(0, 0, BoardGraph.Handle, false);
	//����
	if (turn == Me || turn == You)
	{
		DrawFormatString(680, 300, GetColor(255, 255, 255), "�\���L�[�ňړ�");
		DrawFormatString(680, 320, GetColor(255, 255, 255), "Z�Œu���܂�");
	}
	if (turn == Me)
	{
		DrawFormatString(680, 50, GetColor(255, 255, 255), "��(����)�̃^�[��");
	}
	if (turn == You)
	{
		DrawFormatString(680, 50, GetColor(255, 255, 255), "��(����)�̃^�[��");
	}
	if (turn == End)
	{
		DrawFormatString(680, 300, GetColor(255, 255, 255), "R�Ń��g���C");
		DrawFormatString(680, 320, GetColor(255, 255, 255), "T�Ń^�C�g��");
		DrawFormatString(680, 340, GetColor(255, 255, 255), "F�ŃV���b�g�_�E��");

		if (Black.num > White.num)
		{
			DrawFormatString(680, 100, GetColor(255, 255, 255), "���̏����ł�");
		}
		else if (Black.num < White.num)
		{
			DrawFormatString(680, 100, GetColor(255, 255, 255), "���̏����ł�");
		}
		else
		{
			DrawFormatString(680, 100, GetColor(255, 255, 255), "���������ł�");
		}
	}
	//�R�}
	for (int y = 0; y < BOARDSIZE; ++y)
	{
		for (int x = 0; x < BOARDSIZE; ++x)
		{
			//�{�[�h�̏��WHITE�Ȃ甒�R�}���u�����
			if (Board[y][x] == WHITE)
			{
				DrawGraph(CELLSIZE * x, CELLSIZE * y, White.Handle, true);
			}
			//�{�[�h�̏��BLACK�Ȃ獕�R�}���u�����
			if (Board[y][x] == BLACK)
			{
				DrawGraph(CELLSIZE * x, CELLSIZE * y, Black.Handle, true);
			}
		}
	}
	//�O���b�h��������
	for (int x = 0; x < 9; ++x)
	{
		//�c��
		DrawLineAA((float)CELLSIZE * x, 0.f, (float)CELLSIZE * x, (float)SCREEN_HEIGHT, GetColor(0, 0, 0), 1.4f);
	}
	for (int y = 0; y < 9; ++y)
	{
		//����
		DrawLineAA(0.f, (float)CELLSIZE * y, (float)SCREEN_HEIGHT, (float)CELLSIZE * y, GetColor(0 ,0 ,0), 1.4f);
	}
	//�J�[�\���̕\��
	DrawGraph(cursor.x, cursor.y, cursor.Handle, true);
}
//�I������
void Game_Fin()
{
	//�摜�Ȃǂ̃f�[�^���폜(�^�C�g���ɖ߂�Ƃ��ɌĂяo��)
	DeleteGraph(Black.Handle);
	DeleteGraph(White.Handle);
	DeleteGraph(cursor.Handle);
	DeleteGraph(BoardGraph.Handle);
}
//�ȉ��͏�L�̏������܂Ƃ߂�����----------------------------------------------------------------------------------
//���t���[�����Ƃɏ������s��
int Main_Program(int &turn)		//��CPU
{
	while(Gameflag == false)
	{
		Updata_cursor();
		Me_PutPice(board_y, board_x, turn);
		AI_PutPice(board_y, board_x, turn);
		turn = CheckPass(turn);				//CheckPass�̖߂�l��turn�ɓ����(�^�[���̐؂�ւ�)
		Gameflag = true;					//�������s�������炷����true�ɂ���(�����1�t���[�������������s��)
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

int Main_Program2(int &turn)	//�ΐl��
{
	while (Gameflag == false)
	{
		Updata_cursor();
		Me_PutPice(board_y, board_x, turn);
		You_PutPice(board_y, board_x, turn);
		turn = CheckPass(turn);				//CheckPass�̖߂�l��turn�ɓ����(�^�[���̐؂�ւ�)
		Gameflag = true;					//�������s�������炷����true�ɂ���(�����1�t���[�������������s��)
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

int Main_Program3(int &turn)		//��CPU2
{
	while (Gameflag == false)
	{
		Updata_cursor();
		Me_PutPice(board_y, board_x, turn);
		AI2_PutPice(board_y, board_x, turn);
		turn = CheckPass(turn);				//CheckPass�̖߂�l��turn�ɓ����(�^�[���̐؂�ւ�)
		Gameflag = true;					//�������s�������炷����true�ɂ���(�����1�t���[�������������s��)
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