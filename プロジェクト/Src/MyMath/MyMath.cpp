#include "DxLib.h"

#include <math.h>

#include "MyMath.h"

// �_P(x1, y1)����_Q(x2, y2)�܂ł̋�����Ԃ�
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

// ��]�l��0�`2�΂̒l�ɂ���
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

// �n�_�ƏI�_����x�N�g�����쐬
VECTOR CMyMath::VecCreate(VECTOR start, VECTOR end)
{
	VECTOR result;

	//�x�N�g���́u�I�_ - �n�_�v�ŋ��߂���
	result.x = end.x - start.x;
	result.y = end.y - start.y;
	result.z = end.z - start.z;

	return result;
}

// �x�N�g���̑����Z
VECTOR CMyMath::VecAdd(VECTOR vec1, VECTOR vec2)
{
	VECTOR result;

	//�e�����𑫂��Z����
	result.x = vec1.x + vec2.x;
	result.y = vec1.y + vec2.y;
	result.z = vec1.z + vec2.z;

	return result;
}

// �x�N�g���̈��Z
VECTOR CMyMath::VecSub(VECTOR vec1, VECTOR vec2)
{
	VECTOR result;

	result.x = vec1.x - vec2.x;
	result.y = vec1.y - vec2.y;
	result.z = vec1.z - vec2.z;

	return result;
}

// �x�N�g���̒���
float CMyMath::VecLong(VECTOR vec)
{
	if (vec.x == 0 &&
		vec.y == 0 &&
		vec.z == 0)
	{
		return 1.f;
	}

	//�s�^�S���X�̒藝����x�N�g���̒��������߂�
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
// �x�N�g���̒���
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


// �x�N�g���̐��K��
VECTOR CMyMath::VecNormalize(VECTOR vec)
{
	VECTOR result;

	//�x�N�g���̒��������߂�
	float vec_long = VecLong(vec);

	//�e�����𒷂��Ŋ��邱�Ƃɂ�萳�K������
	result.x = vec.x / vec_long;
	result.y = vec.y / vec_long;
	result.z = vec.z / vec_long;

	return result;
}

// �x�N�g���̃X�J���[�{
VECTOR CMyMath::VecScale(VECTOR vec, float scale)
{
	VECTOR result;

	//�e�������X�J���[�{����
	result.x = vec.x * scale;
	result.y = vec.y * scale;
	result.z = vec.z * scale;

	return result;
}

// �x�N�g���̓���
float CMyMath::VecDot(VECTOR vec1, VECTOR vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

// �x�N�g���̌��������W�A���l�Ŏ擾
float CMyMath::VecDir(VECTOR vec)
{
	return atan2f(vec.x, vec.y);
}
float CMyMath::VecDir(float x, float y)
{
	return atan2f(x, y);
}

//�P�ʍs����擾
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

//���s�ړ��s����擾����
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

//�g�k�s����擾����
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

//X����]�s����擾����(���W�A��)
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

//Y����]�s����擾����(���W�A��)
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

//Z����]�s����擾����(���W�A��)
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

////X,Y,Z���ׂẲ�]�s��v�Z
//MATRIX CMyMath::GetAllRotMatrix(VECTOR radian)
//{
//
//}

//�Q�̍s��̑����Z
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

//�Q�̍s��̈����Z
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

//�s��̃X�J���[�{
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

//�Q�̍s��̊|���Z
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

//�s��ƃx�N�g���̐�
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

	//VECTOR�ɕύX
	VECTOR result = { res_f[0], res_f[1], res_f[2] };

	return result;
}

//�s��̓]�u
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

//�����œn���ꂽ�s������̐������ԂɊ|���Z����
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