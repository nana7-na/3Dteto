#pragma once

#include "DxLib.h"
#include "draw_element.h"
#include "stage.h"

class Pen {
public:
	Pen();//�R���X�g���N�^

	void draw_button(Button b);//�{�^���̕`��
	void draw_box(VECTOR v, int model);//�u���b�N��`��
	void draw_box(BOX b);//�w�肳�ꂽBOX��`��
	void draw_BoxEffect(stage *s);//���n�n�_��`��
	void draw_axis();//����`��
	void draw_stage(stage s);//���݂̃X�e�[�W����`��

	//�㏈��
	void del_boxs();//�Ō�Ƀ��f��������

	//�Z�b�^�[
	void set_campos(float x, float y, float z);

	//�Q�b�^�[
	int get_w();
	int get_h();
	int get_bit();

private:
	int width;//��ʕ�
	int hight;//��ʍ���
	int bit;//��ʃT�C�Y
	VECTOR camera_pos;//�J�����ʒu
	int boxs[7];//�e�u���b�N�̃��f��
	float h = 150.0f;//points�̕␳
	VECTOR points[4] = { VGet(0.0f,STAGE_Y * BOX_SIZE / 2.0f + h,0.0f),VGet(BOX_SIZE * STAGE_XZ,STAGE_Y * BOX_SIZE / 2 + h,0.0f),VGet(0.0f,STAGE_Y * BOX_SIZE / 2 + h,BOX_SIZE * STAGE_XZ),VGet(BOX_SIZE * STAGE_XZ, STAGE_Y * BOX_SIZE / 2 + h,BOX_SIZE * STAGE_XZ) };//�J�����̒��S�ʒu
	int cam_num;//�ǂ�points�̒l���g����

	//�Z�b�e�B���O
	void box_set();//���f�����Z�b�g����

	//cam_num�̐ݒ������
	int c_pos_num();
};