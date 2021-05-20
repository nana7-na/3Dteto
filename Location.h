#pragma once
#include <string>

#define PI 3.14

//一つの正方形の大きさ
#define BOX_SIZE 55

typedef struct Location {
public:
	//コンストラクタ
	Location();
	Location(int x, int y, int z);
	Location(float x, float y, float z);

	//ゲッターとセッター
	int get_xi();
	int get_yi();
	int get_zi();
	float get_xf();
	float get_yf();
	float get_zf();
	void set_xi(int i);
	void set_yi(int i);
	void set_zi(int i);
	void set_xf(float f);
	void set_yf(float f);
	void set_zf(float f);

	//加算(後でオペレータに変える?)
	Location operator+(Location a);
	Location operator*(int i);
	Location operator+=(int i);

	//加算
	void add(float x, float y, float z);
	void add(int x, int y, int z);

	//回転
	void rotate(double xz, double yx, double zy);

	Location copy();//コピー返す

private:
	//位置情報
	int x_i;
	int y_i;
	int z_i;
	float x_f;
	float y_f;
	float z_f;

	//intの情報とfloatの情報の変換
	int convert(float i);
	float convert(int i);
}Location;
