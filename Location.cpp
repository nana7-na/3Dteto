#include "Location.h"

//コンストラクタ
Location::Location() :x_i(0), y_i(0), z_i(0),
x_f(0), y_f(0), z_f(0)
{}
Location::Location(int x, int y, int z) : //int型
	x_i(x), y_i(y), z_i(z)
{
	x_f = convert(x);
	y_f = convert(y);
	z_f = convert(z);
}
Location::Location(float x, float y, float z) : //float型
	x_f(x), y_f(y), z_f(z)
{
	x_i = convert(x);
	y_i = convert(y);
	z_i = convert(z);
}

//自分のコピーを返す
Location Location::copy() {
	Location ll(get_xf(), get_yf(), get_zf());
	return ll;
}

//オペレーター
Location Location::operator+(Location a) {//+
	this->set_xf(this->get_xf() + a.get_xf());
	this->set_yf(this->get_yf() + a.get_yf());
	this->set_zf(this->get_zf() + a.get_zf());
	return this->copy();
}
Location Location::operator*(int i) {//*
	this->set_xf(this->get_xf() * i);
	this->set_yf(this->get_yf() * i);
	this->set_zf(this->get_zf() * i);
	return this->copy();
}
Location Location::operator+=(int i) {
	this->set_xf(this->get_xf() + i);
	this->set_yf(this->get_yf() + i);
	this->set_zf(this->get_zf() + i);
	return this->copy();
}

//足す
void Location::add(float x, float y, float z) {
	x_f += x;
	x_i = convert(x_f);
	y_f += y;
	y_i = convert(y_f);
	z_f += z;
	z_i = convert(z_f);
}
void Location::add(int x, int y, int z) {
	x_i += x;
	x_f += float(x)*BOX_SIZE;
	y_i += y;
	y_f += float(y) * BOX_SIZE;;
	z_i += z;
	z_f += float(z) * BOX_SIZE;
}

//回転
void Location::rotate(double xz, double yx, double zy) {
	Location local2 = this->copy();
	double arc[3] = { double(xz) * PI / 180,double(yx) * PI / 180,double(zy) * PI / 180 };

	//回転
	//xz平面
	set_xf(local2.get_xf() * cos(arc[0]) + local2.get_zf() * sin(arc[0]));
	set_zf(local2.get_zf() * cos(arc[0]) - local2.get_xf() * sin(arc[0]));
	local2 = this->copy();
	//yx平面
	set_xf(local2.get_xf() * cos(arc[1]) + local2.get_yf() * sin(arc[1]));
	set_yf(local2.get_yf() * cos(arc[1]) - local2.get_xf() * sin(arc[1]));
	local2 = this->copy();
	//zy平面
	set_yf(local2.get_yf() * cos(arc[2]) + local2.get_zf() * sin(arc[2]));
	set_zf(local2.get_zf() * cos(arc[2]) - local2.get_yf() * sin(arc[2]));
}

//座標intとfloatの変換
int Location::convert(float i) {
	int f = i >= 0 ? (int)(i/BOX_SIZE) : (int)i/BOX_SIZE-1;
	return f;
}
float Location::convert(int i) {
	float f = float(i) * BOX_SIZE;
	if (i >= 0)f += BOX_SIZE / 2.0f;
	else if (i < 0)f -= BOX_SIZE / 2.0f;
	return f;
}

//ゲッター
int Location::get_xi() {
	return x_i;
}
int Location::get_yi() {
	return y_i;
}
int Location::get_zi() {
	return z_i;
}
float Location::get_xf() {
	return x_f;
}
float Location::get_yf() {
	return y_f;
}
float Location::get_zf() {
	return z_f;
}
//セッター
void Location::set_xi(int i) {
	x_i = i;
	x_f = convert(x_i);
}
void Location::set_yi(int i) {
	y_i = i;
	y_f = convert(y_i);
}
void Location::set_zi(int i) {
	z_i = i;
	z_f = convert(z_i);
}
void Location::set_xf(float f) {
	x_f = f;
	x_i = convert(x_f);
}
void Location::set_yf(float f) {
	y_f = f;
	y_i = convert(y_f);
}
void Location::set_zf(float f) {
	z_f = f;
	z_i = convert(z_f);
}