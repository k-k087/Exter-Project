#ifndef __FPS_H__
#define __FPS_H__

class CFrameRate
{
public:
	static int currentTime;		//現在の時間
	static int lastFrameTime;	//前回のフレーム実行時の時間
	static int count;			//フレームカウント用
	static int calcFpsTime;		//FPSを計算した時間
	static float fps;			//計測したFPS（表示用）

	//FPS初期化
	static void Init();

	//FPSステップ
	static void Step();

	//フレームレート情報更新
	static void Update();

	//1/60秒経過しているかの判定
	static bool IsPassageFrameTime();

	//FPS計算
	static void Calc();

	//FPS表示（デバッグ用）
	static void Draw();
};



#endif
