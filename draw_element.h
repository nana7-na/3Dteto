#pragma once
#include <string>

typedef struct Button {
public:
	bool on;
	int x; int y;//�`�悷�鎞�̒��S�ʒu
	int width; int hight;//�{�^���̕��ƍ���
	int font_size;//������̑傫��
	int b_color[3];//�{�b�N�X�̐F
	std::string name;//�{�^���̖��O(�{�^���̒��S�ɂ�����镶����)
	int s_len;//����
	int s_color[3];//������̐F

	//�R���X�g���N�^
	Button(int x, int y, int w, int h, std::string s,int size);
	Button();

	//�F�̃Z�b�g
	void setColorB(int a, int b, int c);
	void setColorS(int a, int b, int c);
}Button;