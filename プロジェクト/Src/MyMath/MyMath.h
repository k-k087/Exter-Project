#ifndef __MYMATH_H__
#define __MYMATH_H__

#include "DxLib.h"

class CMyMath
{
public:

	// �_P(x1, y1)����_Q(x2, y2)�܂ł̋�����Ԃ�
	static float GetDistance(float x1, float y1, float x2, float y2);
	static float GetDistance(VECTOR vec1, VECTOR vec2);

	// ��]�l��0�`2�΂̒l�ɂ���
	static float RadianNormalize(float rad);

	//�����x�N�g����

	// �n�_�ƏI�_����x�N�g�����쐬
	static VECTOR VecCreate(VECTOR start, VECTOR end);

	// �x�N�g���̑����Z
	static VECTOR VecAdd(VECTOR vec1, VECTOR vec2);

	// �x�N�g���̈��Z
	static VECTOR VecSub(VECTOR vec1, VECTOR vec2);

	// �x�N�g���̒���
	static float VecLong(VECTOR vec);
	static float VecLong(VECTOR vec1, VECTOR vec2);

	// �x�N�g���̐��K��
	static VECTOR VecNormalize(VECTOR vec);

	// �x�N�g���̃X�J���[�{
	static VECTOR VecScale(VECTOR vec, float scale);

	// �x�N�g���̓���
	static float VecDot(VECTOR vec1, VECTOR vec2);

	// �x�N�g���̌��������W�A���l�Ŏ擾
	static float VecDir(VECTOR vec);
	static float VecDir(float x, float y);

	//�P�ʍs����擾
	static MATRIX GetIdentityMatrix();

	//���s�ړ��s����擾����
	static MATRIX GetTranslateMatrix(VECTOR move);
	static MATRIX GetTranslateMatrix(float move_x, float move_y, float move_z);

	//�g�k�s����擾����
	static MATRIX GetScaleMatrix(VECTOR scale);
	static MATRIX GetScaleMatrix(float scale_x, float scale_y, float scale_z);

	//X����]�s����擾����(���W�A��)
	static MATRIX GetPitchMatrix(float radian);

	//Y����]�s����擾����(���W�A��)
	static MATRIX GetYawMatrix(float radian);

	//Z����]�s����擾����(���W�A��)
	static MATRIX GetRollMatrix(float radian);

	//X,Y,Z���ׂẲ�]�s��v�Z
	//static MATRIX GetAllRotMatrix(VECTOR radian);

	//�Q�̍s��̑����Z
	static MATRIX MatAdd(MATRIX mat1, MATRIX mat2);

	//�Q�̍s��̈����Z
	static MATRIX MatSub(MATRIX mat1, MATRIX mat2);

	//�s��̃X�J���[�{
	static MATRIX MatScale(MATRIX mat, float scale);

	//�Q�̍s��̊|���Z
	static MATRIX MatMult(MATRIX mat1, MATRIX mat2);

	//�s��ƃx�N�g���̐�
	static VECTOR MatTransform(MATRIX mat, VECTOR vec);

	//�s��̓]�u
	static MATRIX MatTranspose(MATRIX mat);

	//�����œn���ꂽ�s������̐������ԂɊ|���Z����
	static MATRIX MatMultAll(MATRIX* mat, int mat_num);

	static float Abs(float val);
	static int	 Abs(int val);
};

#endif
