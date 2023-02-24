#include "Beam.h"
#include "BeamManager.h"

#include "../Common.h"

#include "../MyMath/MyMath.h"
#include "../Effekseer.h"



void CBeam::Init()
{
	m_pos = m_rot = m_startPos = m_oldPos = m_move = ZERO_VECTOR;
	memset(&m_detailInfo, 0, sizeof(BeamDetailInfo));
	m_requestIndex = -1;
	m_isUse = false;
}
void CBeam::Step()
{
	if (!m_isUse) { return; }


	//	ˆÚ“®—Ê•ª”ò‚Î‚·
	VECTOR move = m_move;

	move = CMyMath::VecNormalize(move);
	move = CMyMath::VecScale(move, m_detailInfo.speed);

	m_pos = CMyMath::VecAdd(m_pos, move);


	//Å‘å‹——£‚Æ‚ñ‚¾‚çisUse‚ðfalse‚É
	float lenge = CMyMath::VecLong(m_pos, m_startPos);
	if (lenge >= m_detailInfo.maxLange)
	{
		m_isUse = false;
	}

}
void CBeam::Update()
{
	if (!m_isUse) { return; }


#ifdef DEBUG_MODE

	DrawSphere3D(m_pos, m_detailInfo.radius, 8, GetColor(255, 255, 255), GetColor(0, 0, 0), false);
#endif
}
void CBeam::Fin()
{

}

void CBeam::Request(BeamRequestInfo& request_info, BeamDetailInfo& detail_info, int request_index)
{
	m_detailInfo = detail_info;
	m_pos = m_startPos = m_oldPos = request_info.startPos;
	m_rot = request_info.rot;
	m_move = request_info.move;
	m_requestIndex = request_index;
	m_beamKind = detail_info.beamKind;

	m_isUse = true;
}