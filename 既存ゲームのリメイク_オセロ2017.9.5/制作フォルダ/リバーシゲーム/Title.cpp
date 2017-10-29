#include "DxLib.h"
#include "SceneManager.h"
#include "Title.h"

static const int 
	SCREEN_WIDIH = 840,
	SCREEN_HEIGHT = 640,
	CELLSIZE = 80;	  //�}�X����̑傫��
int TitleGraph;

//�^�C�g���V�[���Ɏg���摜�̏�����
void TitleGraph_Init()
{
	TitleGraph = LoadGraph("Graph/�{�[�h.bmp");
}
void Draw_Title()
{
	DrawFormatString(670, 300, GetColor(255, 255, 255), "S�ő�CPU��(�ア)");
	DrawFormatString(670, 320, GetColor(255, 255, 255), "A�ő�CPU��2(���ア)");
	DrawFormatString(670, 340, GetColor(255, 255, 255), "W�őΐl��");
	DrawGraph(0, 0, TitleGraph, false);
	//�O���b�h��������
	for (int x = 0; x < 9; ++x)
	{
		//�c��
		DrawLineAA((float)CELLSIZE * x, 0.f, (float)CELLSIZE * x, (float)SCREEN_HEIGHT, GetColor(0, 0, 0), 1.4f);
	}
	for (int y = 0; y < 9; ++y)
	{
		//����
		DrawLineAA(0.f, (float)CELLSIZE * y, (float)SCREEN_HEIGHT, (float)CELLSIZE * y, GetColor(0, 0, 0), 1.4f);
	}
}
//�^�C�g���V�[���̃f�[�^���폜
void Title_Fin()
{
	DeleteGraph(TitleGraph);	//�^�C�g���摜���폜
}