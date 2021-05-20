#pragma once
#include<vector>
#include <time.h>

#include "BOX.h"

struct box_state {
	bool in;
	int color;
	box_state();
	box_state(bool b, int i);
};

//���̌`���΂�Ȃ��o������悤�ɂ���
typedef struct box_line {
public:
	//�ǂ̌`���o�Ă��邩�̏���
	vector<int> line;
	vector<int>::iterator at;//���̃C�e���[�^�[
	
	box_line();//�R���X�g���N�^

	int next_num();//���̌`��Ԃ�

private:
	void start();//�������֐�
} box_line;

class stage {
public:
	struct box_state now_boxs[STAGE_XZ][STAGE_Y][STAGE_XZ];//���݂̃u���b�N�� x,y,z
	BOX dropping; //���쒆�̃u���b�N

	stage();//�R���X�g���N�^
	//�������֐�
	void initialize();

	//�t���[�����̏���
	int frame();

	//depth�̐[���ɔ����l�܂��Ă�����A���������B
	void delete_plane(int depth);

	//�����ɉ��ɗ��Ƃ�
	int shortcut();

	//�����ɓ�����܂ł̋���
	float collide_next();

	//�����̌v�Z
	float dist(float x1, float y1, float z1, float x2, float y2, float z2);

	//�ړ�
	bool move(float x, float y, float z);
	bool move(int x, int y, int z);

	//��](BOX)
	void rotate(float xz, float yx, float zy);

	//���_�ړ�
	void rotate_eye(float xz);

	//�x�N�g���̕ϊ��i���W�ϊ��͂ł��Ȃ��j
	void convert_vec(float* p);
	void convert_vec(int* p);

	//�Q�[���I�[�o�[
	bool gameover();

	//�Q�b�^�[
	void get_camerapos(float* pos);
	int get_cnum();
	int get_score();
	int get_dropst();

private:
	struct box_line next_box;//���̃u���b�N�̌`��ۑ�
	int drop_st;//���݂̑���u���b�N�̏�� 0:�u���b�N���� 1:�u���b�N���쒆 2:�ꎞ�I�ɑ���ł��鎞��
	time_t prev_time;//�o�ߎ��Ԍv���p(�O��̎��Ԃ�����)
	float cool_time;//�N�[���^�C��
	int debug = 0;//�f�o�b�O���[�h
	int score;//�X�R�A
	float camera_pos[3];//���݂̉�]�ʒu��\��
	float h = 150.0f;//points�̕␳
	float points[4][3] = { {BOX_SIZE * STAGE_XZ, STAGE_Y * BOX_SIZE / 2 + h,BOX_SIZE * STAGE_XZ } ,{BOX_SIZE * STAGE_XZ,STAGE_Y * BOX_SIZE / 2 + h,0.0f},{0.0f,STAGE_Y * BOX_SIZE / 2 + h,BOX_SIZE * STAGE_XZ},{0.0f,STAGE_Y * BOX_SIZE / 2.0f + h,0.0f} };//�J�����̒��S�ʒu
	int cam_num;//���݂̃J�����ɑ΂��Ăǂ�points�̒l���g����

	//�����m�̏Փ˔���
	bool collide();
	int collide(Location l);

	//�u���b�N���Œ�
	void set_box();

	//���܂肫���Ă�����T��
	int check_stage();

	//��]�̕⏕�֐�
	bool rotate_sub();

	//BOX�̕`�揇�𐮂���
	void sort_box();
	void my_sort(Location* a, int p, int r);
	int partition(Location* a, int p, int r);

	//�X�R�A�Z�o
	void calc_score(int n);

	//cam_num�̐ݒ�
	int c_pos_num();
};
