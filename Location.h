#pragma once
#include <string>

#define PI 3.14

//��̐����`�̑傫��
#define BOX_SIZE 55

typedef struct Location {
public:
	//�R���X�g���N�^
	Location();
	Location(int x, int y, int z);
	Location(float x, float y, float z);

	//�Q�b�^�[�ƃZ�b�^�[
	int get_xi();
	int get_yi();
	int get_zi();
	float get_xf();
	float get_yf();
	float get_zf();
	void set_xi(int i);
	void set_yi(int i);
	void set_zi(int i);
	void set_xf(float f);
	void set_yf(float f);
	void set_zf(float f);

	//���Z(��ŃI�y���[�^�ɕς���?)
	Location operator+(Location a);
	Location operator*(int i);
	Location operator+=(int i);

	//���Z
	void add(float x, float y, float z);
	void add(int x, int y, int z);

	//��]
	void rotate(double xz, double yx, double zy);

	Location copy();//�R�s�[�Ԃ�

private:
	//�ʒu���
	int x_i;
	int y_i;
	int z_i;
	float x_f;
	float y_f;
	float z_f;

	//int�̏���float�̏��̕ϊ�
	int convert(float i);
	float convert(int i);
}Location;
