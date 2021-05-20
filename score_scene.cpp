#include "score_scene.h"

//�R���X�g���N�^
score_scene::score_scene(int s):score(s), scene() {
	for (int i = 0; i < 256; i++) {//���͎擾�p�̈�����������
		keys[i] = 0;
		prev_key[i] = 0;
	}
	snprintf(score_c,10,"%d",score);//�X�R�A�̕�����쐬
	b_index = 0;//�I�𒆂̃{�^��
	SetFontSize(60);
	w = GetDrawStringWidth(score_c, to_string(score).length());//�X�R�A������̉���
	scene_st = 0;//�V�[�����
	//�{�^���쐬
	b = Button(p.get_w() / 2, p.get_h() / 10 * 7, p.get_w() / 3, p.get_h() / 10, "restart", 40);
	b.setColorB(100, 0, 0);
	b.setColorS(200, 0, 0);
	b.on = true;
}

//�t���[�����ɌĂяo�����֐�
int score_scene::frame(char *key) {
	input_pros(key);//���͏���
	draw_scene();//��ʕ`��
	return scene_st;//��ԊǗ��ϐ���Ԃ�
}

//��ʂ̕`��
void score_scene::draw_scene() {
	//�X�R�A�\��
	SetFontSize(40);
	int string_w = GetDrawStringWidth("Your Score is...", 16);
	DrawString(p.get_w() / 2 - string_w/2, p.get_h() / 2 - 100, "Your Score is...", GetColor(255, 0, 0));
	SetFontSize(60);
	DrawString(p.get_w()/2-w/2,p.get_h()/2-30,to_string(score).c_str(),GetColor(255,0,0));
	//�{�^���\��
   	p.draw_button(b);
	SetFontSize(25);
	int t = GetDrawStringWidth("please ENTER", 12);
	DrawString(p.get_w() / 2 - t / 2, b.y + b.hight, "please ENTER ..", GetColor(120, 0, 0));
 }

//���͏����֐�
void score_scene::input_pros(char *key) {
	copy_key(key);//���͂�ϐ��Ɋo��������
	if (keys[KEY_INPUT_UP]) {//��
		if (b_index != 0) {
			b_index--;
		}
	}else if (keys[KEY_INPUT_DOWN]) {//��
		if (b_index != 0) {
			b_index++;
		}
	}else if (keys[KEY_INPUT_RETURN]) {//�G���^�[�L�[
		scene_st = 2;
	}
}