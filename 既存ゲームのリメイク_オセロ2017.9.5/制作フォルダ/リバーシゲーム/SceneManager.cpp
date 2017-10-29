#include "DxLib.h"
#include "Input.h"
#include "Game.h"
#include "SceneManager.h"
#include "Title.h"

 enum GameData
{
	End = 2,		  //�I��
};

enum Scene
{
	//�K�v�ɉ����Ēǉ�
	Title,
	Game_cpu,
	Game_cpu2,
	Game_human

};Scene scene;

extern int turn;

//�^�C�g���V�[���̏�����
void Init_Title()
{
	TitleGraph_Init();
}
//�Q�[���{�҂̏�����
void Init_MainGame()
{
	Init_Graph();
	Init_Board();
	Init_Game();
}
//�V�[���Ǘ�
void ChangeScene()
{
	switch (scene)
	{
		//�^�C�g���V�[��----------------------
	case Title:
		Draw_Title();
		if (Key(KEY_INPUT_S) == 1)
		{
			//�Q�[���̏�����
			Init_MainGame();
			//�^�C�g���̃f�[�^���폜
			Title_Fin();
			//�V�[�����Q�[���{��(CPU)��
			scene = Game_cpu;
		}
		if (Key(KEY_INPUT_A) == 1)
		{
			//�Q�[���̏�����
			Init_MainGame();
			//�^�C�g���̃f�[�^���폜
			Title_Fin();
			//�V�[�����Q�[���{��(CPU)��
			scene = Game_cpu2;
		}
		if (Key(KEY_INPUT_W) == 1)
		{
			//�Q�[���̏�����
			Init_MainGame();
			//�V�[�����Q�[���{��(�ΐl)��
			scene = Game_human;
		}
		break;
		//----------------------------------

		//�Q�[���V�[��1----------------------
	case Game_cpu:
		//���t���[������������
		Main_Program(turn);		//��CPU
		//���g���C
		if (turn >= End && Key(KEY_INPUT_R) == 1)
		{
			//�Q�[���̏�����
			Init_MainGame();
		}
		//�V���b�g�_�E��
		if (turn >= End && Key(KEY_INPUT_F) == 1)
		{
			InitGraph();	//���ׂẲ摜�f�[�^���폜	
			DxLib_End();	//DX���C�u�����I������
		}
		//�^�C�g���ɖ߂�
		if (turn >= End && Key(KEY_INPUT_T) == 1)
		{
			//�^�C�g���̓ǂݍ���
			Init_Title();
			//�Q�[���{�҂̃f�[�^���폜
			Game_Fin();
			scene = Title;
		}
		break;
		//----------------------------------

		//�Q�[���V�[��2----------------------
	case Game_cpu2:
		//���t���[������������
		Main_Program3(turn);	//��CPU2
		//���g���C
		if (turn >= End && Key(KEY_INPUT_R) == 1)
		{
			//�Q�[���̏�����
			Init_MainGame();
		}
		//�V���b�g�_�E��
		if (turn >= End && Key(KEY_INPUT_F) == 1)
		{
			InitGraph();	//���ׂẲ摜�f�[�^���폜	
			DxLib_End();	//DX���C�u�����I������
		}
		//�^�C�g���ɖ߂�
		if (turn >= End && Key(KEY_INPUT_T) == 1)
		{
			//�^�C�g���̓ǂݍ���
			Init_Title();
			//�Q�[���{�҂̃f�[�^���폜
			Game_Fin();
			scene = Title;
		}
		break;
		//----------------------------------

		//�Q�[���V�[��3----------------------
	case Game_human:
		Main_Program2(turn);	//�ΐl��
		//���g���C
		if (turn >= End && Key(KEY_INPUT_R) == 1)
		{
			//�Q�[���̏�����
			Init_MainGame();
		}
		//�V���b�g�_�E��
		if (turn >= End && Key(KEY_INPUT_F) == 1)
		{
			InitGraph();		//���ׂẲ摜�f�[�^���폜	
			DxLib_End();		//DX���C�u�����I������
		}
		//�^�C�g���ɖ߂�
		if (turn >= End && Key(KEY_INPUT_T) == 1)
		{
			//�^�C�g���̓ǂݍ���
			Init_Title();
			//�Q�[���{�҂̃f�[�^���폜
			Game_Fin();
			scene = Title;
		}
		break;
		//-----------------------------------
	}
}
