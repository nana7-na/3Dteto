#pragma once
#include <string>

typedef struct Button {
public:
	bool on;
	int x; int y;//描画する時の中心位置
	int width; int hight;//ボタンの幅と高さ
	int font_size;//文字列の大きさ
	int b_color[3];//ボックスの色
	std::string name;//ボタンの名前(ボタンの中心にかかれる文字列)
	int s_len;//長さ
	int s_color[3];//文字列の色

	//コンストラクタ
	Button(int x, int y, int w, int h, std::string s,int size);
	Button();

	//色のセット
	void setColorB(int a, int b, int c);
	void setColorS(int a, int b, int c);
}Button;