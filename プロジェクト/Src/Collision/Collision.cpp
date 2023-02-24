
#include "../Common.h"
#include "Collision.h"
#include "../MyMath/MyMath.h"

//2D�~�̓����蔻��
bool CCollision::IsHitCircle(float x1, float y1, float r1, float x2, float y2, float r2)
{
	//�Q�̔��a�̍��v
	float sum_radius = r1 + r2;
	sum_radius *= sum_radius;

	//�~���m�̒���
	float x_len = x1 - x2;
	x_len *= x_len;

	float y_len = y1 - y2;
	y_len *= y_len;

	//�~�̓����蔻��
	if(sum_radius > x_len + y_len)
	{
		return true;
	}

	return false;
}

//�����m�̓����蔻��
bool CCollision::IsHitBall(VECTOR pos1, float radius1, VECTOR pos2, float radius2)
{
	//�Q�̔��a�̍��v
	float sum_radius = radius1 + radius2;

	//�~���m�̒���
	float distance;
	VECTOR vec_len;

	vec_len.x = pos1.x - pos2.x;
	vec_len.y = pos1.y - pos2.y;
	vec_len.z = pos1.z - pos2.z;
	distance = CMyMath::VecLong(vec_len);

	//�~�̓����蔻��
	if (sum_radius > distance)
	{
		return true;
	}
	
	return false;
}

//2D��`�̓����蔻��
bool CCollision::IsHitRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	if( x1 - w1 < x2 + w2 && x1 + w1 > x2 - w2&&
		y1 + h1 > y2 - h2 && y1 - h1 < y2 + h2 )
	{
		return true;
	}

	return false;
}

//2D��`�̓����蔻��
bool CCollision::IsHitRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	if (x1 - w1 < x2 + w2 && x1 + w1 > x2 - w2 &&
		y1 + h1 > y2 - h2 && y1 - h1 < y2 + h2)
	{
		return true;
	}

	return false;
}

//3D��`�̓����蔻��
bool CCollision::IsHitBox(VECTOR pos1, VECTOR size1, VECTOR pos2, VECTOR size2)
{
	VECTOR collision1 = VScale(size1, 0.5f);
	VECTOR collision2 = VScale(size2, 0.5f);
	
	bool is_hit_x = pos1.x - collision1.x < pos2.x + collision2.x && 
		pos1.x + collision1.x > pos2.x - collision2.x;
	bool is_hit_y = pos1.y + collision1.y > pos2.y - collision2.y &&
		pos1.y - collision1.y < pos2.y + collision2.y;
	bool is_hit_z = pos1.z + collision1.z > pos2.z - collision2.z &&
		pos1.z - collision1.z < pos2.z + collision2.z;
	
	//���ׂĂ̎��œ������Ă�����true
	if (is_hit_x && is_hit_y && is_hit_z)
	{
		return true;
	}

	return false;
}


//2D���̂����蔻��
bool CCollision::IsHitLine2D(float start1_x, float start1_y, float end1_x, float end1_y, float start2_x, float start2_y, float end2_x, float end2_y)
{
	float t1 = (start1_x - end1_x) * (start2_y - start1_y) + (start1_y - end1_y) * (start1_x - start2_x);
	float t2 = (start1_x - end1_x) * (end2_y - start1_y) + (start1_y - end1_y) * (start1_x - end2_x);

	if ((t1 * t2) < 0) {
		float t3 = (start2_x - end2_x) * (start1_y - start2_y) + (start2_y - end2_y) * (start2_x - start1_x);
		float t4 = (start2_x - end2_x) * (end1_y - start2_y) + (start2_y - end2_y) * (start2_x - end1_x);
		if ((t3 * t4) < 0) {
			return true;
		}
	}

	return false;
}

//2D���̂����蔻��
bool CCollision::IsHitLine2D(VECTOR pos1, VECTOR vec1, VECTOR pos2, VECTOR vec2)
{
	float t1 = (pos1.x - (pos1.x + vec1.x)) * (pos2.y - pos1.y) + (pos1.y - (pos1.y + vec1.y)) * (pos1.x - pos2.x);
	float t2 = (pos1.x - (pos1.x + vec1.x)) * ((pos2.y + vec2.y) - pos1.y) + (pos1.y - (pos1.y + vec1.y)) * (pos1.x - (pos2.x + vec2.x));
	if ((t1 * t2) < 0) {
		float t3 = (pos2.x - (pos2.x + vec2.x)) * (pos1.y - pos2.y) + (pos2.y - (pos2.y + vec2.y)) * (pos2.x - pos1.x);
		float t4 = (pos2.x - (pos2.x + vec2.x)) * ((pos1.y + vec1.y) - pos2.y) + (pos2.y - (pos2.y + vec2.y)) * (pos2.x - (pos1.x + vec1.x));
		if ((t3 * t4) < 0) {
			return true;
		}
	}
	return false;
}

//2D���Ƌ�`�̂����蔻��
bool CCollision::IsLineHitRect(float x1, float y1, float x2, float y2, float left, float right, float top, float bottom)
{
	bool IsHit = false;

	if (IsHitLine2D(x1, y1, x2, y2, left, top, right, top))
		IsHit = true;

	if (IsHitLine2D(x1, y1, x2, y2, right, top, right, bottom))
		IsHit = true;

	if (IsHitLine2D(x1, y1, x2, y2, right, bottom, left, bottom))
		IsHit = true;

	if (IsHitLine2D(x1, y1, x2, y2, left, bottom, left, top))
		IsHit = true;

	if (IsHit)
		return true;

	return false;
}

//2D���Ƌ�`�̂����蔻��
bool CCollision::IsLineHitRect(VECTOR pos, VECTOR vec, float left, float right, float top, float bottom)
{
	bool IsHit = false;

	if (IsHitLine2D(pos.x, pos.y, (pos.x + vec.x), (pos.y + vec.y), left, top, right, top))
		IsHit = true;

	if (IsHitLine2D(pos.x, pos.y, (pos.x + vec.x), (pos.y + vec.y), right, top, right, bottom))
		IsHit = true;

	if (IsHitLine2D(pos.x, pos.y, (pos.x + vec.x), (pos.y + vec.y), right, bottom, left, bottom))
		IsHit = true;

	if (IsHitLine2D(pos.x, pos.y, (pos.x + vec.x), (pos.y + vec.y), left, bottom, left, top))
		IsHit = true;

	if (IsHit)
		return true;

	return false;
}

//VECTOR line_start, VECTOR line_end, VECTOR circle_pos, float radius

bool CCollision::IsLineHitCircle(VECTOR line_start, VECTOR line_end, VECTOR circle_pos, float radius)
{
	VECTOR avec, bvec;

	//�x�N�g���̈����Z�@circle_pos.pos - line.startpos
	avec = CMyMath::VecSub(circle_pos, line_start);                         
	bvec = CMyMath::VecSub(line_end, line_start);

	//���ς����߂�
	float dot = CMyMath::VecDot(avec, bvec);		

	//�x�N�g��b�̒����̓������߂�
	float bl = CMyMath::VecLong(bvec) * CMyMath::VecLong(bvec);			
	if ((dot > -ZEROVALUE) && (dot < bl)) {
		//�~�Ɛ��̋��������߂�
		//�{�[���̒��S���琂���ɂ��낵�����̌�_�����߂�

		//�x�N�g��s�̐��K��
		VECTOR bnvec = CMyMath::VecNormalize(bvec);				
		float dot2 = CMyMath::VecDot(avec, bnvec);
		bnvec.x *= dot2;
		bnvec.y *= dot2;

		//�x�N�g���̑����Z
		VECTOR kouten = CMyMath::VecAdd(line_start, bnvec);                   
		VECTOR dist = CMyMath::VecSub(circle_pos, kouten);
		float dl = CMyMath::VecLong(dist) * CMyMath::VecLong(dist);;

		if (dl < (radius * radius)) 
		{
			return true;
		}
	}


	int ���˂��Ƃ�����		= 10000;
	int �ɂ��ނ�邢		= 10;
	int ���Ƃ��ꂢ		= 1;

	if (���˂��Ƃ����� > ���Ƃ��ꂢ + �ɂ��ނ�邢)
	{
		//���Ƃ��ꂢ����();
		//�ɂ��ނ�邢����();
	}
	else
	{
		//���݂��傷����();
	}

	return false;
}


//void ���Ƃ��ꂢ����()
//{
//
//}
//void �ɂ��ނ�邢����()
//{
//
//}
//
//void ���݂��傷����()
//{
//
//}