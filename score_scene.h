#pragma once

#include "scene.h"

//scene_num = 3

//スコア表示画面
class score_scene : public scene{
public:
	score_scene(int s);//コンストラクタ
	int frame(char *key);//フレーム毎に呼び出される
private:
	//変数
	int score;//スコア
	char score_c[10];//スコア文字列
	int b_index;//選択中のボタン
	int w;//スコアの横幅
	Button b;//ボタン1

	void draw_scene();//画面描画
	void input_pros(char* key);//押したキーの取得をする
};