#include "main_scene.h"

#include <string>

//コンストラクタ
main_scene::main_scene(): scene() {
	stage ss = stage();
	s = &ss;
	drop_st = 0;
}
main_scene::main_scene(stage* s):s(s),scene() {
	drop_st = 0;
	s->initialize();
}

//フレーム毎に呼出し
int main_scene::frame(char* key) {
	input_pros(key);
	drop_st = s->frame();//情報更新
	//ゲームオーバー
	if (drop_st == -1) {
		scene_st = 3;
		p.del_boxs();
		return scene_st;
	}
	draw_scene();//描画
	return scene_st;
}

//シーンの描画
void main_scene::draw_scene() {
	float t[3] = { 0.0f,0.0f,0.0f };
	s->get_camerapos(&t[0]);//カメラ位置を取得
	p.set_campos(t[0], t[1], t[2]);//描画用クラスに渡す
	p.draw_axis();//軸描画
	p.draw_stage(*s);//ステージ状況描画
	//現在のスコア表示
	SetFontSize(50);
	DrawString(0, 0, "Score:", GetColor(150, 0, 0));
	int w = GetDrawStringWidth("Score:", 6);
	DrawString(w/6, 50, std::to_string(s->get_score()).c_str(), GetColor(200, 0, 0));
	//操作中BOXなどの描画
	if (drop_st != 0) {
		p.draw_BoxEffect(s);
		p.draw_box(s->dropping);
	}
}

//入力処理
void main_scene::input_pros(char* key) {
	copy_key(key);
	int i = s->get_dropst();//現在移動中のBOXの状態
	if (i == 1 || i == 2) {
		//十字キーで移動
		if (keys[KEY_INPUT_UP] && !prev_key[KEY_INPUT_UP]) {//上キー
			int vec[3] = { -1,0,0 };
			s->convert_vec(&vec[0]);
			s->move(int(vec[0]), int(vec[1]), int(vec[2]));
		}
		if (keys[KEY_INPUT_LEFT] && !prev_key[KEY_INPUT_LEFT]) {//左キー
			int vec[3] = { 0,0,-1 };
			s->convert_vec(&vec[0]);
			s->move(int(vec[0]), int(vec[1]), int(vec[2]));
		}
		if (keys[KEY_INPUT_RIGHT] && !prev_key[KEY_INPUT_RIGHT]) {//右キー
			int vec[3] = { 0,0,1 };
			s->convert_vec(&vec[0]);
			s->move(int(vec[0]), int(vec[1]), int(vec[2]));
		}
		if (keys[KEY_INPUT_DOWN] && !prev_key[KEY_INPUT_DOWN]) {//下キー
			int vec[3] = { 1,0,0 };
			s->convert_vec(&vec[0]);
			s->move(int(vec[0]), int(vec[1]), int(vec[2]));
		}
		//ASDQWEで回転
		if (keys[KEY_INPUT_A] && !prev_key[KEY_INPUT_A]) {//A
			s->rotate(0.0f, -90.0f, 0.0f);
		}
		if (keys[KEY_INPUT_S] && !prev_key[KEY_INPUT_S]) {//S
			s->rotate(0.0f, 0.0f, -90.0f);
		}
		if (keys[KEY_INPUT_D] && !prev_key[KEY_INPUT_D]) {//D
			s->rotate(90.0f, 0.0f, 0.0f);
		}
		if (keys[KEY_INPUT_Q] && !prev_key[KEY_INPUT_Q]) {//Q
			s->rotate(0.0f, 90.0f, 0.0f);
		}
		if (keys[KEY_INPUT_W] && !prev_key[KEY_INPUT_W]) {//W
			s->rotate(0.0f, 0.0f, 90.0f);
		}
		if (keys[KEY_INPUT_E] && !prev_key[KEY_INPUT_E]) {//E
			s->rotate(-90.0f, 0.0f, 0.0f);
		}
		if (keys[KEY_INPUT_SPACE]) {//スペースを押している間速くなる
			i = s->shortcut();
		}
		//視点変更
		if (keys[KEY_INPUT_R] && !prev_key[KEY_INPUT_R]) {
			s->rotate_eye(90.0f);
		}
	}
}
