#include "FPS.h"
#include "DxLib.h"
#include "../Common.h"

int CFrameRate::currentTime;	//現在の時間
int CFrameRate::lastFrameTime;	//前回のフレーム実行時の時間
int CFrameRate::count;			//フレームカウント用
int CFrameRate::calcFpsTime;	//FPSを計算した時間
float CFrameRate::fps;			//計測したFPS（表示用）

//FPS初期化
void CFrameRate::Init()
{
	currentTime = 0;
	lastFrameTime = 0;
	count = 0;
	calcFpsTime = 0;
	fps = 0.f;
}

//FPSステップ
void CFrameRate::Step()
{
	//現在の時間を取得
	currentTime = GetNowCount();

	//最初のループなら
	//現在の時間を、FPSの計算をした時間に設定
	if (calcFpsTime == 0.0f)
	{
		calcFpsTime = currentTime;
		fps = (float)FRAME_RATE;
	}
}

//フレームレート情報更新
void CFrameRate::Update()
{
	lastFrameTime = currentTime;
	count++;
}

//1/60秒経過しているかの判定
bool CFrameRate::IsPassageFrameTime()
{
	if (currentTime - lastFrameTime >= FRAME_TIME)
		return true;

	else return false;
}

//FPS計算
void CFrameRate::Calc()
{
	//前回のFPSを計算した時間からの経過時間を求める
	int difTime = currentTime - calcFpsTime;

	//前回のFPSを計算した時間から
	//１秒以上経過していたらFPSを計算する
	if (difTime > 1000)
	{
		//フレーム回数をミリ秒に合わせる
		//小数まで出したいのでfloatにキャスト
		float frameCount = (float)(count * 1000.0f);

		//FPSを求める
		//理想の数値は 60000 / 1000 で 60 となる
		fps = frameCount / difTime;

		//フレームレートカウントをクリア
		count = 0;

		//FPSを計算した時間を更新
		calcFpsTime = currentTime;
	}
}

//FPS表示（デバッグ用）
void CFrameRate::Draw()
{
	unsigned int color = GetColor(255, 30, 30);
	DrawFormatString((int)(WINDOW_WIDTH - 100), (int)(WINDOW_HEIGHT - 20), color, "FPS[%.2f]", fps);
}

