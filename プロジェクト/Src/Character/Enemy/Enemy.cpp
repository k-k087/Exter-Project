#include "DxLib.h"

#include "Enemy.h"

#include "../Player/PlayerManager.h"
#include "../../MyMath/MyMath.h"
#include "../../Common.h"

#include "../../Effekseer/Effekseer.h"
#include "../Enemy/EnemyManager.h"

//プレイヤーの方を向く処理
void CEnemy::FacingPlayer()
{
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);
	m_rot.y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
}
void CEnemy::FacingSlowPlayer()
{
	//VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	//VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);
	//
	//float rot_y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
	//
	//*VECTOR enemy_rot = m_rot;
	//while (enemy_rot.y > DX_TWO_PI_F)
	//{
	//
	//}*/
	//
	//if (rot_y > m_rot.y)
	//{
	//	m_rot.y += ONE_DEGREE;
	//	
	//	if (m_rot.y > rot_y)
	//	{
	//		m_rot.y = rot_y;
	//	}
	//}
	//if (rot_y < m_rot.y)
	//{
	//	m_rot.y -= ONE_DEGREE;
	//
	//	if (m_rot.y < rot_y)
	//	{
	//		m_rot.y = rot_y;
	//	}
	//}

	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);

	float rot_y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
	float enemy_rot_y = m_rot.y;

	while (enemy_rot_y > DX_TWO_PI_F)
	{
		enemy_rot_y -= DX_TWO_PI_F;
	}
	while (rot_y > DX_TWO_PI_F)
	{
		rot_y -= DX_TWO_PI_F;
	}
	while (enemy_rot_y <= 0)
	{
		enemy_rot_y += DX_TWO_PI_F;
	}
	while (rot_y <= 0)
	{
		rot_y += DX_TWO_PI_F;
	}

	//どっちが早いかの判定
	int count1 = 0; int count2 = 0;

	float y = enemy_rot_y;

	while (!CCommon::IsNearF(y, rot_y, ONE_DEGREE))
	{
		y += ONE_DEGREE;
		if (y > DX_TWO_PI_F)
		{
			y = y - DX_TWO_PI_F;
		}
		count1++;
	}

	y = enemy_rot_y;
	while (!CCommon::IsNearF(y, rot_y, ONE_DEGREE))
	{
		y -= ONE_DEGREE;
		if (y <= 0)
		{
			y = DX_TWO_PI_F;
		}
		count2++;
	}

	if (count2 > count1)
	{
		m_rot.y += ONE_DEGREE;

		if (y > DX_TWO_PI_F)
		{
			y = y - DX_TWO_PI_F;
		}

		if (m_rot.y > rot_y)
		{
			m_rot.y = rot_y;
		}
	}
	else
	{
		m_rot.y -= ONE_DEGREE;
		if (y <= 0)
		{
			y = DX_TWO_PI_F;
		}

		if (m_rot.y < rot_y)
		{
			m_rot.y = rot_y;
		}
	}
}

void CEnemy::FacingSlowPlayer(float r)
{
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);

	float rot_y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
	float enemy_rot_y = m_rot.y;

	while (enemy_rot_y > DX_TWO_PI_F)
	{
		enemy_rot_y -= DX_TWO_PI_F;
	}
	while (rot_y > DX_TWO_PI_F)
	{
		rot_y -= DX_TWO_PI_F;
	}
	while (enemy_rot_y <= 0)
	{
		enemy_rot_y += DX_TWO_PI_F;
	}
	while (rot_y <= 0)
	{
		rot_y += DX_TWO_PI_F;
	}

	//どっちが早いかの判定
	int count1 = 0; int count2 = 0;

	float y = enemy_rot_y;

	while (!CCommon::IsNearF(y, rot_y, ONE_DEGREE))
	{
		y += ONE_DEGREE;
		if (y > DX_TWO_PI_F)
		{
			y = y - DX_TWO_PI_F;
		}
		count1++;
	}

	y = enemy_rot_y;
	while (!CCommon::IsNearF(y, rot_y, ONE_DEGREE))
	{
		y -= ONE_DEGREE;
		if (y <= 0)
		{
			y = DX_TWO_PI_F;
		}
		count2++;
	}

	if (count2 > count1)
	{
		m_rot.y += ONE_DEGREE * r;

		if (y > DX_TWO_PI_F)
		{
			y = y - DX_TWO_PI_F;
		}

		if (m_rot.y > rot_y)
		{
			m_rot.y = rot_y;
		}
	}
	else
	{
		m_rot.y -= ONE_DEGREE * r;
		if (y <= 0)
		{
			y = DX_TWO_PI_F;
		}

		if (m_rot.y < rot_y)
		{
			m_rot.y = rot_y;
		}
	}
}

bool CEnemy::IsFacingPlayer()
{
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);

	float rot_y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;

	if (m_rot.y >= rot_y - ONE_DEGREE * 5.f &&
		rot_y + ONE_DEGREE * 5.f >= m_rot.y)
	{
		return true;
	}

	return false;
}

void CEnemy::NormalizeRot()
{
	while(m_rot.y < 0)
	{
		m_rot.y += DX_TWO_PI_F;
	}

	while (m_rot.y >= DX_TWO_PI_F)
	{
		m_rot.y -= DX_TWO_PI_F;
	}
}


bool CEnemy::AppearStep()
{
	if (m_appearCount == 0) { return false; }

	m_appearCount -= FRAME_TIME;

	if (!m_isAppear && m_appearCount <= 200)
		m_isAppear = true;

	if (m_appearCount <= 0)
		m_appearCount = 0;

	return true;
}

//攻撃判定時間(ミリ秒)
void CEnemy::LetsAttackColl(int time, float add, float radius)
{
 	m_isAttack = true;
	m_attackCollCount = time;

	//攻撃データをうまく使う
	m_attackData.isUse = true;
	m_attackData.attackAdd = add;
	m_attackData.attackRadius = radius;
}

void CEnemy::StepAttackColl()
{
	if (m_attackCollCount != 0)
	{
		m_attackCollCount -= FRAME_TIME;

		if (m_attackCollCount <= 0)
		{
			m_attackCollCount = 0;
			m_isAttack = false;
			memset(&m_attackData, 0, sizeof(m_attackData));
		}
	}
	else
	{
		m_isAttack = false;
	}
}

void CEnemy::SetBarrier(VECTOR scale)
{
	if (m_barrierEffectID == -1)
	{
		m_barrierEffectID = CEnemyManager::GetManager()->
			RequestEffect(ENEMY_EFFECT_KIND_BARIIER, GetCenterPos(), true);
		CEffekseerCtrl::SetScale(m_barrierEffectID, scale);
		m_barrierHP = m_barrierMaxHP = 100;
	}
}