#pragma once
//�Q�[�����
struct Obj
{
	int Handle,		//�摜�f�[�^�i�[�p
		x,			//�J�[�\���p
		y,
		num;		//�R�}�𐔂��i�[
};
//�摜�̃��[�h
void Init_Graph();
//�Q�[�����̏�����
void Init_Game();
//�Ֆʂ̏�����
void Init_Board();
//�J�[�\���̈ړ�
void Updata_cursor();
//vec�Ŏw�肳�ꂽ�����ɂ��ĂЂ�����Ԃ�R�}�����邩�m�F����(8�����̃`�F�b�N)
int CheckFlip(int y, int x, int turn, int vec);
//���̏ꏊ�ɒu�����Ƃ��ł��邩���m�F
int PutCheck(int y, int x, int turn);
//���ۂɗ��Ԃ�����
void Flip(int y, int x, int turn, int vec);
//���͂��󂯂ė��Ԃ��邩�m���߂�֐�
int Put(int y, int x, int turn);
//�p�X�ƏI������
int CheckPass(int turn);
//�R�}��u������(��CPU�p)
void Me_PutPice(int by, int bx, int &turn);
//�R�}��u������(2p�ΐ�p)
void You_PutPice(int by, int bx, int &turn);
//CPU�u���鏊�ɓK���ɒu��
void AI_PutPice(int by, int bx, int &turn);
//CPU�D�揇�ʂ̍����Ƃ���ɒu��
void AI2_PutPice(int by, int bx, int &turn);
//�R�}�𐔂���
void CountPice();
//1�t���[����������������
int Main_Program(int &turn);	//��CPU
int Main_Program2(int &turn);	//�ΐl��
int Main_Program3(int &turn);	//��CPU2
//�Ֆʂ̕\��
void Draw_Game();
//�I������
void Game_Fin();

