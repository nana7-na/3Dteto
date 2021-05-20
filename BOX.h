#pragma once
#include "Location.h"

using namespace std;

//�e�g���X�̃u���b�N��~���l�߂镔���̑傫��
#define STAGE_XZ 5
#define STAGE_Y 15

//���삵�Ă���u���b�N
class BOX {
public:
	Location b1, b2, b3, b4;//�e�u���b�N�̈ʒu
	Location bc;//���S�ʒu�̕ێ�
	Location hi;//�e���̈�ԍ����l�����ꂼ��ێ�
	Location lo;//�Ⴂ�l

	double shape;//�`

	//�R���X�g���N�^
	BOX();
	BOX(int x, int y, int z, double shape_num);
	BOX(float x, float y, float z, double shape_num);
	BOX(Location l1, Location l2, Location l3, Location l4, double shape);

	//�ړ�
	void move(int x, int y, int z);
	void move(float x, float y, float z);

	//��]
	void rotate(float xz, float yx, float zy);
	void rotate_sub();

private:
	//�`���߂�
	void set_shape(double shape_num);

	//��ԍ����ʒu�ƒႢ�ʒu�̔���ݒ肷��
	void hiilo();
};
