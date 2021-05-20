#include <fstream>
#include <random>
#include <cmath>
#include <iostream>

#include "BOX.h"

//コンストラクタ
BOX::BOX()
{
	b1 = Location(0, 0, 0);
	lo = Location(5, 15, 5);
	hi = Location(0, 0, 0);
	set_shape(7);
	hiilo();
}
BOX::BOX(float x, float y, float z, double shape_num) {
	b1 = Location(x, y, z);
	lo = Location(5, 15, 5);
	hi = Location(0, 0, 0);
	set_shape(shape_num);
	hiilo();
}
BOX::BOX(int x, int y, int z, double shape_num) {
	b1 = Location(x, y, z);
	lo = Location(5, 15, 5);
	hi = Location(0, 0, 0);
	set_shape(shape_num);
	hiilo();
}
BOX::BOX(Location l1, Location l2, Location l3, Location l4, double shape) {
	b1 = l1.copy(); b2 = l2.copy(); b3 = l3.copy(); b4 = l4.copy();
	this->shape = shape;
	lo = Location(5, 15, 5);
	hi = Location(0, 0, 0);
	hiilo();
}

//形状設定
void BOX::set_shape(double shape_num) {
	shape = shape_num;//変数に格納
	switch (int(shape)) {
	case 0:
		/*
		*　□□
		*  □□
		*/
		b2 = Location(b1.get_xi() + 1, b1.get_yi(), b1.get_zi());
		b3 = Location(b1.get_xi(), b1.get_yi(), b1.get_zi() + 1);
		b4 = Location(b1.get_xi() + 1, b1.get_yi(), b1.get_zi() + 1);
		break;
	case 1:
		/*
		*　□
		*  □
		*  □
		*  □
		*/
		b2 = Location(b1.get_xi(), b1.get_yi(), b1.get_zi() + 1);
		b3 = Location(b1.get_xi(), b1.get_yi(), b1.get_zi() + 2);
		b4 = Location(b1.get_xi(), b1.get_yi(), b1.get_zi() + 3);
		break;
	case 2:
		/*
		*　2*□□
		*    □
		*/
		b2 = Location(b1.get_xi(), b1.get_yi() - 1, b1.get_zi());
		b3 = Location(b1.get_xi(), b1.get_yi() - 1, b1.get_zi() + 1);
		b4 = Location(b1.get_xi() + 1, b1.get_yi() - 1, b1.get_zi());
		break;
	case 3:
		/*
		*　2*□□
		*      □
		*/
		b2 = Location(b1.get_xi(), b1.get_yi() - 1, b1.get_zi());
		b3 = Location(b1.get_xi() + 1, b1.get_yi() - 1, b1.get_zi());
		b4 = Location(b1.get_xi() + 1, b1.get_yi() - 1, b1.get_zi() + 1);
		break;
	case 4:
		/*
		 * ■■
		 * ■
		 * ■
		 */
		b2 = Location(b1.get_xi(), b1.get_yi(), b1.get_zi() + 1);
		b3 = Location(b1.get_xi() + 1, b1.get_yi(), b1.get_zi());
		b4 = Location(b1.get_xi() + 2, b1.get_yi(), b1.get_zi());
		break;
	case 5:
		/*
		 * ■
		 * ■■
		 * ■
		 */
		b2 = Location(b1.get_xi() + 1, b1.get_yi(), b1.get_zi());
		b3 = Location(b1.get_xi() + 2, b1.get_yi(), b1.get_zi());
		b4 = Location(b1.get_xi() + 1, b1.get_yi(), b1.get_zi() + 1);
		break;
	case 6:
		/* ■■
		 *   ■■
		 */
		b2 = Location(b1.get_xi() + 1, b1.get_yi(), b1.get_zi());
		b3 = Location(b1.get_xi() + 1, b1.get_yi(), b1.get_zi() + 1);
		b4 = Location(b1.get_xi() + 2, b1.get_yi(), b1.get_zi() + 1);
		break;
	default:
		b2 = Location(0, 0, 0);
		b3 = Location(0, 0, 0);
		b4 = Location(0, 0, 0);
		break;
	};
}

//移動
void BOX::move(int x, int y, int z) {
	b1.add(x, y, z);
	b2.add(x, y, z);
	b3.add(x, y, z);
	b4.add(x, y, z);
	hiilo();
}
void BOX::move(float x, float y, float z) {
	b1.add(x, y, z);
	b2.add(x, y, z);
	b3.add(x, y, z);
	b4.add(x, y, z);
	hiilo();
}

//回転
void BOX::rotate(float xz, float yx, float zy) {
		Location copy[4] = { b1.copy(), b2.copy(),b3.copy(),b4.copy() };
		hiilo();
		//角度をラジアンに変換
		double arc[3] = { double(xz) * PI / 180,double(yx) * PI / 180,double(zy) * PI / 180 };
		for (int i = 0; i < 4;i++) {
			//ローカル座標に変換
			Location local = copy[i].copy();
			local.set_xf(local.get_xf()-bc.get_xf());
			local.set_yf(local.get_yf()- bc.get_yf());
			local.set_zf(local.get_zf() - bc.get_zf());
			local.rotate(double(xz), double(yx), double(zy));
			//ワールド座標に変換
			copy[i].set_xf(local.get_xf() + bc.get_xf()); copy[i].set_yf(local.get_yf() + bc.get_yf()); copy[i].set_zf(local.get_zf() + bc.get_zf());
		}
		b1 = copy[0];b2 = copy[1];b3 = copy[2];b4 = copy[3];//反映させる
		rotate_sub();//xzの最適化（グリッドに合わせる）
}
//xzの補正
void BOX::rotate_sub() {
	float points[3] = { 0.0f,0.0f,0.0f };//補正値
	//b1を基にして補正
	float x = b1.get_xi() * BOX_SIZE + float(BOX_SIZE / 2.0f);
	float z = b1.get_zi() * BOX_SIZE + float(BOX_SIZE / 2.0f);
	points[0] = x - b1.get_xf();
	points[2] = z - b1.get_zf();
	move(points[0], points[1], points[2]);//移動
}

//x,y,zそれぞれの一番大きい値と小さい値を保存
void BOX::hiilo() {
	Location ls[4] = { b1.copy(),b2.copy(),b3.copy(),b4.copy() };//操作用にコピーをとる
	hi = Location(0, 0, 0);//初期化
	lo = Location(STAGE_XZ, STAGE_Y, STAGE_XZ);//初期化

	for (int i = 0; i < 4;i++) {
		if (ls[i].get_xf() > hi.get_xf()) {//hi_x
			hi.set_xf(ls[i].get_xf());
		}
		if (ls[i].get_xf() < lo.get_xf()) {//lo_x
			lo.set_xf(ls[i].get_xf());
		}
		if (ls[i].get_yf() > hi.get_yf()) {//hi_y
			hi.set_yf(ls[i].get_yf());
		}
		if (ls[i].get_yf() < lo.get_yf()) {//lo_y
			lo.set_yf(ls[i].get_yf());
		}if (ls[i].get_zf() > hi.get_zf()) {//hi_z
			hi.set_zf(ls[i].get_zf());
		}
		if (ls[i].get_zf() < lo.get_zf()) {//lo_z
			lo.set_zf(ls[i].get_zf());
		}
	}
	bc = Location((lo.get_xf() + hi.get_xf()) / 2, (lo.get_yf() + hi.get_yf()) / 2, (lo.get_zf() + hi.get_zf()) / 2);//中心位置を更新
}