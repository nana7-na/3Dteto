#pragma once
#include "scene.h"

//scene_num = 2

class main_scene : public scene{
public:
	//コンストラクタ
	main_scene();
	main_scene(stage *s);

	int frame(char* key);//フレーム毎に呼び出される関数

protected:

private:
	//変数
	stage *s;//ステージ情報
	int drop_st;//ステージ状態管理

	void draw_scene();//シーンの描画

	void input_pros(char* key);//入力処理

};
