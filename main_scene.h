#pragma once
#include "scene.h"

//scene_num = 2

class main_scene : public scene{
public:
	//�R���X�g���N�^
	main_scene();
	main_scene(stage *s);

	int frame(char* key);//�t���[�����ɌĂяo�����֐�

protected:

private:
	//�ϐ�
	stage *s;//�X�e�[�W���
	int drop_st;//�X�e�[�W��ԊǗ�

	void draw_scene();//�V�[���̕`��

	void input_pros(char* key);//���͏���

};
