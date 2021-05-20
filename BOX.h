#pragma once
#include "Location.h"

using namespace std;

//テトリスのブロックを敷き詰める部分の大きさ
#define STAGE_XZ 5
#define STAGE_Y 15

//操作しているブロック
class BOX {
public:
	Location b1, b2, b3, b4;//各ブロックの位置
	Location bc;//中心位置の保持
	Location hi;//各軸の一番高い値をそれぞれ保持
	Location lo;//低い値

	double shape;//形

	//コンストラクタ
	BOX();
	BOX(int x, int y, int z, double shape_num);
	BOX(float x, float y, float z, double shape_num);
	BOX(Location l1, Location l2, Location l3, Location l4, double shape);

	//移動
	void move(int x, int y, int z);
	void move(float x, float y, float z);

	//回転
	void rotate(float xz, float yx, float zy);
	void rotate_sub();

private:
	//形決める
	void set_shape(double shape_num);

	//一番高い位置と低い位置の箱を設定する
	void hiilo();
};
