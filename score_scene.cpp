#include "score_scene.h"

//コンストラクタ
score_scene::score_scene(int s):score(s), scene() {
	for (int i = 0; i < 256; i++) {//入力取得用の引数を初期化
		keys[i] = 0;
		prev_key[i] = 0;
	}
	snprintf(score_c,10,"%d",score);//スコアの文字列作成
	b_index = 0;//選択中のボタン
	SetFontSize(60);
	w = GetDrawStringWidth(score_c, to_string(score).length());//スコア文字列の横幅
	scene_st = 0;//シーン状態
	//ボタン作成
	b = Button(p.get_w() / 2, p.get_h() / 10 * 7, p.get_w() / 3, p.get_h() / 10, "restart", 40);
	b.setColorB(100, 0, 0);
	b.setColorS(200, 0, 0);
	b.on = true;
}

//フレーム毎に呼び出される関数
int score_scene::frame(char *key) {
	input_pros(key);//入力処理
	draw_scene();//画面描画
	return scene_st;//状態管理変数を返す
}

//画面の描画
void score_scene::draw_scene() {
	//スコア表示
	SetFontSize(40);
	int string_w = GetDrawStringWidth("Your Score is...", 16);
	DrawString(p.get_w() / 2 - string_w/2, p.get_h() / 2 - 100, "Your Score is...", GetColor(255, 0, 0));
	SetFontSize(60);
	DrawString(p.get_w()/2-w/2,p.get_h()/2-30,to_string(score).c_str(),GetColor(255,0,0));
	//ボタン表示
   	p.draw_button(b);
	SetFontSize(25);
	int t = GetDrawStringWidth("please ENTER", 12);
	DrawString(p.get_w() / 2 - t / 2, b.y + b.hight, "please ENTER ..", GetColor(120, 0, 0));
 }

//入力処理関数
void score_scene::input_pros(char *key) {
	copy_key(key);//入力を変数に覚えさせる
	if (keys[KEY_INPUT_UP]) {//上
		if (b_index != 0) {
			b_index--;
		}
	}else if (keys[KEY_INPUT_DOWN]) {//下
		if (b_index != 0) {
			b_index++;
		}
	}else if (keys[KEY_INPUT_RETURN]) {//エンターキー
		scene_st = 2;
	}
}