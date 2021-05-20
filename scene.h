#pragma once

#include "Pen.h"

class scene{
public:
	scene() {//コンストラクタ
		scene_st = 0;
		for (int i = 0; i < 256; i++) {
			keys[i] = 0; prev_key[i] = 0;
		}
		p = Pen();
	}

	virtual int frame(char* key) = 0;//フレーム毎に呼び出される関数

	virtual void draw_scene() = 0;//シーンの描画

protected:
	int scene_st;//シーンの状態管理変数
	char keys[256];//現在の入力
	char prev_key[256];//前回の入力
	Pen p;//描画関数

	//入力状態の取得
	void copy_key(char* key) {
		for (int i = 0; i < 256; i++) {
			//前回の状態を覚える
			if (keys[i]) {
				prev_key[i] = 1;
			}
			else if (!keys[i]) {
				prev_key[i] = 0;
			}
			//今回の入力を入れる
			if (key[i]) {
				keys[i] = 1;
			}
			else if (!key[i]) {
				keys[i] = 0;
			}
		}
	}

private:
	virtual void input_pros(char* key) = 0;//押したキーの取得をする
};