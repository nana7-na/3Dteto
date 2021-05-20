#include <fstream>
#include <random>]
#include <iostream>

#include "stage.h"

using namespace std;

//箱の形の数
#define SHAPES 7

//struct box_line 本体
box_line::box_line() {
	start();
}
//次の数返す
int box_line::next_num() {
	if (at == line.end()) {//
		start();
	}
	int next = *at;
	at++;
	return next;
}
//初期化
void box_line::start() {
	line = { 0, 1, 2, 3, 4, 5, 6 };
	random_device seed;
	mt19937 engine(seed());
	shuffle(line.begin(), line.end(), engine);
	at = line.begin();
}

//struct box_state
box_state::box_state():in(false),color(-1){}
box_state::box_state(bool b,int i) : in(b), color(i) {}


//class stage本体

//コンストラクタ
stage::stage() {
	initialize();
}

//初期化
void stage::initialize() {
	//配列の初期化
	for (int xx = 0; xx < STAGE_XZ; xx++) {//ステージ状態
		for (int yy = 0; yy < STAGE_Y; yy++) {
			for (int zz = 0; zz < STAGE_XZ; zz++) {
				now_boxs[xx][yy][zz].in = false;
			}
		}
	}
	//変数の初期化
	next_box = box_line();
	drop_st = 0;   
	cool_time = 0.0f;
	camera_pos[0] = 700.0f; camera_pos[1] = 1200.0f; camera_pos[2] = 600.0f;//カメラ初期位置
	c_pos_num();
	time(&prev_time);//時間をセット
	score = 0.0f;
}

//ブロックの生成と移動(フレーム毎の処理)
int stage::frame() {
	time_t now_time;//現在時間
	time(&now_time);
	float delta_time = float(now_time - prev_time);//前フレームからの経過時間

	if (drop_st == 0) {//操作中のブロックがない状態
		if (debug == 0) {
			dropping = BOX(0, STAGE_Y - 1, 0, next_box.next_num());//ブロック生成
			drop_st = 1;//状態遷移
		}
		else if (debug == 1) {//デバッグ
			dropping = BOX(0, 1, 0, next_box.next_num());//ブロック生成
			drop_st = 1;
		}
	}else if(drop_st == 1) {//ブロック操作中の状態
		if (debug == 0) {
			bool a = move(0.0f, -1.0f, 0.0f);//1.0ずつ落としていく
			if (dropping.lo.get_yf() < BOX_SIZE/2 || !a) {//何かに当たったら
				drop_st = 2;//状態遷移
				cool_time = 1.0f;//もがき状態の時間
			}
		}
		else if (debug == 1) {
		}
	}
	else if (drop_st == 2) {//もがける状態
		if ((cool_time -= delta_time) <= 0) {//もがきタイム終了
			set_box();//位置の確定
			drop_st = 0;
			cool_time = 0.0f;
		}
		if (move(0, -1, 0)) {//下に動けるようになったら落としていく(1の状態に戻す)
			move(0, 1, 0);
			drop_st = 1;
			cool_time = 0.0f;
		}
	}
	else {
		//error
	}

	//ゲームオーバー
	if (gameover()) {
		drop_st = -1;
		return drop_st;
	}

	time(&prev_time);//現在の時間を覚える
	return drop_st;
}

//消去できる列があるか探す
int stage::check_stage() {
	int result = drop_st;//drop_stの値を返す
	vector<int> del;
	del.clear();
	//消せる列を探す
	for (int y = 0; y < STAGE_Y; y++) {
		bool check = true;//最後にtrueだった場合消す
		for (int x = 0; x < STAGE_XZ; x++) {
			for (int z = 0; z < STAGE_XZ; z++) {
				if (now_boxs[x][y][z].in) {
					check = true;
				}else {
					check = false;
					break;
				}
			}
			if (!check)break;
		}
		if (check) {
        	del.push_back(y);
			result = 3;
		}
	}
	//消す
	for (int i = del.size()-1;i >= 0; i--) {
		delete_plane(del[i]);
	}
	calc_score(del.size());//スコア算出
	if (!gameover())drop_st = -1;
	return result;
}

//衝突判定
bool stage::collide() {//移動方向に行った場合衝突するかの判定(BOX用)
	//ステージの壁と衝突
	if (collide(dropping.lo) == 1 || collide(dropping.hi) == 1) {
		return true;
	}
	//既にあるブロックとの衝突
	Location* ls[4] = {&dropping.b1,&dropping.b2,&dropping.b3,&dropping.b4};
	for (int i = 0; i < 4; i++) {
		if (collide(*ls[i]) == 2) {
			return true;
		}
	}
	return false;
}
 int stage::collide(Location l) {//Locationに対して衝突判定
	int result = 0;//衝突していない->0 壁に衝突or貫通->1 その場にボックス->2
	if (l.get_xi() < 0 || l.get_xi() >= STAGE_XZ || l.get_yi() > STAGE_Y || l.get_yi() < 0 || l.get_zi() < 0 || l.get_zi() >= STAGE_XZ) {
		result = 1;
	}else if (now_boxs[l.get_xi()][l.get_yi()][l.get_zi()].in || (l.get_yf() < BOX_SIZE / 2 + l.get_yi() * BOX_SIZE && now_boxs[l.get_xi()][l.get_yi() - 1][l.get_zi()].in)) {//既にブロックがある
		result = 2;
	}
	return result;
}

//指定列が全て埋まっていた場合消去
void stage::delete_plane(int depth) {
	for (int x = 0; x < STAGE_XZ; x++) {
		for (int z = 0; z < STAGE_XZ; z++) {
			now_boxs[x][depth][z].in = false;
			now_boxs[x][depth][z].color = -1;
			for (int i = depth+1; i < STAGE_Y; i++) {
				if (now_boxs[x][i][z].in) {
					now_boxs[x][i- 1][z].in = true;
					now_boxs[x][i- 1][z].color = now_boxs[x][i][z].color;
					now_boxs[x][i][z].in = false;
					now_boxs[x][i][z].color = -1;
				}
			}
		}
	}
}

//速く落とす
int stage::shortcut() {
	if (!move(0.0f, -18.0f, 0.0f)) {
		set_box();
		drop_st = 0;
	}
	return drop_st;
}

//移動(主に左右)
bool stage::move(float x, float y, float z) {
	int vec[3] = { 1,1,1 };
	dropping.move(x, y, z);
	if (collide()) {
		dropping.move(x*-1, y*-1, z*-1);
		return false;
	}
	return true;
}
bool stage::move(int x, int y, int z) {
	dropping.move(x, y, z);
	if (collide()) {
		dropping.move(x * -1, y * -1, z * -1);
		return false;
	}
	return true;
}

//ブロックの位置固定
void stage::set_box() {
	Location ls[4] = { dropping.b1.copy(),dropping.b2.copy(),dropping.b3.copy(),dropping.b4.copy() };
	bool check = true;
	for (int i = 0; i < 4; i++) {
		if (ls[i].get_yi() >= STAGE_Y) {//ステージ外だった場合の処理
			drop_st = -1;
			check = false;
			break;
		}
		now_boxs[ls[i].get_xi()][ls[i].get_yi()][ls[i].get_zi()] = box_state(true, int(dropping.shape));
	}
	if(check)drop_st = check_stage();
}

//ゲームオーバー
bool stage::gameover() {
	for (int x = 0; x < STAGE_XZ; x++) {
		for (int z = 0; z < STAGE_XZ; z++) {
			if (now_boxs[x][14][z].in)return true;
		}
	}
	return false;
}

//回転
void stage::rotate(float xz, float yx, float zy) {
	BOX copy_b = BOX(dropping.b1, dropping.b2, dropping.b3, dropping.b4, dropping.shape);
	dropping.rotate(xz, yx, zy);//回転
	bool r = rotate_sub();//回転後にぶつかるかなどの処理
	if (!r)dropping = copy_b;//上手く行かなかった場合元に戻す
	sort_box();//描画順に並び替え
}
bool stage::rotate_sub() {
	Location copy[4] = { dropping.b1.copy(),dropping.b2.copy(),dropping.b3.copy(),dropping.b4.copy() };
	float points[3] = { 0,0,0 };//補正値
	bool rotate_f = false;//回転を戻すか
	bool loop = true;//エラーが出た場合用
	int r = collide();//衝突判定
	bool vec[6] = { 0,0,0,0,0,0 };//動ける方向

	if (!r)return true;//何事もなければ終了
	//まず壁に当たっていたら戻す
	if (dropping.hi.get_xf() > BOX_SIZE * STAGE_XZ - BOX_SIZE / 2) {
		points[0] = (BOX_SIZE * STAGE_XZ - BOX_SIZE / 2) - dropping.hi.get_xf();
	}
	else if (dropping.lo.get_xf() < BOX_SIZE / 2) {
		points[0] = BOX_SIZE / 2 - dropping.lo.get_xf();
	}
	if (dropping.lo.get_yf() < BOX_SIZE / 2) {
		points[1] = BOX_SIZE / 2 - dropping.lo.get_yf();
	}
	if (dropping.hi.get_zf() > BOX_SIZE * STAGE_XZ - BOX_SIZE / 2) {
		points[2] = (BOX_SIZE * STAGE_XZ - BOX_SIZE / 2) - dropping.hi.get_zf();
	}
	else if (dropping.lo.get_zf() < BOX_SIZE / 2) {
		points[2] = BOX_SIZE / 2 - dropping.lo.get_zf();
	}
	move(points[0], points[1], points[2]);

	r = collide();
	if (!r)return true;

	//既に配置されているブロックに衝突した場合
	//それぞれの方向に1ずつまでなら動いてok
	bool check = false;
	int moving[3] = { 0,0,0 };//動く方向
	for (int i = 0; i < 3; i++) {
		moving[i] = 1;
		for (int j = 0; j < 2; j++) {
			if (j == 0)moving[i] = -1;
			else if (j == 1)moving[i] = 1;
			if (move(moving[0], moving[1], moving[2])) {
				check = true;
				break;
			}
		}
		break;
		moving[i] = 0;
	}
	if (collide())return false;
	return true;
}

//後どれだけ落ちたら当たるか
float stage::collide_next() {
	float result = dropping.lo.get_yf();//現在の一番低いy座標
	float comp_r = dropping.lo.get_yf() - dropping.lo.get_yi() * BOX_SIZE;//現在の中心とのずれ
	Location* ls[4] = { &dropping.b1,&dropping.b2,&dropping.b3,&dropping.b4 };//コピー

	for (int i = 0; i < 4; i++) {
		int dist = 0;
		for (int y = ls[i]->get_yi()-1; y >= 0; y--) {//-1ずつ落としていき、次に何かと当たる距離をだす
			if (now_boxs[ls[i]->get_xi()][y][ls[i]->get_zi()].in) {
				break;
			}
			dist++;
		}
		if (comp_r + dist * BOX_SIZE < result) {//今までで一番短い距離ならresultに入れる
			result = comp_r + dist * BOX_SIZE;
		}
	}
	result -= BOX_SIZE / 2;//補正
	if (result < 0) {//マイナスになってしまった場合の処理
		result = BOX_SIZE/2 - dropping.lo.get_yf();
	}
	return result;
}

//視点切り替え
void stage::rotate_eye(float xz) {
	float central[3] = { STAGE_XZ*BOX_SIZE / 2,STAGE_Y*BOX_SIZE / 2,STAGE_XZ*BOX_SIZE / 2 };//中心軸
	float copy[3] = { camera_pos[0],camera_pos[1],camera_pos[2] };//計算用のコピー
	double arc = double(xz * PI / 180);//ラジアンに変換

	//ローカルに変換
	for (int i = 0; i < 3; i++) copy[i] = copy[i] - central[i];
	float t[3] = { copy[0],copy[1],copy[2] };//値を一時的に置いておく
	//回転
	//xz平面
	copy[0] = t[0] * cos(arc) + t[2] * sin(arc);
	copy[2] = t[2] * cos(arc) - t[0] * sin(arc);
	//更新
	for (int i = 0; i < 3; i++) { camera_pos[i] = copy[i] + central[i]; }
	c_pos_num();
	sort_box();
}

//描画順に入れ替え
void stage::sort_box() {
	Location copy[4] = { dropping.b1.copy(),dropping.b2.copy(),dropping.b3.copy(),dropping.b4.copy() };
	my_sort(&copy[0], 0, 3);
	dropping.b1 = copy[0].copy(); dropping.b2 = copy[1].copy(); dropping.b3 = copy[2].copy(); dropping.b4 = copy[3].copy();
}
//距離の計算
float stage::dist(float x1,float y1,float z1, float x2, float y2, float z2) {
	float xx = x1 - x2;
	float yy = y1 - y2;
	float zz = z1 - z2;
	return sqrt(xx * xx + yy * yy + zz * zz);
}
//ソート（Location）　カメラ位置空の距離
void stage::my_sort(Location* a, int p, int r) {
	if (p < r) {
		int q = partition(a, p, r);
		my_sort(a, p, q - 1);
		my_sort(a, q + 1, r);
	}
}
int stage::partition(Location* a, int p, int r) {
	float x = dist(a[r].get_xf(), a[r].get_yf(), a[r].get_zf(),camera_pos[0],camera_pos[1],camera_pos[2]);
	int i = p - 1;
	for (int j = p; j < r; j++) {
		if (dist(a[j].get_xf(), a[j].get_yf(), a[j].get_zf(), camera_pos[0], camera_pos[1], camera_pos[2]) >= x) {
			i = i + 1;
			Location t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
	}
	Location t = a[i + 1];
	a[i + 1] = a[r];
	a[r] = t;
	return i + 1;
}

//一番近い中心位置を返す
int stage::c_pos_num() {
	int result = -1;//返す値
	float min = FLT_MAX;
	for (int i = 0; i < 4; i++) {//カメラ位置と一番近い中心位置を探す
		float a = dist(camera_pos[0], camera_pos[1], camera_pos[2], points[i][0], points[i][1], points[i][2]);
		if (a < min) {
			min = a;
			result = i;
		}
	}
	cam_num = result;
	return result;
}

//現在の視点からみたベクトルの変換
void stage::convert_vec(float* p) {
	float t[3] = { p[0],p[1],p[2] };
	switch (cam_num) {//カメラ位置によって変える
	case 0://基準
		break;
	case 1:
		p[0] = t[2]; p[1] = t[1]; p[2] = t[0] * -1;
		break;
	case 2:
		p[0] = t[2] * -1; p[1] = t[1]; p[2] = t[0];
		break;
	case 3:
		p[0] = t[0] * -1; p[1] = t[1]; p[2] = t[2] * -1;
		break;
	default:
		break;
	}
}
void stage::convert_vec(int* p) {//int ver
	int t[3] = { p[0],p[1],p[2] };
	switch (cam_num) {
	case 0:
		break;
	case 1:
		p[0] = t[2]; p[1] = t[1]; p[2] = t[0] * -1;
		break;
	case 2:
		p[0] = t[2] * -1; p[1] = t[1]; p[2] = t[0];
		break;
	case 3:
		p[0] = t[0] * -1; p[1] = t[1]; p[2] = t[2] * -1;
		break;
	default:
		break;
	}
}

//score計算
void stage::calc_score(int n) {//一度に消せた列牙多いと高得点
	for (int i = 0; i < n; i++) {
		score += n * 10;
	}
}

//getter
void stage::get_camerapos(float* pos) {
	pos[0] = camera_pos[0];
	pos[1] = camera_pos[1];
	pos[2] = camera_pos[2];
}
int stage::get_cnum() {
	return cam_num;
}

int stage::get_score() {
	return score;
}

int stage::get_dropst() {
	return drop_st;
}