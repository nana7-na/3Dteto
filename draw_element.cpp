#include "draw_element.h"

Button::Button(int x, int y, int w, int h, std::string s,int size) : x(x), y(y), width(w), hight(h),font_size(size),name(s){
	//変数初期化
	on = false;
	s_len = s.length();
	b_color[0] = 0; b_color[1] = 0; b_color[2] = 0;
	s_color[0] = 0; s_color[1] = 0; s_color[2] = 0;
}
Button::Button() {
	x = 0; y = 0; width = 0; hight = 0;
	on = false;
	font_size = 0;
	name.clear();
	s_len = 0;
	b_color[0] = 0; b_color[1] = 0; b_color[2] = 0;
	s_color[0] = 0; s_color[1] = 0; s_color[2] = 0;
}

//色のセット
void Button::setColorB(int a, int b, int c) {
	b_color[0] = a; b_color[1] = b; b_color[2] = c;
}
void Button::setColorS(int a, int b, int c) {
	s_color[0] = a; s_color[1] = b; s_color[2] = c;
}