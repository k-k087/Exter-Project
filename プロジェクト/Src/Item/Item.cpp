#include "Item.h"
#include "ItemManager.h"

#include "../Common.h"

#include "../MyMath/MyMath.h"


void CItem::Init()
{
	m_pos = m_rot = m_startPos = m_oldPos = m_move = ZERO_VECTOR;
	m_isUse = false;
}
void CItem::Load()
{

}
void CItem::Load(int handle)
{
	m_startPos = m_pos =
	m_move = ZERO_VECTOR;

	//モデルロード
	if (m_handle == -1)
	{
		m_handle = MV1DuplicateModel(handle);
	}

	//大きさセット
	//MV1SetScale(m_handle, NORMAL_ENEMY_SCALE);

	m_isUse = false;
}
void CItem::Step()
{
	if (!m_isUse) { return; }


	//	移動量分飛ばす
	VECTOR move = m_move;

	m_pos = CMyMath::VecAdd(m_pos, move);

	//	ｙは0以下にならない
	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	//	カウント減算
	m_useCount -= FRAME_TIME;
	if (m_useCount <= 0)
	{
		m_isUse = false;
	}

}
void CItem::Update()
{
	if (!m_isUse) { return; }

	//回転値反映
	MV1SetRotationXYZ(m_handle, m_rot);

	//座標セット
	MV1SetPosition(m_handle, m_pos);

#ifdef DEBUG_MODE

	DrawSphere3D(m_pos, m_radius, 8, GetColor(255, 255, 255), GetColor(0, 0, 0), false);
#endif
}
void CItem::Draw()
{
	//最後に描画
	MV1DrawModel(m_handle);
}
void CItem::Fin()
{

}

void CItem::Request(ItemRequestData request_data)
{
	m_isUse = true;

	m_useCount = request_data.useCount;
	m_move = request_data.move;
	m_startPos = m_pos = m_oldPos = request_data.startPos;
	m_speed = request_data.speed;
}

void CItem::HitFunc()
{

}