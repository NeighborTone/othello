//--------------------��ƊJ�n��--------------------
//--------------------2017�N8��2��------------------
//--------------------��ʂ�̍�ƏI��---------------
//--------------------2017�N8��2��------------------
//--------------------AI2��ǉ�---------------------
//--------------------2017�N8��4��------------------
//--------------------�p�X������C��-----------------
//--------------------2017�N8��16��-----------------
//--------------------�p�X�̔�����C��---------------
//--------------------2017�N9��05��-----------------

#include "DxLib.h"
#include "SceneManager.h"
#include "Input.h"

static const int 
SCREEN_WIDIH = 840,
SCREEN_HEIGHT = 640;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	//��ʃT�C�Y��840*640�ɕύX
	SetGraphMode(SCREEN_WIDIH, SCREEN_HEIGHT, 32);
	//�^�C�g���̓ǂݍ���
	Init_Title();

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		Updata_Key();			//�L�[���͂�L���ɂ���
		ChangeScene();			//�V�[���̐؂�ւ�
	}
	DxLib_End();			// DX���C�u�����I������
	return 0;
}
