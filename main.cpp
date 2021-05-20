#include <stdio.h>
#include <iostream>

#include "start_scene.h"
#include "score_scene.h"
#include "main_scene.h"

using namespace std;

//DX���C�u�����̃��C���֐�
int WINAPI WinMain(HINSTANCE hInstace, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow) {
	//DX���C�u�����̐ݒ�
	SetOutApplicationLogValidFlag(FALSE); //Log.txt�𐶐����Ȃ��悤�ɂ���
	ChangeWindowMode(TRUE);//�S��ʂɂ��Ȃ�
	SetGraphMode(640, 700, 32);//��ʃT�C�Y�ݒ�
	SetUseLighting(0);//���C�g������
	if (DxLib_Init() == -1) {//���s�����ꍇ�̃G���[����
		return -1;
	}

	//�ϐ��̏�����
	char key[256];//���̓Q�b�g
	scene* now = new start_scene();;//���݂̃V�[��
	stage s = stage();//��ԊǗ��ϐ�
	int state = 0;//�V�[���̏�ԊǗ�

	for (int i = 0; i < 256; i++)key[i] = 0;
	
	//�J�����ݒ�(�\)
	SetCameraNearFar(0.1f, 10000.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(700.0f,1200.0f,600.0f), VGet(STAGE_XZ/2, 1000, STAGE_XZ/2));

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);
	//�J�����ݒ�(��)
	SetCameraNearFar(0.1f, 10000.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(700.0f, 1200.0f, 600.0f), VGet(STAGE_XZ / 2, 1000, STAGE_XZ / 2));
	
	//��ʃA�b�v�f�[�g
	while (ProcessMessage() == 0) {
		ClearDrawScreen(); //��ʏ���
		GetHitKeyStateAll(key);//���͎擾
		int i = now->frame(key);//�t���[�����̏���

		if (i != state) {//��ʑJ��
			state = i;
			switch (state) {
				case 0://���s
					state = i;
					break;
				case 1://�X�^�[�g��ʂ�
					now = new start_scene();
					break;
				case 2://���C����ʂ�
					now = new main_scene(&s);
					break;
				case 3://�X�R�A��ʂ�
					now = new score_scene(s.get_score());
					break;
				case -1:
					break;
			}
		}

		if(state == -1 || ProcessMessage() == -1)break;//�Q�[���I�[�o�[or�G���[�̎��ɏI��

		//����
		DxLib::ScreenFlip();
	}

	delete now;
	DxLib::DxLib_End();//DX���C�u�����I��

	return 0;
}