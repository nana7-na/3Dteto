#pragma once

#include "scene.h"

//scene num = 1

//�X�^�[�g���
class start_scene : public  scene {
public:
	start_scene();//�R���X�g���N�^
	int frame(char *key);//�t���[�����ɌĂяo�����֐�
protected:
private:
	//�ϐ�
	Button b;//�{�^��
	int start_x; int start_y;//�^�C�g���\���̈ʒu
	int hed_size;//�^�C�g���̕����T�C�Y
	int b_size;//�{�^���̖��O�̕����T�C�Y

	void draw_scene();//�V�[���`��
	void input_pros(char* key);//�������L�[�̎擾������
};