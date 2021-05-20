#pragma once

#include "Pen.h"

class scene{
public:
	scene() {//�R���X�g���N�^
		scene_st = 0;
		for (int i = 0; i < 256; i++) {
			keys[i] = 0; prev_key[i] = 0;
		}
		p = Pen();
	}

	virtual int frame(char* key) = 0;//�t���[�����ɌĂяo�����֐�

	virtual void draw_scene() = 0;//�V�[���̕`��

protected:
	int scene_st;//�V�[���̏�ԊǗ��ϐ�
	char keys[256];//���݂̓���
	char prev_key[256];//�O��̓���
	Pen p;//�`��֐�

	//���͏�Ԃ̎擾
	void copy_key(char* key) {
		for (int i = 0; i < 256; i++) {
			//�O��̏�Ԃ��o����
			if (keys[i]) {
				prev_key[i] = 1;
			}
			else if (!keys[i]) {
				prev_key[i] = 0;
			}
			//����̓��͂�����
			if (key[i]) {
				keys[i] = 1;
			}
			else if (!key[i]) {
				keys[i] = 0;
			}
		}
	}

private:
	virtual void input_pros(char* key) = 0;//�������L�[�̎擾������
};