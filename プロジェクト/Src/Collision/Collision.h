#ifndef __COLLISION_H__
#define __COLLISION_H__

//コリジョンクラス
class CCollision
{
public:
	// 円の当たり判定
	static bool IsHitCircle(float x1, float y1, float r1, float x2, float y2, float r2);
	static bool IsHitBall(VECTOR pos1, float radius1, VECTOR pos2, float radius2);

	// 矩形の当たり判定
	static bool IsHitBox(VECTOR pos1, VECTOR size1, VECTOR pos2, VECTOR size2);
	static bool IsHitRect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
	static bool IsHitRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

	//線のあたり判定
	static bool IsHitLine2D(float start1_x, float start1_y, float end1_x, float end1_y, float start2_x, float start2_y, float end2_x, float end2_y);
	static bool IsHitLine2D(VECTOR pos1, VECTOR vec1, VECTOR pos2, VECTOR vec2);

	//線と矩形のあたり判定
	static bool IsLineHitRect(float x1, float y1, float x2, float y2, float left, float right, float top, float bottom);
	static bool IsLineHitRect(VECTOR pos, VECTOR vec, float left, float right, float top, float bottom);

	//線と円の当たり判定
	static bool IsLineHitCircle(VECTOR line_start, VECTOR line_end, VECTOR circle_pos, float radius);

};

#endif
