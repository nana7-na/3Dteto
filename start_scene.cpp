#include "start_scene.h"

//�R���X�g���N�^
start_scene::start_scene():scene() {
	//������
	scene_st = 0;
	b = Button(p.get_w()/2,p.get_h()/10*7,p.get_w()/5,p.get_h()/10,"start",40);
	b.setColorB(100, 0, 0);
	b.setColorS(200, 0, 0);
	b.on = true;
	hed_size = 70;
	SetFontSize(hed_size);
	int t = GetDrawStringWidth("3D tetoris", 10);
	start_x = p.get_w() / 2 - t/2;
	start_y = p.get_h() / 2 - hed_size / 2 - p.get_h()/10;
}

//�t���[�����ɌĂяo�����֐�
int start_scene::frame(char* key) {
	input_pros(key);
	draw_scene();
	return scene_st;
}

//�V�[���`��
void start_scene::draw_scene() {
	//�����\��
	SetFontSize(25);
	int t = GetDrawStringWidth("please ENTER", 12);
	DrawString(p.get_w()/2 - t/2, b.y+b.hight , "please ENTER ..", GetColor(120, 0, 0));
	//�薼
	SetFontSize(hed_size);
	DrawString(start_x, start_y, "3D�e�g���X", GetColor(100, 0, 0));
	//�{�^��
	p.draw_button(b);
}

//���͏���
void start_scene::input_pros(char* key) {
	copy_key(key);
	if (keys[KEY_INPUT_RETURN]) {//�G���^�[�L�[
		scene_st = 2;
	}
}