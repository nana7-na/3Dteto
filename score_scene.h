#pragma once

#include "scene.h"

//scene_num = 3

//�X�R�A�\�����
class score_scene : public scene{
public:
	score_scene(int s);//�R���X�g���N�^
	int frame(char *key);//�t���[�����ɌĂяo�����
private:
	//�ϐ�
	int score;//�X�R�A
	char score_c[10];//�X�R�A������
	int b_index;//�I�𒆂̃{�^��
	int w;//�X�R�A�̉���
	Button b;//�{�^��1

	void draw_scene();//��ʕ`��
	void input_pros(char* key);//�������L�[�̎擾������
};