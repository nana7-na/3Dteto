#include "Pen.h"

Pen::Pen() {
	box_set();//読み込み
	GetScreenState(&width, &hight, &bit);//画面の大きさ取得
}

//モデルをセット
void Pen::box_set() {
	for (int i = 0; i < 7; i++) {
		switch (i) {
		case 0:
			boxs[i] = MV1LoadModel("boxs/box_red.fbx_Collection.mv1");// モデルの読み込み 赤
			break;
		case 1:
			boxs[i] = MV1LoadModel("boxs/box_blue.fbx_Collection.mv1");// モデルの読み込み 青
			break;
		case 2:
			boxs[i] = MV1LoadModel("boxs/box_green.fbx_Collection.mv1");// モデルの読み込み 緑
			break;
		case 3:
			boxs[i] = MV1LoadModel("boxs/box_orange.fbx_Collection.mv1");// モデルの読み込み オレンジ
			break;
		case 4:
			boxs[i] = MV1LoadModel("boxs/box_pink.fbx_Collection.mv1");// モデルの読み込み ピンク
			break;
		case 5:
			boxs[i] = MV1LoadModel("boxs/box_purple.fbx_Collection.mv1");// モデルの読み込み 紫
			break;
		case 6:
			boxs[i] = MV1LoadModel("boxs/box_yellow.fbx_Collection.mv1");// モデルの読み込み 黄色
			break;
		default:
			break;
		}
		MV1SetScale(boxs[i], VGet(0.3f, 0.3f, 0.3f));//サイズ調整
	}
}

void Pen::draw_button(Button b) {
	//ボックス描画
	DrawBox(b.x - b.width / 2, b.y - b.hight / 2, b.x + b.width / 2, b.y + b.hight / 2, GetColor(b.b_color[0], b.b_color[1], b.b_color[2]), 1);
	//文字列描画
	SetFontSize(b.font_size);
	int b_w = GetDrawStringWidth(b.name.c_str(), b.s_len);
	DrawString(b.x - b_w / 2, b.y-b.font_size/2, b.name.c_str(), GetColor(b.s_color[0], b.s_color[1], b.s_color[2]));
	if (b.on) {//選択中は周りが白くなる
		DrawBox(b.x - b.width / 2, b.y - b.hight / 2, b.x + b.width / 2, b.y + b.hight / 2, GetColor(255, 255, 255), 0);
	}
}

//箱を描く
void Pen::draw_box(VECTOR v, int model) { //位置指定してブロック描画
	MV1SetPosition(boxs[model], VGet(v.x, v.y, v.z));
	MV1DrawModel(boxs[model]);
}
void Pen::draw_box(BOX b) {//BOXを描く
	//BOX本体
	draw_box(VGet(b.b1.get_xf(), b.b1.get_yf(), b.b1.get_zf()), int(b.shape));
	draw_box(VGet(b.b2.get_xf(), b.b2.get_yf(), b.b2.get_zf()), int(b.shape));
	draw_box(VGet(b.b3.get_xf(), b.b3.get_yf(), b.b3.get_zf()), int(b.shape));
	draw_box(VGet(b.b4.get_xf(), b.b4.get_yf(), b.b4.get_zf()), int(b.shape));
}

//落下予測地点
void Pen::draw_BoxEffect(stage *s) {
	Location* ls[4] = { &(s->dropping.b1),&(s->dropping.b2),&(s->dropping.b3),&(s->dropping.b4) };
	float high = BOX_SIZE / 2;
	float y_d = abs(s->collide_next()) + BOX_SIZE / 2;
	unsigned int color[3] = { 20, 160, 200 };
	for (int i = 0; i < 4; i++) {
		//下までの線
		DrawLine3D(VGet(ls[i]->get_xf() + high, ls[i]->get_yf() - high, ls[i]->get_zf() + high), VGet(ls[i]->get_xf() + high, ls[i]->get_yf() - y_d, ls[i]->get_zf() + high), GetColor(color[0], color[1], color[2]));
		DrawLine3D(VGet(ls[i]->get_xf() - high, ls[i]->get_yf() - high, ls[i]->get_zf() + high), VGet(ls[i]->get_xf() - high, ls[i]->get_yf() - y_d, ls[i]->get_zf() + high), GetColor(color[0], color[1], color[2]));
		DrawLine3D(VGet(ls[i]->get_xf() - high, ls[i]->get_yf() - high, ls[i]->get_zf() - high), VGet(ls[i]->get_xf() - high, ls[i]->get_yf() - y_d, ls[i]->get_zf() - high), GetColor(color[0], color[1], color[2]));
		DrawLine3D(VGet(ls[i]->get_xf() + high, ls[i]->get_yf() - high, ls[i]->get_zf() - high), VGet(ls[i]->get_xf() + high, ls[i]->get_yf() - y_d, ls[i]->get_zf() - high), GetColor(color[0], color[1], color[2]));
		//着地地点にブロックを描く
		DrawLine3D(VGet(ls[i]->get_xf() + high, ls[i]->get_yf() - y_d, ls[i]->get_zf() + high), VGet(ls[i]->get_xf() + high - BOX_SIZE, ls[i]->get_yf() - y_d, ls[i]->get_zf() + high), GetColor(color[0], color[1], color[2]));
		DrawLine3D(VGet(ls[i]->get_xf() + high, ls[i]->get_yf() - y_d, ls[i]->get_zf() + high), VGet(ls[i]->get_xf() + high, ls[i]->get_yf() - y_d, ls[i]->get_zf() + high - BOX_SIZE), GetColor(color[0], color[1], color[2]));
		DrawLine3D(VGet(ls[i]->get_xf() - high, ls[i]->get_yf() - y_d, ls[i]->get_zf() - high), VGet(ls[i]->get_xf() - high + BOX_SIZE, ls[i]->get_yf() - y_d, ls[i]->get_zf() - high), GetColor(color[0], color[1], color[2]));
		DrawLine3D(VGet(ls[i]->get_xf() - high, ls[i]->get_yf() - y_d, ls[i]->get_zf() - high), VGet(ls[i]->get_xf() - high, ls[i]->get_yf() - y_d, ls[i]->get_zf() - high + BOX_SIZE), GetColor(color[0], color[1], color[2]));
	}
}

//軸を描く
void Pen::draw_axis() {
	DrawLine3D(VGet(0, 0, 0), VGet(BOX_SIZE * STAGE_XZ + 200, 0, 0), GetColor(255, 0, 0));//x軸 red
	DrawLine3D(VGet(0, 0, 0), VGet(0, BOX_SIZE * STAGE_Y, 0), GetColor(0, 255, 0));//y軸 green
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, BOX_SIZE * STAGE_XZ + 200), GetColor(0, 0, 255));//z軸 blue
	DrawLine3D(VGet(BOX_SIZE * STAGE_XZ, 0, 0), VGet(BOX_SIZE * STAGE_XZ, 0, BOX_SIZE * STAGE_XZ), GetColor(255, 0, 0));//x軸 red
	DrawLine3D(VGet(0, 0, BOX_SIZE * STAGE_XZ), VGet(BOX_SIZE * STAGE_XZ, 0, BOX_SIZE * STAGE_XZ), GetColor(0, 0, 255));//z軸 blue
	for (int i = 1; i <= 5; i++) {
		DrawLine3D(VGet(BOX_SIZE * i, 0.0f, 0.0f), VGet(BOX_SIZE * i, 0.0f, BOX_SIZE * STAGE_XZ), GetColor(255, 0, 0));//x軸 red
		DrawLine3D(VGet(0, 0, BOX_SIZE * i), VGet(BOX_SIZE * STAGE_XZ, 0, BOX_SIZE * i), GetColor(0, 0, 255));//z軸 blue
	}

	DrawLine3D(VGet(0, BOX_SIZE * STAGE_Y, 0), VGet(BOX_SIZE * STAGE_XZ, BOX_SIZE * STAGE_Y, 0), GetColor(255, 0, 0));//x軸 red
	DrawLine3D(VGet(0, BOX_SIZE * STAGE_Y, 0), VGet(0, BOX_SIZE * STAGE_Y, BOX_SIZE * STAGE_XZ), GetColor(0, 0, 255));//z軸 blue
	DrawLine3D(VGet(BOX_SIZE * STAGE_XZ, BOX_SIZE * STAGE_Y, 0), VGet(BOX_SIZE * STAGE_XZ, BOX_SIZE * STAGE_Y, BOX_SIZE * STAGE_XZ), GetColor(255, 0, 0));//x軸 red
	DrawLine3D(VGet(0, BOX_SIZE * STAGE_Y, BOX_SIZE * STAGE_XZ), VGet(BOX_SIZE * STAGE_XZ, BOX_SIZE * STAGE_Y, BOX_SIZE * STAGE_XZ), GetColor(0, 0, 255));//z軸 blue

	DrawLine3D(VGet(BOX_SIZE * STAGE_XZ, 0, 0), VGet(BOX_SIZE * STAGE_XZ, BOX_SIZE * STAGE_Y, 0), GetColor(0, 255, 0));//y軸 green
	DrawLine3D(VGet(0.0f, 0.0f, BOX_SIZE * STAGE_XZ), VGet(0.0f, BOX_SIZE * STAGE_Y, BOX_SIZE * STAGE_XZ), GetColor(0, 255, 0));//y軸 green
	DrawLine3D(VGet(BOX_SIZE * STAGE_XZ, 0.0f, BOX_SIZE * STAGE_XZ), VGet(BOX_SIZE * STAGE_XZ, BOX_SIZE * STAGE_Y, BOX_SIZE * STAGE_XZ), GetColor(0, 255, 0));//y軸 green	
}

//現在のステージを描く
void Pen::draw_stage(stage s) {
	for (int y = 0; y < STAGE_Y; y++) {
		switch(cam_num) {//カメラ位置によって描画順を変化
		case 0:
			for (int x = STAGE_XZ - 1; x >= 0; x--) {
				for (int z = STAGE_XZ - 1; z >= 0; z--) {
					box_state b = s.now_boxs[x][y][z];
					if (b.in) {
						Location l = Location(x, y, z);
						draw_box(VGet(l.get_xf(), l.get_yf(), l.get_zf()), b.color);
					}
				}
			}
			break;
		case 1:
			for (int z = STAGE_XZ - 1; z >= 0; z--) {
				for (int x = 0; x < STAGE_XZ; x++) {
					box_state b = s.now_boxs[x][y][z];
					if (b.in) {
						Location l = Location(x, y, z);
						draw_box(VGet(l.get_xf(), l.get_yf(), l.get_zf()), b.color);
					}
				}
			}
			break;
		case 2:
			for (int x = STAGE_XZ - 1; x >= 0; x--) {
				for (int z = 0; z < STAGE_XZ; z++) {
					box_state b = s.now_boxs[x][y][z];
					if (b.in) {
						Location l = Location(x, y, z);
						draw_box(VGet(l.get_xf(), l.get_yf(), l.get_zf()), b.color);
					}
				}
			}
			break;
		case 3:
			for (int x = 0; x < STAGE_XZ; x++) {
				for (int z = 0; z < STAGE_XZ; z++) {
					box_state b = s.now_boxs[x][y][z];
					if (b.in) {
						Location l = Location(x, y, z);
						draw_box(VGet(l.get_xf(), l.get_yf(), l.get_zf()), b.color);
					}
				}
			}
			break;
		default:
			for (int x = 0; x < STAGE_XZ; x++) {
				for (int z = 0; z < STAGE_XZ; z++) {
					box_state b = s.now_boxs[x][y][z];
					if (b.in) {
						Location l = Location(x, y, z);
						draw_box(VGet(l.get_xf(), l.get_yf(), l.get_zf()), b.color);
					}
				}
			}
			break;
		}
	}
}

//一番遠い場所を返す
int Pen::c_pos_num() {
	int result = -1;
	float min = FLT_MAX;
	stage s = stage();
	for (int i = 0; i < 4; i++) {
		float a = s.dist(camera_pos.x, camera_pos.y, camera_pos.z, points[i].x, points[i].y, points[i].z);//距離の計算
		if (a < min) {
			min = a;
			result = i;
		}
	}
	cam_num = result;
	return result;
}

// モデルハンドルの削除
void Pen::del_boxs() {
	for (int i = 0; i < 7; i++) {
		MV1DeleteModel(boxs[i]);
	}
}

//セッター
void Pen::set_campos(float x, float y, float z) {
	camera_pos = VGet(x, y, z);
	c_pos_num();
	SetCameraPositionAndTarget_UpVecY(camera_pos, points[cam_num]);
}

//ゲッター
int Pen::get_w() {
	return width;
}
int Pen::get_h() {
	return hight;
}
int Pen::get_bit() {
	return bit;
}
