#include <stdio.h>
#include <iostream>

#include "start_scene.h"
#include "score_scene.h"
#include "main_scene.h"

using namespace std;

//DXライブラリのメイン関数
int WINAPI WinMain(HINSTANCE hInstace, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow) {
	//DXライブラリの設定
	SetOutApplicationLogValidFlag(FALSE); //Log.txtを生成しないようにする
	ChangeWindowMode(TRUE);//全画面にしない
	SetGraphMode(640, 700, 32);//画面サイズ設定
	SetUseLighting(0);//ライトを消す
	if (DxLib_Init() == -1) {//失敗した場合のエラー処理
		return -1;
	}

	//変数の初期化
	char key[256];//入力ゲット
	scene* now = new start_scene();;//現在のシーン
	stage s = stage();//状態管理変数
	int state = 0;//シーンの状態管理

	for (int i = 0; i < 256; i++)key[i] = 0;
	
	//カメラ設定(表)
	SetCameraNearFar(0.1f, 10000.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(700.0f,1200.0f,600.0f), VGet(STAGE_XZ/2, 1000, STAGE_XZ/2));

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);
	//カメラ設定(裏)
	SetCameraNearFar(0.1f, 10000.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(700.0f, 1200.0f, 600.0f), VGet(STAGE_XZ / 2, 1000, STAGE_XZ / 2));
	
	//画面アップデート
	while (ProcessMessage() == 0) {
		ClearDrawScreen(); //画面消去
		GetHitKeyStateAll(key);//入力取得
		int i = now->frame(key);//フレーム毎の処理

		if (i != state) {//画面遷移
			state = i;
			switch (state) {
				case 0://続行
					state = i;
					break;
				case 1://スタート画面へ
					now = new start_scene();
					break;
				case 2://メイン画面へ
					now = new main_scene(&s);
					break;
				case 3://スコア画面へ
					now = new score_scene(s.get_score());
					break;
				case -1:
					break;
			}
		}

		if(state == -1 || ProcessMessage() == -1)break;//ゲームオーバーorエラーの時に終了

		//交換
		DxLib::ScreenFlip();
	}

	delete now;
	DxLib::DxLib_End();//DXライブラリ終了

	return 0;
}