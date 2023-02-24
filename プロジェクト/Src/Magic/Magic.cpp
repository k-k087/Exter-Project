#include "Magic.h"
#include "MagicManager.h"

#include "../Common.h"

#include "../MyMath/MyMath.h"
#include "../Effekseer.h"



void CMagic::Init()
{
	m_pos = m_rot = m_startPos = m_oldPos = m_move = ZERO_VECTOR;
	m_effectHandleID = -1;
	m_isUse = false;
}
void CMagic::Step()
{
	if (!m_isUse) { return; }


	//	移動量分飛ばす
	VECTOR move = m_move;

	move = CMyMath::VecNormalize(move);
	move = CMyMath::VecScale(move, m_detailInfo.speed);

	m_pos = CMyMath::VecAdd(m_pos, move);


	//最大距離とんだらisUseをfalseに
	float lenge = CMyMath::VecLong(m_pos, m_startPos);
	if (lenge >= m_detailInfo.maxLange)
	{
		CEffekseerCtrl::Stop(m_effectHandleID);
		m_isUse = false;
	}

}
void CMagic::Update()
{
	if (!m_isUse) { return; }


	CEffekseerCtrl::SetPosition(m_effectHandleID, m_pos);

#ifdef DEBUG_MODE

	DrawSphere3D(m_pos, m_detailInfo.radius, 8, GetColor(255, 255, 255), GetColor(0, 0, 0), false);
#endif
}
void CMagic::Fin()
{

}

void CMagic::Request(MagicRequestInfo& request_info, MagicDetailInfo& detail_info, int handle_id)
{
	m_detailInfo = detail_info;
	m_pos = m_startPos = m_oldPos = request_info.startPos;
	m_move = request_info.move;
	m_rot = request_info.rot;

	m_isUse = true;

	//エフェクト要請
	m_effectHandleID = CEffekseerCtrl::Request(handle_id, m_pos, true);
	CEffekseerCtrl::SetRot(m_effectHandleID, m_rot);
	CEffekseerCtrl::SetScale(m_effectHandleID, m_detailInfo.scale);
}