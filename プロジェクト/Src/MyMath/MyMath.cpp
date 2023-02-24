#include "DxLib.h"

#include <math.h>

#include "MyMath.h"

// 点P(x1, y1)から点Q(x2, y2)までの距離を返す
float CMyMath::GetDistance(float x1, float y1, float x2, float y2)
{
	float a = x2 - x1;

	float b = y2 - y1;

	float double_c = a * a + b * b;

	return sqrtf(double_c);
}
float CMyMath::GetDistance(VECTOR vec1, VECTOR vec2)
{
	VECTOR  a = { vec1.x - vec2.x,
	vec1.y - vec2.y,vec1.z - vec2.z };

	return VecLong(a);
}

// 回転値を0～2πの値にする
float CMyMath::RadianNormalize(float rad)
{
	float result = rad;
	if (result < 0.0f) {
		while (result < 0.0f) {
			result += DX_PI_F;
		}
	}
	else if (result > DX_TWO_PI_F) {
		while (result <= DX_TWO_PI_F) {
			result -= DX_TWO_PI_F;
		}
	}

	return result;
}

// 始点と終点からベクトルを作成
VECTOR CMyMath::VecCreate(VECTOR start, VECTOR end)
{
	VECTOR result;

	//ベクトルは「終点 - 始点」で求められる
	result.x = end.x - start.x;
	result.y = end.y - start.y;
	result.z = end.z - start.z;

	return result;
}

// ベクトルの足し算
VECTOR CMyMath::VecAdd(VECTOR vec1, VECTOR vec2)
{
	VECTOR result;

	//各成分を足し算する
	result.x = vec1.x + vec2.x;
	result.y = vec1.y + vec2.y;
	result.z = vec1.z + vec2.z;

	return result;
}

// ベクトルの引算
VECTOR CMyMath::VecSub(VECTOR vec1, VECTOR vec2)
{
	VECTOR result;

	result.x = vec1.x - vec2.x;
	result.y = vec1.y - vec2.y;
	result.z = vec1.z - vec2.z;

	return result;
}

// ベクトルの長さ
float CMyMath::VecLong(VECTOR vec)
{
	if (vec.x == 0 &&
		vec.y == 0 &&
		vec.z == 0)
	{
		return 1.f;
	}

	//ピタゴラスの定理からベクトルの長さを求める
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
// ベクトルの長さ
float CMyMath::VecLong(VECTOR vec1, VECTOR vec2)
{
	VECTOR len = VecCreate(vec1, vec2);

	if (len.x == 0 &&
		len.y == 0 &&
		len.z == 0)
	{
		return 1.f;
	}

	return VecLong(len);
}


// ベクトルの正規化
VECTOR CMyMath::VecNormalize(VECTOR vec)
{
	VECTOR result;

	//ベクトルの長さを求める
	float vec_long = VecLong(vec);

	//各成分を長さで割ることにより正規化する
	result.x = vec.x / vec_long;
	result.y = vec.y / vec_long;
	result.z = vec.z / vec_long;

	return result;
}

// ベクトルのスカラー倍
VECTOR CMyMath::VecScale(VECTOR vec, float scale)
{
	VECTOR result;

	//各成分をスカラー倍する
	result.x = vec.x * scale;
	result.y = vec.y * scale;
	result.z = vec.z * scale;

	return result;
}

// ベクトルの内積
float CMyMath::VecDot(VECTOR vec1, VECTOR vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

// ベクトルの向きをラジアン値で取得
float CMyMath::VecDir(VECTOR vec)
{
	return atan2f(vec.x, vec.y);
}
float CMyMath::VecDir(float x, float y)
{
	return atan2f(x, y);
}

//単位行列を取得
MATRIX CMyMath::GetIdentityMatrix()
{
	MATRIX result =
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f,
	};

	return result;
}

//平行移動行列を取得する
MATRIX CMyMath::GetTranslateMatrix(VECTOR move)
{
	MATRIX result =
	{
		1.f, 0.f, 0.f, move.x,
		0.f, 1.f, 0.f, move.y,
		0.f, 0.f, 1.f, move.z,
		0.f, 0.f, 0.f,    1.f,
	};

	return result;
}
MATRIX CMyMath::GetTranslateMatrix(float move_x, float move_y, float move_z)
{
	VECTOR result = { move_x, move_y, move_z };

	return GetTranslateMatrix(result);
}

//拡縮行列を取得する
MATRIX CMyMath::GetScaleMatrix(VECTOR scale)
{
	MATRIX result =
	{
		scale.x,   0.f,     0.f,   0.f,
		  0.f,   scale.y,   0.f,   0.f,
		  0.f,     0.f,   scale.z, 0.f,
		  0.f,     0.f,     0.f,   1.f,
	};

	return result;
}
MATRIX CMyMath::GetScaleMatrix(float scale_x, float scale_y, float scale_z)
{
	VECTOR result = { scale_x, scale_y, scale_z };

	return GetScaleMatrix(result);
}

//X軸回転行列を取得する(ラジアン)
MATRIX CMyMath::GetPitchMatrix(float radian)
{
	MATRIX result =
	{
		1.f,     0.f,            0.f,     0.f,
		0.f, cosf(radian), -sinf(radian), 0.f,
		0.f, sinf(radian),  cosf(radian), 0.f,
		0.f,     0.f,            0.f,     1.f,
	};

	return result;
}

//Y軸回転行列を取得する(ラジアン)
MATRIX CMyMath::GetYawMatrix(float radian)
{
	MATRIX result =
	{
		cosf(radian),  0.f, sinf(radian), 0.f,
			0.f,       1.f,     0.f,      0.f,
		-sinf(radian), 0.f, cosf(radian), 0.f,
			0.f,       0.f,     0.f,      1.f,
	};

	return result;
}

//Z軸回転行列を取得する(ラジアン)
MATRIX CMyMath::GetRollMatrix(float radian)
{
	MATRIX result =
	{
		cosf(radian), -sinf(radian), 0.f, 0.f,
		sinf(radian),  cosf(radian), 0.f, 0.f,
		    0.f,           0.f,      1.f, 0.f,
			0.f,           0.f,      0.f, 1.f,
	};

	return result;
}

////X,Y,Zすべての回転行列計算
//MATRIX CMyMath::GetAllRotMatrix(VECTOR radian)
//{
//
//}

//２つの行列の足し算
MATRIX CMyMath::MatAdd(MATRIX mat1, MATRIX mat2)
{
	MATRIX result = { 0.f };

	for (int index1 = 0; index1 < 4; index1++)
	{
		for (int index2 = 0; index2 < 4; index2++)
		{
			result.m[index1][index2] = mat1.m[index1][index2] + mat2.m[index1][index2];
		}
	}

	return result;
}

//２つの行列の引き算
MATRIX CMyMath::MatSub(MATRIX mat1, MATRIX mat2)
{
	MATRIX result = { 0.f };

	for (int index1 = 0; index1 < 4; index1++)
	{
		for (int index2 = 0; index2 < 4; index2++)
		{
			result.m[index1][index2] = mat1.m[index1][index2] - mat2.m[index1][index2];
		}
	}

	return result;
}

//行列のスカラー倍
MATRIX CMyMath::MatScale(MATRIX mat, float scale)
{
	MATRIX result = { 0.f };

	for (int index1 = 0; index1 < 4; index1++)
	{
		for (int index2 = 0; index2 < 4; index2++)
		{
			result.m[index1][index2] = mat.m[index1][index2] * scale;
		}
	}

	return result;
}

//２つの行列の掛け算
MATRIX CMyMath::MatMult(MATRIX mat1, MATRIX mat2)
{
	MATRIX result = { 0.f };

	for (int index1 = 0; index1 < 4; index1++)
	{
		for (int index2 = 0; index2 < 4; index2++)
		{
			for (int index3 = 0; index3 < 4; index3++)
			{
				result.m[index1][index2] += (mat1.m[index1][index3] * mat2.m[index3][index2]);
			}
		}
	}

	return result;
}

//行列とベクトルの積
VECTOR CMyMath::MatTransform(MATRIX mat, VECTOR vec)
{
	float vec_f[4] = { vec.x, vec.y, vec.z, 1.f };
	float res_f[4] = { 0.f };

	for (int index1 = 0; index1 < 4; index1++)
	{
		for (int index2 = 0; index2 < 4; index2++)
		{
			res_f[index1] += (mat.m[index1][index2] * vec_f[index2]);
		}
	}

	//VECTORに変更
	VECTOR result = { res_f[0], res_f[1], res_f[2] };

	return result;
}

//行列の転置
MATRIX CMyMath::MatTranspose(MATRIX mat)
{
	MATRIX result = { 0.f };

	for (int index1 = 0; index1 < 4; ++index1)
	{
		for (int index2 = 0; index2 < 4; ++index2)
		{
			result.m[index1][index2] = mat.m[index2][index1];
		}
	}

	return result;
}

//引数で渡された行列をその数分順番に掛け算する
MATRIX CMyMath::MatMultAll(MATRIX* mat, int mat_num)
{
	MATRIX result = { 0.f };

	for (int mat_index = 0; mat_index < mat_num - 1; ++mat_index, ++mat)
	{
		MATRIX* next_mat = mat + 1;

		if (mat_index == 0)
		{
			result = MatMult(*mat, *next_mat);
		}
		else
		{
			result = MatMult(result, *next_mat);
		}
	}

	return result;
}
	
float CMyMath::Abs(float val)
{
	return val = val < 0 ? -val : val;
}
int	 CMyMath::Abs(int val)
{
	return val = val < 0 ? -val : val;
}