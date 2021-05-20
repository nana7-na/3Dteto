#include <fstream>
#include <random>]
#include <iostream>

#include "stage.h"

using namespace std;

//���̌`�̐�
#define SHAPES 7

//struct box_line �{��
box_line::box_line() {
	start();
}
//���̐��Ԃ�
int box_line::next_num() {
	if (at == line.end()) {//
		start();
	}
	int next = *at;
	at++;
	return next;
}
//������
void box_line::start() {
	line = { 0, 1, 2, 3, 4, 5, 6 };
	random_device seed;
	mt19937 engine(seed());
	shuffle(line.begin(), line.end(), engine);
	at = line.begin();
}

//struct box_state
box_state::box_state():in(false),color(-1){}
box_state::box_state(bool b,int i) : in(b), color(i) {}


//class stage�{��

//�R���X�g���N�^
stage::stage() {
	initialize();
}

//������
void stage::initialize() {
	//�z��̏�����
	for (int xx = 0; xx < STAGE_XZ; xx++) {//�X�e�[�W���
		for (int yy = 0; yy < STAGE_Y; yy++) {
			for (int zz = 0; zz < STAGE_XZ; zz++) {
				now_boxs[xx][yy][zz].in = false;
			}
		}
	}
	//�ϐ��̏�����
	next_box = box_line();
	drop_st = 0;   
	cool_time = 0.0f;
	camera_pos[0] = 700.0f; camera_pos[1] = 1200.0f; camera_pos[2] = 600.0f;//�J���������ʒu
	c_pos_num();
	time(&prev_time);//���Ԃ��Z�b�g
	score = 0.0f;
}

//�u���b�N�̐����ƈړ�(�t���[�����̏���)
int stage::frame() {
	time_t now_time;//���ݎ���
	time(&now_time);
	float delta_time = float(now_time - prev_time);//�O�t���[������̌o�ߎ���

	if (drop_st == 0) {//���쒆�̃u���b�N���Ȃ����
		if (debug == 0) {
			dropping = BOX(0, STAGE_Y - 1, 0, next_box.next_num());//�u���b�N����
			drop_st = 1;//��ԑJ��
		}
		else if (debug == 1) {//�f�o�b�O
			dropping = BOX(0, 1, 0, next_box.next_num());//�u���b�N����
			drop_st = 1;
		}
	}else if(drop_st == 1) {//�u���b�N���쒆�̏��
		if (debug == 0) {
			bool a = move(0.0f, -1.0f, 0.0f);//1.0�����Ƃ��Ă���
			if (dropping.lo.get_yf() < BOX_SIZE/2 || !a) {//�����ɓ���������
				drop_st = 2;//��ԑJ��
				cool_time = 1.0f;//��������Ԃ̎���
			}
		}
		else if (debug == 1) {
		}
	}
	else if (drop_st == 2) {//����������
		if ((cool_time -= delta_time) <= 0) {//�������^�C���I��
			set_box();//�ʒu�̊m��
			drop_st = 0;
			cool_time = 0.0f;
		}
		if (move(0, -1, 0)) {//���ɓ�����悤�ɂȂ����痎�Ƃ��Ă���(1�̏�Ԃɖ߂�)
			move(0, 1, 0);
			drop_st = 1;
			cool_time = 0.0f;
		}
	}
	else {
		//error
	}

	//�Q�[���I�[�o�[
	if (gameover()) {
		drop_st = -1;
		return drop_st;
	}

	time(&prev_time);//���݂̎��Ԃ��o����
	return drop_st;
}

//�����ł���񂪂��邩�T��
int stage::check_stage() {
	int result = drop_st;//drop_st�̒l��Ԃ�
	vector<int> del;
	del.clear();
	//��������T��
	for (int y = 0; y < STAGE_Y; y++) {
		bool check = true;//�Ō��true�������ꍇ����
		for (int x = 0; x < STAGE_XZ; x++) {
			for (int z = 0; z < STAGE_XZ; z++) {
				if (now_boxs[x][y][z].in) {
					check = true;
				}else {
					check = false;
					break;
				}
			}
			if (!check)break;
		}
		if (check) {
        	del.push_back(y);
			result = 3;
		}
	}
	//����
	for (int i = del.size()-1;i >= 0; i--) {
		delete_plane(del[i]);
	}
	calc_score(del.size());//�X�R�A�Z�o
	if (!gameover())drop_st = -1;
	return result;
}

//�Փ˔���
bool stage::collide() {//�ړ������ɍs�����ꍇ�Փ˂��邩�̔���(BOX�p)
	//�X�e�[�W�̕ǂƏՓ�
	if (collide(dropping.lo) == 1 || collide(dropping.hi) == 1) {
		return true;
	}
	//���ɂ���u���b�N�Ƃ̏Փ�
	Location* ls[4] = {&dropping.b1,&dropping.b2,&dropping.b3,&dropping.b4};
	for (int i = 0; i < 4; i++) {
		if (collide(*ls[i]) == 2) {
			return true;
		}
	}
	return false;
}
 int stage::collide(Location l) {//Location�ɑ΂��ďՓ˔���
	int result = 0;//�Փ˂��Ă��Ȃ�->0 �ǂɏՓ�or�ђ�->1 ���̏�Ƀ{�b�N�X->2
	if (l.get_xi() < 0 || l.get_xi() >= STAGE_XZ || l.get_yi() > STAGE_Y || l.get_yi() < 0 || l.get_zi() < 0 || l.get_zi() >= STAGE_XZ) {
		result = 1;
	}else if (now_boxs[l.get_xi()][l.get_yi()][l.get_zi()].in || (l.get_yf() < BOX_SIZE / 2 + l.get_yi() * BOX_SIZE && now_boxs[l.get_xi()][l.get_yi() - 1][l.get_zi()].in)) {//���Ƀu���b�N������
		result = 2;
	}
	return result;
}

//�w��񂪑S�Ė��܂��Ă����ꍇ����
void stage::delete_plane(int depth) {
	for (int x = 0; x < STAGE_XZ; x++) {
		for (int z = 0; z < STAGE_XZ; z++) {
			now_boxs[x][depth][z].in = false;
			now_boxs[x][depth][z].color = -1;
			for (int i = depth+1; i < STAGE_Y; i++) {
				if (now_boxs[x][i][z].in) {
					now_boxs[x][i- 1][z].in = true;
					now_boxs[x][i- 1][z].color = now_boxs[x][i][z].color;
					now_boxs[x][i][z].in = false;
					now_boxs[x][i][z].color = -1;
				}
			}
		}
	}
}

//�������Ƃ�
int stage::shortcut() {
	if (!move(0.0f, -18.0f, 0.0f)) {
		set_box();
		drop_st = 0;
	}
	return drop_st;
}

//�ړ�(��ɍ��E)
bool stage::move(float x, float y, float z) {
	int vec[3] = { 1,1,1 };
	dropping.move(x, y, z);
	if (collide()) {
		dropping.move(x*-1, y*-1, z*-1);
		return false;
	}
	return true;
}
bool stage::move(int x, int y, int z) {
	dropping.move(x, y, z);
	if (collide()) {
		dropping.move(x * -1, y * -1, z * -1);
		return false;
	}
	return true;
}

//�u���b�N�̈ʒu�Œ�
void stage::set_box() {
	Location ls[4] = { dropping.b1.copy(),dropping.b2.copy(),dropping.b3.copy(),dropping.b4.copy() };
	bool check = true;
	for (int i = 0; i < 4; i++) {
		if (ls[i].get_yi() >= STAGE_Y) {//�X�e�[�W�O�������ꍇ�̏���
			drop_st = -1;
			check = false;
			break;
		}
		now_boxs[ls[i].get_xi()][ls[i].get_yi()][ls[i].get_zi()] = box_state(true, int(dropping.shape));
	}
	if(check)drop_st = check_stage();
}

//�Q�[���I�[�o�[
bool stage::gameover() {
	for (int x = 0; x < STAGE_XZ; x++) {
		for (int z = 0; z < STAGE_XZ; z++) {
			if (now_boxs[x][14][z].in)return true;
		}
	}
	return false;
}

//��]
void stage::rotate(float xz, float yx, float zy) {
	BOX copy_b = BOX(dropping.b1, dropping.b2, dropping.b3, dropping.b4, dropping.shape);
	dropping.rotate(xz, yx, zy);//��]
	bool r = rotate_sub();//��]��ɂԂ��邩�Ȃǂ̏���
	if (!r)dropping = copy_b;//��肭�s���Ȃ������ꍇ���ɖ߂�
	sort_box();//�`�揇�ɕ��ёւ�
}
bool stage::rotate_sub() {
	Location copy[4] = { dropping.b1.copy(),dropping.b2.copy(),dropping.b3.copy(),dropping.b4.copy() };
	float points[3] = { 0,0,0 };//�␳�l
	bool rotate_f = false;//��]��߂���
	bool loop = true;//�G���[���o���ꍇ�p
	int r = collide();//�Փ˔���
	bool vec[6] = { 0,0,0,0,0,0 };//���������

	if (!r)return true;//�������Ȃ���ΏI��
	//�܂��ǂɓ������Ă�����߂�
	if (dropping.hi.get_xf() > BOX_SIZE * STAGE_XZ - BOX_SIZE / 2) {
		points[0] = (BOX_SIZE * STAGE_XZ - BOX_SIZE / 2) - dropping.hi.get_xf();
	}
	else if (dropping.lo.get_xf() < BOX_SIZE / 2) {
		points[0] = BOX_SIZE / 2 - dropping.lo.get_xf();
	}
	if (dropping.lo.get_yf() < BOX_SIZE / 2) {
		points[1] = BOX_SIZE / 2 - dropping.lo.get_yf();
	}
	if (dropping.hi.get_zf() > BOX_SIZE * STAGE_XZ - BOX_SIZE / 2) {
		points[2] = (BOX_SIZE * STAGE_XZ - BOX_SIZE / 2) - dropping.hi.get_zf();
	}
	else if (dropping.lo.get_zf() < BOX_SIZE / 2) {
		points[2] = BOX_SIZE / 2 - dropping.lo.get_zf();
	}
	move(points[0], points[1], points[2]);

	r = collide();
	if (!r)return true;

	//���ɔz�u����Ă���u���b�N�ɏՓ˂����ꍇ
	//���ꂼ��̕�����1���܂łȂ瓮����ok
	bool check = false;
	int moving[3] = { 0,0,0 };//��������
	for (int i = 0; i < 3; i++) {
		moving[i] = 1;
		for (int j = 0; j < 2; j++) {
			if (j == 0)moving[i] = -1;
			else if (j == 1)moving[i] = 1;
			if (move(moving[0], moving[1], moving[2])) {
				check = true;
				break;
			}
		}
		break;
		moving[i] = 0;
	}
	if (collide())return false;
	return true;
}

//��ǂꂾ���������瓖���邩
float stage::collide_next() {
	float result = dropping.lo.get_yf();//���݂̈�ԒႢy���W
	float comp_r = dropping.lo.get_yf() - dropping.lo.get_yi() * BOX_SIZE;//���݂̒��S�Ƃ̂���
	Location* ls[4] = { &dropping.b1,&dropping.b2,&dropping.b3,&dropping.b4 };//�R�s�[

	for (int i = 0; i < 4; i++) {
		int dist = 0;
		for (int y = ls[i]->get_yi()-1; y >= 0; y--) {//-1�����Ƃ��Ă����A���ɉ����Ɠ����鋗��������
			if (now_boxs[ls[i]->get_xi()][y][ls[i]->get_zi()].in) {
				break;
			}
			dist++;
		}
		if (comp_r + dist * BOX_SIZE < result) {//���܂łň�ԒZ�������Ȃ�result�ɓ����
			result = comp_r + dist * BOX_SIZE;
		}
	}
	result -= BOX_SIZE / 2;//�␳
	if (result < 0) {//�}�C�i�X�ɂȂ��Ă��܂����ꍇ�̏���
		result = BOX_SIZE/2 - dropping.lo.get_yf();
	}
	return result;
}

//���_�؂�ւ�
void stage::rotate_eye(float xz) {
	float central[3] = { STAGE_XZ*BOX_SIZE / 2,STAGE_Y*BOX_SIZE / 2,STAGE_XZ*BOX_SIZE / 2 };//���S��
	float copy[3] = { camera_pos[0],camera_pos[1],camera_pos[2] };//�v�Z�p�̃R�s�[
	double arc = double(xz * PI / 180);//���W�A���ɕϊ�

	//���[�J���ɕϊ�
	for (int i = 0; i < 3; i++) copy[i] = copy[i] - central[i];
	float t[3] = { copy[0],copy[1],copy[2] };//�l���ꎞ�I�ɒu���Ă���
	//��]
	//xz����
	copy[0] = t[0] * cos(arc) + t[2] * sin(arc);
	copy[2] = t[2] * cos(arc) - t[0] * sin(arc);
	//�X�V
	for (int i = 0; i < 3; i++) { camera_pos[i] = copy[i] + central[i]; }
	c_pos_num();
	sort_box();
}

//�`�揇�ɓ���ւ�
void stage::sort_box() {
	Location copy[4] = { dropping.b1.copy(),dropping.b2.copy(),dropping.b3.copy(),dropping.b4.copy() };
	my_sort(&copy[0], 0, 3);
	dropping.b1 = copy[0].copy(); dropping.b2 = copy[1].copy(); dropping.b3 = copy[2].copy(); dropping.b4 = copy[3].copy();
}
//�����̌v�Z
float stage::dist(float x1,float y1,float z1, float x2, float y2, float z2) {
	float xx = x1 - x2;
	float yy = y1 - y2;
	float zz = z1 - z2;
	return sqrt(xx * xx + yy * yy + zz * zz);
}
//�\�[�g�iLocation�j�@�J�����ʒu��̋���
void stage::my_sort(Location* a, int p, int r) {
	if (p < r) {
		int q = partition(a, p, r);
		my_sort(a, p, q - 1);
		my_sort(a, q + 1, r);
	}
}
int stage::partition(Location* a, int p, int r) {
	float x = dist(a[r].get_xf(), a[r].get_yf(), a[r].get_zf(),camera_pos[0],camera_pos[1],camera_pos[2]);
	int i = p - 1;
	for (int j = p; j < r; j++) {
		if (dist(a[j].get_xf(), a[j].get_yf(), a[j].get_zf(), camera_pos[0], camera_pos[1], camera_pos[2]) >= x) {
			i = i + 1;
			Location t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
	}
	Location t = a[i + 1];
	a[i + 1] = a[r];
	a[r] = t;
	return i + 1;
}

//��ԋ߂����S�ʒu��Ԃ�
int stage::c_pos_num() {
	int result = -1;//�Ԃ��l
	float min = FLT_MAX;
	for (int i = 0; i < 4; i++) {//�J�����ʒu�ƈ�ԋ߂����S�ʒu��T��
		float a = dist(camera_pos[0], camera_pos[1], camera_pos[2], points[i][0], points[i][1], points[i][2]);
		if (a < min) {
			min = a;
			result = i;
		}
	}
	cam_num = result;
	return result;
}

//���݂̎��_����݂��x�N�g���̕ϊ�
void stage::convert_vec(float* p) {
	float t[3] = { p[0],p[1],p[2] };
	switch (cam_num) {//�J�����ʒu�ɂ���ĕς���
	case 0://�
		break;
	case 1:
		p[0] = t[2]; p[1] = t[1]; p[2] = t[0] * -1;
		break;
	case 2:
		p[0] = t[2] * -1; p[1] = t[1]; p[2] = t[0];
		break;
	case 3:
		p[0] = t[0] * -1; p[1] = t[1]; p[2] = t[2] * -1;
		break;
	default:
		break;
	}
}
void stage::convert_vec(int* p) {//int ver
	int t[3] = { p[0],p[1],p[2] };
	switch (cam_num) {
	case 0:
		break;
	case 1:
		p[0] = t[2]; p[1] = t[1]; p[2] = t[0] * -1;
		break;
	case 2:
		p[0] = t[2] * -1; p[1] = t[1]; p[2] = t[0];
		break;
	case 3:
		p[0] = t[0] * -1; p[1] = t[1]; p[2] = t[2] * -1;
		break;
	default:
		break;
	}
}

//score�v�Z
void stage::calc_score(int n) {//��x�ɏ�������命���ƍ����_
	for (int i = 0; i < n; i++) {
		score += n * 10;
	}
}

//getter
void stage::get_camerapos(float* pos) {
	pos[0] = camera_pos[0];
	pos[1] = camera_pos[1];
	pos[2] = camera_pos[2];
}
int stage::get_cnum() {
	return cam_num;
}

int stage::get_score() {
	return score;
}

int stage::get_dropst() {
	return drop_st;
}