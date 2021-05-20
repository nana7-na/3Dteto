#pragma once

#include "DxLib.h"
#include "draw_element.h"
#include "stage.h"

class Pen {
public:
	Pen();//コンストラクタ

	void draw_button(Button b);//ボタンの描画
	void draw_box(VECTOR v, int model);//ブロックを描く
	void draw_box(BOX b);//指定されたBOXを描く
	void draw_BoxEffect(stage *s);//着地地点を描く
	void draw_axis();//軸を描く
	void draw_stage(stage s);//現在のステージ情報を描く

	//後処理
	void del_boxs();//最後にモデルを消す

	//セッター
	void set_campos(float x, float y, float z);

	//ゲッター
	int get_w();
	int get_h();
	int get_bit();

private:
	int width;//画面幅
	int hight;//画面高さ
	int bit;//画面サイズ
	VECTOR camera_pos;//カメラ位置
	int boxs[7];//各ブロックのモデル
	float h = 150.0f;//pointsの補正
	VECTOR points[4] = { VGet(0.0f,STAGE_Y * BOX_SIZE / 2.0f + h,0.0f),VGet(BOX_SIZE * STAGE_XZ,STAGE_Y * BOX_SIZE / 2 + h,0.0f),VGet(0.0f,STAGE_Y * BOX_SIZE / 2 + h,BOX_SIZE * STAGE_XZ),VGet(BOX_SIZE * STAGE_XZ, STAGE_Y * BOX_SIZE / 2 + h,BOX_SIZE * STAGE_XZ) };//カメラの中心位置
	int cam_num;//どのpointsの値を使うか

	//セッティング
	void box_set();//モデルをセットする

	//cam_numの設定をする
	int c_pos_num();
};