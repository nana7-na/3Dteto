#pragma once
#include<vector>
#include <time.h>

#include "BOX.h"

struct box_state {
	bool in;
	int color;
	box_state();
	box_state(bool b, int i);
};

//七つの形が偏りなく出現するようにする
typedef struct box_line {
public:
	//どの形が出てくるかの順番
	vector<int> line;
	vector<int>::iterator at;//今のイテレーター
	
	box_line();//コンストラクタ

	int next_num();//次の形を返す

private:
	void start();//初期化関数
} box_line;

class stage {
public:
	struct box_state now_boxs[STAGE_XZ][STAGE_Y][STAGE_XZ];//現在のブロック状況 x,y,z
	BOX dropping; //操作中のブロック

	stage();//コンストラクタ
	//初期化関数
	void initialize();

	//フレーム毎の処理
	int frame();

	//depthの深さに箱が詰まっていたら、箱を消す。
	void delete_plane(int depth);

	//すぐに下に落とす
	int shortcut();

	//何かに当たるまでの距離
	float collide_next();

	//距離の計算
	float dist(float x1, float y1, float z1, float x2, float y2, float z2);

	//移動
	bool move(float x, float y, float z);
	bool move(int x, int y, int z);

	//回転(BOX)
	void rotate(float xz, float yx, float zy);

	//視点移動
	void rotate_eye(float xz);

	//ベクトルの変換（座標変換はできない）
	void convert_vec(float* p);
	void convert_vec(int* p);

	//ゲームオーバー
	bool gameover();

	//ゲッター
	void get_camerapos(float* pos);
	int get_cnum();
	int get_score();
	int get_dropst();

private:
	struct box_line next_box;//次のブロックの形を保存
	int drop_st;//現在の操作ブロックの状態 0:ブロック無し 1:ブロック操作中 2:一時的に操作できる時間
	time_t prev_time;//経過時間計測用(前回の時間を入れる)
	float cool_time;//クールタイム
	int debug = 0;//デバッグモード
	int score;//スコア
	float camera_pos[3];//現在の回転位置を表す
	float h = 150.0f;//pointsの補正
	float points[4][3] = { {BOX_SIZE * STAGE_XZ, STAGE_Y * BOX_SIZE / 2 + h,BOX_SIZE * STAGE_XZ } ,{BOX_SIZE * STAGE_XZ,STAGE_Y * BOX_SIZE / 2 + h,0.0f},{0.0f,STAGE_Y * BOX_SIZE / 2 + h,BOX_SIZE * STAGE_XZ},{0.0f,STAGE_Y * BOX_SIZE / 2.0f + h,0.0f} };//カメラの中心位置
	int cam_num;//現在のカメラに対してどのpointsの値を使うか

	//箱同士の衝突判定
	bool collide();
	int collide(Location l);

	//ブロックを固定
	void set_box();

	//埋まりきっている列を探す
	int check_stage();

	//回転の補助関数
	bool rotate_sub();

	//BOXの描画順を整える
	void sort_box();
	void my_sort(Location* a, int p, int r);
	int partition(Location* a, int p, int r);

	//スコア算出
	void calc_score(int n);

	//cam_numの設定
	int c_pos_num();
};
