#ifndef __MYMATH_H__
#define __MYMATH_H__

#include "DxLib.h"

class CMyMath
{
public:

	// 点P(x1, y1)から点Q(x2, y2)までの距離を返す
	static float GetDistance(float x1, float y1, float x2, float y2);
	static float GetDistance(VECTOR vec1, VECTOR vec2);

	// 回転値を0～2πの値にする
	static float RadianNormalize(float rad);

	//向きベクトルを

	// 始点と終点からベクトルを作成
	static VECTOR VecCreate(VECTOR start, VECTOR end);

	// ベクトルの足し算
	static VECTOR VecAdd(VECTOR vec1, VECTOR vec2);

	// ベクトルの引算
	static VECTOR VecSub(VECTOR vec1, VECTOR vec2);

	// ベクトルの長さ
	static float VecLong(VECTOR vec);
	static float VecLong(VECTOR vec1, VECTOR vec2);

	// ベクトルの正規化
	static VECTOR VecNormalize(VECTOR vec);

	// ベクトルのスカラー倍
	static VECTOR VecScale(VECTOR vec, float scale);

	// ベクトルの内積
	static float VecDot(VECTOR vec1, VECTOR vec2);

	// ベクトルの向きをラジアン値で取得
	static float VecDir(VECTOR vec);
	static float VecDir(float x, float y);

	//単位行列を取得
	static MATRIX GetIdentityMatrix();

	//平行移動行列を取得する
	static MATRIX GetTranslateMatrix(VECTOR move);
	static MATRIX GetTranslateMatrix(float move_x, float move_y, float move_z);

	//拡縮行列を取得する
	static MATRIX GetScaleMatrix(VECTOR scale);
	static MATRIX GetScaleMatrix(float scale_x, float scale_y, float scale_z);

	//X軸回転行列を取得する(ラジアン)
	static MATRIX GetPitchMatrix(float radian);

	//Y軸回転行列を取得する(ラジアン)
	static MATRIX GetYawMatrix(float radian);

	//Z軸回転行列を取得する(ラジアン)
	static MATRIX GetRollMatrix(float radian);

	//X,Y,Zすべての回転行列計算
	//static MATRIX GetAllRotMatrix(VECTOR radian);

	//２つの行列の足し算
	static MATRIX MatAdd(MATRIX mat1, MATRIX mat2);

	//２つの行列の引き算
	static MATRIX MatSub(MATRIX mat1, MATRIX mat2);

	//行列のスカラー倍
	static MATRIX MatScale(MATRIX mat, float scale);

	//２つの行列の掛け算
	static MATRIX MatMult(MATRIX mat1, MATRIX mat2);

	//行列とベクトルの積
	static VECTOR MatTransform(MATRIX mat, VECTOR vec);

	//行列の転置
	static MATRIX MatTranspose(MATRIX mat);

	//引数で渡された行列をその数分順番に掛け算する
	static MATRIX MatMultAll(MATRIX* mat, int mat_num);

	static float Abs(float val);
	static int	 Abs(int val);
};

#endif
