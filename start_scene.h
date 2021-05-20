#pragma once

#include "scene.h"

//scene num = 1

//スタート画面
class start_scene : public  scene {
public:
	start_scene();//コンストラクタ
	int frame(char *key);//フレーム毎に呼び出される関数
protected:
private:
	//変数
	Button b;//ボタン
	int start_x; int start_y;//タイトル表示の位置
	int hed_size;//タイトルの文字サイズ
	int b_size;//ボタンの名前の文字サイズ

	void draw_scene();//シーン描画
	void input_pros(char* key);//押したキーの取得をする
};