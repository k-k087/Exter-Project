#include "DxLib.h"

#include <math.h>
#include <cassert>

#include "Boss1.h"

#include "../../EnemyManager.h"

#include "../../../Player/PlayerManager.h"
#include "../../../Player/Player.h"

#include "../../../../Common.h"
#include "../../../../Field/Field.h"
#include "../../../../Input/InputManager.h"
#include "../../../../Debug/Debug.h"
#include "../../../../Camera/CameraManager.h"
#include "../../../../Draw/DrawManager.h"
#include "../../../../Scene/SceneManager.h"
#include "../../../../Sound/SoundManager.h"
#include "../../../../Anime/Anime.h"
#include "../../../../MyMath/MyMath.h"
#include "../../../../Magic/MagicManager.h"
#include "../../../../UI/UIManager.h"

#include "../../Effekseer/Effekseer.h"

#include "../../../../Beam/BeamManager.h"


static const CPlayAnimeData BOSS1_ANIME_DATA[BOSS1_ANIME_KIND_NUM] =
{
	{ BOSS1_ANIME_KIND_ATTACK,				 0.5f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_ATTACK_ALL,			 0.38f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_ATTACK_CHAIN,		 0.5f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_ATTACK_DASH,			 0.15f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_ATTACK_JUMP,			 0.55f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_ATTACK_SPECIAL,		 0.2f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_AVOID,				 0.5f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_DEFAULT,				 0.5f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_DOWN,				 0.5f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_HIT,					 0.5f,		-1, false, ANIME_TYPE_END	},
	{ BOSS1_ANIME_KIND_IDLE,				 0.5f,		-1, false, ANIME_TYPE_LOOP	},
	{ BOSS1_ANIME_KIND_WALK,				 0.5f,		-1, false, ANIME_TYPE_LOOP	},
};

//	半径
static const float BOSS1_RADIUS = 40.f;

//	移動量
static const float BOSS1_MOVE_SPEED = 0.3f;

//	モデルサイズ
static const VECTOR BOSS1_SIZE = { 14.f,46.f,14.f };
static const VECTOR BOSS1_SCALE = { 18.3f,16.5f,20.3f };

//	無敵時間
static const int BOSS1_INVINCIBLE_TIME = 200;

//	ミリ秒
static const int ATTACK_HIT_MAX_COUNT = 1500;
static const int ATTACK_HIT_RAND_COUNT = 1300;

static const int ATTACK_ALL_MAX_COUNT = 1500;
static const int ATTACK_ALL_RAND_COUNT = 1300;

static const int ATTACK_CHAIN_MAX_COUNT = 1500;
static const int ATTACK_CHAIN_RAND_COUNT = 1300;

static const int ATTACK_DASH_MAX_COUNT = 1500;
static const int ATTACK_DASH_RAND_COUNT = 1500;

static const int ATTACK_JUMP_MAX_COUNT = 4000;
static const int ATTACK_JUMP_RAND_COUNT = 2500;

static const int ATTACK_SPECIAL_MAX_COUNT = 2000;
static const int ATTACK_SPECIAL_RAND_COUNT = 2000;

static const int POAISION_RAND = 500;


static const int ATTACK_MAX[BOSS1_ATTACK_NUM] =
{
	ATTACK_HIT_MAX_COUNT, 
	ATTACK_ALL_MAX_COUNT, 
	ATTACK_CHAIN_MAX_COUNT,
	ATTACK_DASH_MAX_COUNT,
	ATTACK_JUMP_MAX_COUNT,
	ATTACK_SPECIAL_MAX_COUNT,
};
static const int ATTACK_RAND[BOSS1_ATTACK_NUM] =
{
	ATTACK_HIT_RAND_COUNT,
	ATTACK_ALL_RAND_COUNT,
	ATTACK_CHAIN_RAND_COUNT,
	ATTACK_DASH_RAND_COUNT,
	ATTACK_JUMP_RAND_COUNT,
	ATTACK_SPECIAL_RAND_COUNT,
};

//	HP
static const int BOSS1_HP = 750;

static const float PLAYER_FIND_RADIUS = 400.f;
static const float ATTACK_RANGE_RADIUS = 66.f;


static const float ATTACK_COLLISION_FRAME = 65.f;

static const float ATTACK_HIT_COLLISION_RADIUS = 60.f;
static const float ATTACK_HIT_COLLISION_ADD = 60.f;

static const float ATTACK_ALL_COLLISION_RADIUS = 95.f;
static const float ATTACK_ALL_COLLISION_ADD = 0.f;

static const float ATTACK_CHAIN_COLLISION_RADIUS = 50.f;
static const float ATTACK_CHAIN_COLLISION_ADD = 60.f;

static const float ATTACK_DASH_COLLISION_RADIUS = 50.f;
static const float ATTACK_DASH_COLLISION_ADD = 16.f;

static const float ATTACK_JUMP_COLLISION_RADIUS = 86.f;
static const float ATTACK_JUMP_COLLISION_ADD = 35.f;

static const float ATTACK_SPECIAL_COLLISION_RADIUS = 35.f;
static const float ATTACK_SPECIAL_COLLISION_ADD = 30.f;

static const float BARRIER_HP = 100.f;

static const int FACING_PLAYER_HIT_TIME = 47;

static const float ATTACK_COLLISION_RADIUS[BOSS1_ATTACK_NUM] = 
{
	ATTACK_HIT_COLLISION_RADIUS,
	ATTACK_ALL_COLLISION_RADIUS,
	ATTACK_CHAIN_COLLISION_RADIUS,
	ATTACK_DASH_COLLISION_RADIUS,
	ATTACK_JUMP_COLLISION_RADIUS,
	ATTACK_SPECIAL_COLLISION_RADIUS,
};
static const float ATTACK_COLLISION_ADD[BOSS1_ATTACK_NUM] = 
{
	ATTACK_HIT_COLLISION_ADD,
	ATTACK_ALL_COLLISION_ADD,
	ATTACK_CHAIN_COLLISION_ADD,
	ATTACK_DASH_COLLISION_ADD,
	ATTACK_JUMP_COLLISION_ADD,
	ATTACK_SPECIAL_COLLISION_ADD,
};


//初期化
void CBoss1::Init()
{
	m_size = BOSS1_SIZE;
	m_hp = m_maxHP = BOSS1_HP;
	m_radius = BOSS1_RADIUS;
	m_enemyHitRadius = BOSS1_RADIUS * 1.8f;
	m_horizonRadius = PLAYER_FIND_RADIUS;
	m_attackRange = ATTACK_RANGE_RADIUS;
	m_isAppear = false;
	m_appearCount = 0;

	for (int attack_index = 0; attack_index < BOSS1_ATTACK_NUM; ++attack_index)
	{
		m_attackMaxCount[attack_index] = ATTACK_MAX[attack_index] + GetRand(ATTACK_RAND[attack_index]);
	}
}
void CBoss1::Reset()
{
	m_appearCount = m_invincibleVal = 0;
	memset(&m_attackCount, 0, sizeof(m_attackCount));
	m_speed = 0.f;
	m_hp = m_maxHP = BOSS1_HP;
	m_isAlive = true;
	m_isAppear = m_isAttack = m_isTargeting = false;
	m_move = m_oldPos = m_pos = m_rot = m_startPos = ZERO_VECTOR;
	m_attackKind = BOSS1_ATTACK_NONE;
}

void CBoss1::Appear()
{
	Init();
	m_pos.x = (float)GetRand(300); if (GetRand(1) == 1) { m_pos.x *= -1.f; }
	m_pos.z = (float)GetRand(300); if (GetRand(1) == 1) { m_pos.z *= -1.f; }
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;
	m_isAlive = true; 

	CUIManager::GetManager()->GetUI()->SetBossHP(m_hp);


	//アニメ再生
	m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CBoss1::Appear(VECTOR start_pos, bool is_barrier)
{
	Init();
	
	m_pos = start_pos;
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;
	m_isAlive = true;

	if (is_barrier)
	{
		m_barrierEffectID = CEnemyManager::GetManager()->
			RequestEffect(ENEMY_EFFECT_KIND_BARIIER, GetCenterPos(), true);
		CEffekseerCtrl::SetScale(m_barrierEffectID, VGet(1.4f, 1.4f, 1.4f));
		m_barrierHP = m_barrierMaxHP = (int)BARRIER_HP;
	}

	CUIManager::GetManager()->GetUI()->SetBossHP(m_hp);


	//アニメ再生
	m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}

//ロード
void CBoss1::Load()
{
	m_pos.x = (float)GetRand(POAISION_RAND); if (GetRand(1) == 1) { m_pos.x *= -1.f; }
	m_pos.z = (float)GetRand(POAISION_RAND); if (GetRand(1) == 1) { m_pos.z *= -1.f; }
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;

	//モデルロード
	if (m_handle == -1)
	{

	}

	//アニメ情報セット
	m_animeInfo.SetAnimeData(BOSS1_ANIME_DATA, BOSS1_ANIME_KIND_NUM, m_handle);

	//大きさセット
	MV1SetScale(m_handle, BOSS1_SCALE);

	//アニメ再生
	m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CBoss1::Load(int model_handle)
{
	m_pos.x = (float)GetRand(POAISION_RAND);
	m_pos.z = (float)GetRand(POAISION_RAND);
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;

	//モデルロード
	if (m_handle == -1)
	{
		m_handle = MV1DuplicateModel(model_handle);
	}

	//アニメ情報セット
	m_animeInfo.SetAnimeData(BOSS1_ANIME_DATA, BOSS1_ANIME_KIND_NUM, m_handle);

	//大きさセット
	MV1SetScale(m_handle, BOSS1_SCALE);

	//アニメ再生
	m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}

//ステップ
void CBoss1::Step()
{
	//生きていないならreturn
	if (!m_isAlive)return;
	if (AppearStep()) { return; }
	if (!m_isAppear)return;
#ifdef DEBUG_MODE
	//カメラマネジャー
	CCameraManager* camera_manager = CCameraManager::GetManager();
	//デバッグモードなら動かせない
	if (camera_manager->GetCurrentCameraMode() == CCameraManager::CAMERA_ID_DEBUG)return;
#endif

	StepAttackColl();

	//移動前座標記録
	m_oldPos = m_pos;

	//点滅処理
	StepFlash();

	//角度初期化
	NormalizeRot();

	//状態をみてしょりを変える
	(this->*ExecFunc[m_state])();

	//移動量加算
	m_pos = VAdd(m_pos, m_move);

	//最後にアニメアップデート
	m_animeInfo.UpdateAnime();
}

void CBoss1::ChangeAnime(Boss1AnimeKind kind)
{
	m_animeInfo.ChangeAnime(kind);
}

void CBoss1::ExecIdle()
{

	//移動量を０に
	m_move = ZERO_VECTOR;

	//プレイヤーのほうを見る
	FacingSlowPlayer();

	//m_state = ENEMY_STATE_WALK;
	//m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_WALK);
}
void CBoss1::ExecWalk()
{




}
void CBoss1::ExecAvoid()
{

}
void CBoss1::ExecTrack()
{
	//プレイヤーの方を向いて歩く
	//まだプレイヤーの方をむいていないなら歩かない
	if (IsFacingPlayer())
	{
		VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
		VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);
		m_rot.y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
		//正規化
		vec = CMyMath::VecNormalize(vec);
		//スカラー倍
		vec = CMyMath::VecScale(vec, BOSS1_MOVE_SPEED);
		//情報反映
		m_move = vec;
	}
	else
	{
		FacingSlowPlayer();
	}

	if (m_hp < m_maxHP / 2)
	{
		m_attackCount[BOSS1_ATTACK_SPECIAL] += FRAME_TIME;
		if (m_attackCount[BOSS1_ATTACK_SPECIAL] >= m_attackMaxCount[BOSS1_ATTACK_SPECIAL])
		{
			m_state = ENEMY_STATE_ATTACK;
			m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK_SPECIAL);
			m_attackKind = BOSS1_ATTACK_SPECIAL;

			//カウント用変数初期化
			m_attackCount[BOSS1_ATTACK_SPECIAL] = 0;
			m_attackMaxCount[BOSS1_ATTACK_SPECIAL] = ATTACK_MAX[BOSS1_ATTACK_SPECIAL] + GetRand(ATTACK_RAND[BOSS1_ATTACK_SPECIAL]);
		}
	}
 
	m_attackCount[BOOS1_ATTACK_DASH] += FRAME_TIME;
	if (m_attackCount[BOOS1_ATTACK_DASH] >= m_attackMaxCount[BOOS1_ATTACK_DASH])
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK_DASH);
		m_attackKind = BOOS1_ATTACK_DASH;
	
		//カウント用変数初期化
		m_attackCount[BOOS1_ATTACK_DASH] = 0;
		m_attackMaxCount[BOOS1_ATTACK_DASH] = ATTACK_MAX[BOOS1_ATTACK_DASH] + GetRand(ATTACK_RAND[BOOS1_ATTACK_DASH]);
	}


	m_attackCount[BOSS1_ATTACK_JUMP] += FRAME_TIME;
	if (m_attackCount[BOSS1_ATTACK_JUMP] >= m_attackMaxCount[BOSS1_ATTACK_JUMP])
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK_JUMP);
		m_attackKind = BOSS1_ATTACK_JUMP;
	
		//カウント用変数初期化
		m_attackCount[BOSS1_ATTACK_JUMP] = 0;
		m_attackMaxCount[BOSS1_ATTACK_JUMP] = ATTACK_MAX[BOSS1_ATTACK_JUMP] + GetRand(ATTACK_RAND[BOSS1_ATTACK_JUMP]);
	}


}
void CBoss1::ExecAttackReady()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	//	プレイヤーのほうを向く
	FacingSlowPlayer();

	//	カウント加算
	m_attackCount[BOSS1_ATTACK_HIT] += FRAME_TIME;


	//	カウントがたまったらHit攻撃へ
	if (m_attackCount[BOSS1_ATTACK_HIT] >= m_attackMaxCount[BOSS1_ATTACK_HIT])
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK);
		m_attackKind = BOSS1_ATTACK_HIT;

		//カウント用変数初期化
		m_attackCount[BOSS1_ATTACK_HIT] = 0;
		m_attackMaxCount[BOSS1_ATTACK_HIT] = ATTACK_MAX[BOSS1_ATTACK_HIT] + GetRand(ATTACK_RAND[BOSS1_ATTACK_HIT]);
	}

	if (m_hp < m_maxHP / 2)
	{
		m_attackCount[BOSS1_ATTACK_ALL] += FRAME_TIME;
		if (m_attackCount[BOSS1_ATTACK_ALL] >= m_attackMaxCount[BOSS1_ATTACK_ALL])
		{
			m_state = ENEMY_STATE_ATTACK;
			m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK_ALL);
			m_attackKind = BOSS1_ATTACK_ALL;

			//カウント用変数初期化
			m_attackCount[BOSS1_ATTACK_ALL] = 0;
			m_attackMaxCount[BOSS1_ATTACK_ALL] = ATTACK_MAX[BOSS1_ATTACK_ALL] + GetRand(ATTACK_RAND[BOSS1_ATTACK_ALL]);
		}
	}

	m_attackCount[BOSS1_ATTACK_CHAIN] += FRAME_TIME;
	if (m_attackCount[BOSS1_ATTACK_CHAIN] >= m_attackMaxCount[BOSS1_ATTACK_CHAIN])
	{
		m_state = ENEMY_STATE_ATTACK;
			m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK_CHAIN);
			m_attackKind = BOSS1_ATTACK_CHAIN;
		
			//カウント用変数初期化
			m_attackCount[BOSS1_ATTACK_CHAIN] = 0;
			m_attackMaxCount[BOSS1_ATTACK_CHAIN] = ATTACK_MAX[BOSS1_ATTACK_CHAIN] + GetRand(ATTACK_RAND[BOSS1_ATTACK_CHAIN]);
	}

	if (CCommon::GetProbability(5))
	{
		if (m_hp < m_maxHP / 2)
		{
			m_attackCount[BOSS1_ATTACK_SPECIAL] += FRAME_TIME;
			if (m_attackCount[BOSS1_ATTACK_SPECIAL] >= m_attackMaxCount[BOSS1_ATTACK_SPECIAL])
			{
				m_state = ENEMY_STATE_ATTACK;
				m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK_SPECIAL);
				m_attackKind = BOSS1_ATTACK_SPECIAL;

				//カウント用変数初期化
				m_attackCount[BOSS1_ATTACK_SPECIAL] = 0;
				m_attackMaxCount[BOSS1_ATTACK_SPECIAL] = ATTACK_MAX[BOSS1_ATTACK_SPECIAL] + GetRand(ATTACK_RAND[BOSS1_ATTACK_SPECIAL]);
			}
		}
	}

	if (CCommon::GetProbability(5))
	{
		m_attackCount[BOOS1_ATTACK_DASH] += FRAME_TIME;
		if (m_attackCount[BOOS1_ATTACK_DASH] >= m_attackMaxCount[BOOS1_ATTACK_DASH])
		{
			m_state = ENEMY_STATE_ATTACK;
			m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK_DASH);
			m_attackKind = BOOS1_ATTACK_DASH;

			//カウント用変数初期化
			m_attackCount[BOOS1_ATTACK_DASH] = 0;
			m_attackMaxCount[BOOS1_ATTACK_DASH] = ATTACK_MAX[BOOS1_ATTACK_DASH] + GetRand(ATTACK_RAND[BOOS1_ATTACK_DASH]);
		}
	}

	if (CCommon::GetProbability(5))
	{
		m_attackCount[BOSS1_ATTACK_JUMP] += FRAME_TIME;
		if (m_attackCount[BOSS1_ATTACK_JUMP] >= m_attackMaxCount[BOSS1_ATTACK_JUMP])
		{
			m_state = ENEMY_STATE_ATTACK;
			m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_ATTACK_JUMP);
			m_attackKind = BOSS1_ATTACK_JUMP;

			//カウント用変数初期化
			m_attackCount[BOSS1_ATTACK_JUMP] = 0;
			m_attackMaxCount[BOSS1_ATTACK_JUMP] = ATTACK_MAX[BOSS1_ATTACK_JUMP] + GetRand(ATTACK_RAND[BOSS1_ATTACK_JUMP]);
		}
	}
}
void CBoss1::ExecAttack()
{
	(this->*ExecAttackFunc[m_attackKind])();


	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;
		m_attackKind = BOSS1_ATTACK_NONE;
		
		m_move = ZERO_VECTOR;
	}
}

void CBoss1::ExecAttackHit()
{
	m_move = ZERO_VECTOR;

	//	途中までプレイヤーの方を向く
	if (m_animeInfo.m_currentFrame < FACING_PLAYER_HIT_TIME)
		FacingPlayer();

	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(50.f))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 55.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y = 5.f;

		//エフェクトリクエスト
		int handle = CEnemyManager::GetManager()->RequestEffect(ENEMY_EFFECT_KIND_BIG_BLUE_GREEN,
			attack_center_pos, false , 111);

		CEffekseerCtrl::SetScale(handle, VGet(1.0f, 2.0f, 1.0f));
		CEffekseerCtrl::SetSpeed(handle, 1.4f);
		
	}

	//攻撃判定
	if (m_animeInfo.IsBetweenFrame(52, 55))
	{
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_ATTACK3);
	}
}
void CBoss1::ExecAttackAll()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	if (m_animeInfo.IsSpecificFrame(24))
	{
		m_animeInfo.SetReproductionSpeed(1.5f);
	}

	//28,44
	if (m_animeInfo.IsBetweenFrame(32, 44))
	{
		m_isAttack = true;
	}
	else
	{
		int a = 0;
		a++;
	}


	if (m_animeInfo.IsSpecificFrame(26))
	{
		VECTOR attack_center_pos = GetCenterPos();
		attack_center_pos.y = 5.f;

		//エフェクトリクエスト
		int handle = CEnemyManager::GetManager()->RequestEffect(ENEMY_EFFECT_KIND_SONIC_BOOM1,
			attack_center_pos, false);

		CEffekseerCtrl::SetScale(handle, VGet(1.7f, 1.7f, 0.5f));
		CEffekseerCtrl::SetSpeed(handle, 1.4f);
		CEffekseerCtrl::SetRot(handle, VGet(-DX_PI_F / 2.f, 0.f, 0.f));
	}

}
void CBoss1::ExecAttackChain()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	//	プレイヤーのほうを向く
	if (!m_animeInfo.IsBetweenFrame(40, 50))
	{
		FacingSlowPlayer(3);
	}

	if (m_animeInfo.IsSpecificFrame(40)) { m_animeInfo.SetReproductionSpeed(0.5f); }
	if (m_animeInfo.IsSpecificFrame(50)) { m_animeInfo.SetReproductionSpeed(1.f); }

	if (m_animeInfo.IsBetweenFrame(71, static_cast<int>(m_animeInfo.m_frameTime)))
	{
		m_animeInfo.SetReproductionSpeed(0.3f);
	}

	if (m_animeInfo.IsBetweenFrame(36, 40))
	{
		m_isAttack = true;
	}

	if (m_animeInfo.IsBetweenFrame(62, 66))
	{
		m_isAttack = true;
	}
}
void CBoss1::ExecAttackDash()
{
	//途中まで追撃する
	if (m_animeInfo.IsBetweenFrame(0, 19))
	{
		FacingSlowPlayer(9.f);
		m_move = ZERO_VECTOR;
	}
	else if (m_animeInfo.IsBetweenFrame(20,45))
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 2.5f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 2.5f;

		FacingSlowPlayer(1.f);
	}

	if (m_animeInfo.IsSpecificFrame(20))
	{
		m_animeInfo.SetReproductionSpeed(3.0f);
	}

	if (m_animeInfo.IsBetweenFrame(60.f, m_animeInfo.m_frameTime))
	{
		CMyMath::VecScale(m_move, 0.9f);
	}

	if (m_animeInfo.IsBetweenFrame(25, 70))
	{
		m_isAttack = true;
	}

}
void CBoss1::ExecAttackJump()
{
	if (m_animeInfo.IsBetweenFrame(0, 39))
	{
		FacingSlowPlayer(9.f);
		m_move = ZERO_VECTOR;
	}
	else if (m_animeInfo.IsBetweenFrame(40, 62))
	{
		FacingSlowPlayer(4.f);

		VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
		VECTOR boss_pos = m_pos;

		float distance = CMyMath::VecLong(boss_pos, player_pos);
		distance -= 20.f;

		int all_num = m_animeInfo.GetProcessingNumToFrame(80);

		if (all_num > 0.f)
		{

			m_jumpAttackSpd = distance / static_cast<float>(all_num);

			//x,zの移動量を決める
			m_move.x = sinf(m_rot.y + DX_PI_F) * m_jumpAttackSpd;
			m_move.z = cosf(m_rot.y + DX_PI_F) * m_jumpAttackSpd;
		}
	}
	else if (m_animeInfo.IsBetweenFrame(63, 80))
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * m_jumpAttackSpd;
		m_move.z = cosf(m_rot.y + DX_PI_F) * m_jumpAttackSpd;
	}
	else
	{
		m_move = ZERO_VECTOR;
	}


	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(78.f))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, ATTACK_JUMP_COLLISION_ADD);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y = 2.f;

		//エフェクトリクエスト
		int handle = CEnemyManager::GetManager()->RequestEffect(ENEMY_EFFECT_KIND_BLOW,
			attack_center_pos, false, 22);

		CEffekseerCtrl::SetScale(handle, VGet(1.7f, 1.7f, 1.7f));
		CEffekseerCtrl::SetSpeed(handle, 1.4f);

	}

	//当たり判定
	if (m_animeInfo.IsSpecificFrame(80))
	{
		LetsAttackColl(1000, ATTACK_COLLISION_ADD[m_attackKind], ATTACK_COLLISION_RADIUS[m_attackKind]);
		CSoundManager::PlaySE(SE_KIND_ATTACK3);
	}

}
void CBoss1::ExecAttackSpecial()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	if (m_animeInfo.IsSpecificFrame(20))
	{
		AttackSpecialRand(200.f);

		CSoundManager::PlaySE(SE_KIND_THUNDER);
	}

	if (m_animeInfo.IsSpecificFrame(25))
	{
		AttackSpecialHomingRand(90.f);
	}

	if (m_animeInfo.IsSpecificFrame(30))
	{
		AttackSpecialRand(200.f);
		CSoundManager::PlaySE(SE_KIND_THUNDER);
	}

	if (m_animeInfo.IsSpecificFrame(40))
	{
		AttackSpecialHoming();
	}

	if (m_animeInfo.IsSpecificFrame(45))
	{
		AttackSpecialRand(200.f);
		CSoundManager::PlaySE(SE_KIND_THUNDER);
	}

	if (m_animeInfo.IsSpecificFrame(50))
	{
		AttackSpecialRand(200.f);
	}

	if (m_animeInfo.IsSpecificFrame(60))
	{
		AttackSpecialHomingRand(90.f);
		CSoundManager::PlaySE(SE_KIND_THUNDER);
	}

	if (m_animeInfo.IsSpecificFrame(65))
	{
		AttackSpecialRand(150.f);
	}

	if (m_animeInfo.IsSpecificFrame(70))
	{
		AttackSpecialHoming();
		CSoundManager::PlaySE(SE_KIND_THUNDER);
	}
}
void CBoss1::AttackSpecialRand(float f)
{
	//エフェクト出す
	VECTOR effect_pos = { m_pos.x + CCommon::GetPMRandF(f),0.f,m_pos.z + CCommon::GetPMRandF(f) };
	VECTOR start_pos = effect_pos;
	start_pos.y += 100.f;

	VECTOR move_vec = { 0.f, -1.f, 0.f };

	VECTOR beam_rot = m_rot;
	beam_rot.y = CMyMath::VecDir(move_vec.y, CMyMath::VecLong(VGet(move_vec.x, move_vec.z, 0.f)));

	//リクエスト情報
	BeamRequestInfo request_info = { start_pos,	beam_rot,	move_vec,	0,		500,	1.0f,		0,		0.f, };

	CBeamManager::GetManager()->RequestEnemyBeam(request_info, BEAM_KIND_BOSS1_THUNDER, effect_pos);
}
void CBoss1::AttackSpecialHoming()
{
	//エフェクト出す
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	player_pos.y += 5.f;
	VECTOR start_pos = player_pos;
	start_pos.y += 100.f;


	VECTOR move_vec = { 0.f, -1.f, 0.f };

	VECTOR beam_rot = m_rot;
	beam_rot.y = CMyMath::VecDir(move_vec.y, CMyMath::VecLong(VGet(move_vec.x, move_vec.z, 0.f)));

	//リクエスト情報
	BeamRequestInfo request_info = { start_pos,	beam_rot,	move_vec,	0,		300,	1.0f,		0,		0.f, };

	CBeamManager::GetManager()->RequestEnemyBeam(request_info, BEAM_KIND_BOSS1_THUNDER, player_pos);
}
void CBoss1::AttackSpecialHomingRand(float f)
{
	//エフェクト出す
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR add_pos = { CCommon::GetPMRandF(f), 5.f, CCommon::GetPMRandF(f) };
	player_pos = CMyMath::VecAdd(player_pos, add_pos);
	VECTOR start_pos = player_pos;
	start_pos.y += 100.f;


	VECTOR move_vec = { 0.f, -1.f, 0.f };

	VECTOR beam_rot = m_rot;
	beam_rot.y = CMyMath::VecDir(move_vec.y, CMyMath::VecLong(VGet(move_vec.x, move_vec.z, 0.f)));

	//リクエスト情報
	BeamRequestInfo request_info = { start_pos,	beam_rot,	move_vec,	0,		300,	1.0f,		0,		0.f, };

	CBeamManager::GetManager()->RequestEnemyBeam(request_info, BEAM_KIND_BOSS1_THUNDER, player_pos);
}

void CBoss1::ExecHit()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	////	プレイヤーのほうを向く
	//FacingPlayer();

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime / 10.f)
	{
		//x,zの移動量を決める
		m_move.x = -sinf(m_rot.y + DX_PI_F) * 0.9f;
		m_move.z = -cosf(m_rot.y + DX_PI_F) * 0.9f;

		return;
	}

	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;
	}
}
void CBoss1::ExecDown()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	if (m_animeInfo.m_currentFrame < 10.f)
	{
		//x,zの移動量を決める
		m_move.x = -sinf(m_rot.y + DX_PI_F) * 0.5f;
		m_move.z = -cosf(m_rot.y + DX_PI_F) * 0.5f;

		return;
	}

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 3.f / 5.f) { return; }

	//ｈｐが０ならisAliveをfalseに
	if (m_hp <= 0)
	{
		m_isAlive = m_isAppear = false;
		CEnemyManager::GetManager()->SubAliveNum();

		if (m_barrierEffectID != -1)
		{
			CEffekseerCtrl::Stop(m_barrierEffectID);
			m_barrierEffectID = -1;
		}
	}

	if (m_animeInfo.IsSpecificFrame(80.f))
	{
		m_animeInfo.SetReproductionSpeed(0.5f);
	}


	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(BOSS1_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;

		m_isInvincible = true;
	}


}

void CBoss1::Fin()
{

}

//点滅処理
void CBoss1::StepFlash()
{
	if (!m_isInvincible)
		return;

	m_invincibleVal += FRAME_TIME;

	//点滅切り替え
	if (m_invincibleVal % 200 >= 100)
	{
		m_isDraw = true;
	}
	else
	{
		m_isDraw = false;
	}

	//一定時間たったらもとに戻る
	if (m_invincibleVal >= BOSS1_INVINCIBLE_TIME)
	{
		m_isInvincible = false;
		m_isDraw = true;

		m_invincibleVal = 0;
	}
}

//データ反映処理
void CBoss1::Update()
{
	//生きていないならreturn
	if (!m_isAlive)return;
	if (!m_isAppear)return;

	//回転値反映
	MV1SetRotationXYZ(m_handle, m_rot);

	//座標セット
	MV1SetPosition(m_handle, m_pos);

	//再生時間のセット
	MV1SetAttachAnimTime(m_handle, m_animeInfo.m_attachIndex,
		m_animeInfo.m_currentFrame);

	//エフェクトアップデート
	if (m_barrierEffectID != -1)
	{
		CEffekseerCtrl::SetPosition(m_barrierEffectID, GetCenterPos());
	}
}

//座標セット＆描画
void CBoss1::Draw()
{
	//生きていないならreturn
	if (!m_isAlive)return;
	if (!m_isAppear)return;

	if (m_isDraw)
	{
		//最後に描画
		MV1DrawModel(m_handle);
	}

	VECTOR pos = m_pos;
	pos.y += 50.f;

#ifdef DEBUG_MODE

	VECTOR player_coll_center_pos = m_pos;
	player_coll_center_pos.y += 18.f;
	VECTOR player_coll_pos_up = player_coll_center_pos;
	player_coll_pos_up.y += 18.f;
	player_coll_pos_up.x += 9.f;
	player_coll_pos_up.z += 9.f;
	VECTOR player_coll_pos_down = player_coll_center_pos;
	player_coll_pos_down.y -= 18.f;
	player_coll_pos_down.x -= 9.f;
	player_coll_pos_down.z -= 9.f;

	DrawCube3D(player_coll_pos_up, player_coll_pos_down, GetColor(255, 50, 50),
		GetColor(0, 0, 0), false);

	VECTOR player_pos = m_pos;
	player_pos.y += BOSS1_CENTER_Y;
	DrawSphere3D(player_pos, m_radius, 8, GetColor(255, 255, 255), GetColor(0, 0, 0), false);

#if 0

	DrawSphere3D(player_pos, PLAYER_FIND_RADIUS, 4, GetColor(50, 255, 50), GetColor(0, 0, 0), false);

#endif

#endif
}


bool CBoss1::JudgeCollission(EnemyHitKind kind)
{
	//まず生きていないならfalseをかえす
	if (!m_isAlive) { return false; }
	if (!m_isAppear) { return false; }

	switch (kind)
	{
		//敵同士
	case ENEMY_HIT_ENEMY:
		break;

		//プレイヤー
	case ENEMY_HIT_PLAYER:
		break;

		//プレイヤー攻撃
	case ENEMY_HIT_PLAYER_ATTACK:
		//ダウンしてるときや無敵状態のときは当たり判定をとらない
		if (m_state == ENEMY_STATE_DOWN || m_isInvincible)
			return false; break;

		//視界
	case ENEMY_HORIZON_HIT_PLAYER:
		//ヒット、ダウン、攻撃準備、攻撃のときは当たり判定をとらない
		if (m_state == ENEMY_STATE_HIT ||
			m_state == ENEMY_STATE_DOWN ||
			m_state == ENEMY_STATE_ATTACK)
			return false; break;

	case ENEMY_RANGE_HIT_PLAYER:
		//敵がプレイヤーをみつけてないときはとらない
		if (m_state != ENEMY_STATE_TRACK)
			return false; break;

	case ENEMY_ATTACK_HIT_PLAYER:
		if (!m_isAttack)return false; break;
	}

	//最後まで回ったらtrueをかえす
	return true;
}

//当たり判定処理
void CBoss1::HitFunc(EnemyHitKind hit_kind)
{
	switch (hit_kind)
	{
	case ENEMY_HIT_PLAYER:


		break;


	case ENEMY_HIT_PLAYER_ATTACK:

		CPlayer* player = CPlayerManager::GetManager()->GetPlayer();
		PlayerAttackInfo player_attack_info =
			player->GetPlayerAttackInfo();
		PlayerBoostKind boost_kind = player->GetBoostKind();


		//	まずバリアがあるならバリアにダメージ
		if (m_barrierEffectID != -1)
		{
			m_barrierHP -= player_attack_info.barrier_damage;

			if (m_barrierHP <= 0)
			{
				CEffekseerCtrl::Stop(m_barrierEffectID);
				m_barrierEffectID = -1;

				//	超えた分HPにダメージ
				m_hp -= CMyMath::Abs(m_barrierHP);
				m_barrierHP = 0;

				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_HIT);
				m_animeInfo.SetReproductionSpeed(2.2f);
			}
		}
		else
		{
			m_hp -= player_attack_info.damage;

			//	攻撃種類
			switch (player->GetAttackKind())
			{
			case PLAYER_ATTACK_NORMAL1:
			case PLAYER_ATTACK_NORMAL2:
			case PLAYER_ATTACK_NORMAL3:
				
				break;

			case PLAYER_ATTACK_NORMAL4:
			{
				int combo_count = player->GetBoostComboCount();
				if (combo_count == 0)
				{
					if (CCommon::GetProbability(10))
					{
						m_state = ENEMY_STATE_HIT;
						m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_HIT);
						m_animeInfo.SetReproductionSpeed(2.2f);
					}
				}
				else if (combo_count == 1)
				{
					if (CCommon::GetProbability(10))
					{
						m_state = ENEMY_STATE_HIT;
						m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_HIT);
						m_animeInfo.SetReproductionSpeed(2.2f);
					}
				}
				else
				{
					m_state = ENEMY_STATE_HIT;
					m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_HIT);
					m_animeInfo.SetReproductionSpeed(2.2f);
				}

				break;
			}
			case PLAYER_ATTACK_NORMAL5:
			case PLAYER_ATTACK_NORMAL6:
			
				break;

			case PLAYER_ATTACK_NORMAL7:
			
				break;

			case PLAYER_ATTACK_STRONG1:
				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_HIT);

				//	プレイヤーのほうを向く
				FacingPlayer();

				break;
			case PLAYER_ATTACK_RISING:
				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_HIT);

				//	プレイヤーのほうを向く
				FacingPlayer();

				break;
			}
		}


		//	０いかになったらダウンする
		if (m_hp <= 0)
		{
			m_hp = 0;
			m_state = ENEMY_STATE_DOWN;
			m_animeInfo.PlayAnime(BOSS1_ANIME_KIND_DOWN);
		}

		break;
	}
}

void CBoss1::ChangeAnimeToState()
{
	Boss1AnimeKind anime_kind = BOSS1_ANIME_KIND_NONE;

	switch (m_state)
	{
	case ENEMY_STATE_ATTACK: anime_kind = BOSS1_ANIME_KIND_ATTACK; break;
	case ENEMY_STATE_ATTACK_READY: anime_kind = BOSS1_ANIME_KIND_IDLE; break;
	case ENEMY_STATE_DOWN: anime_kind = BOSS1_ANIME_KIND_DOWN; break;
	case ENEMY_STATE_HIT: anime_kind = BOSS1_ANIME_KIND_HIT; break;
	case ENEMY_STATE_IDLE: anime_kind = BOSS1_ANIME_KIND_IDLE; break;
	case ENEMY_STATE_TRACK: anime_kind = BOSS1_ANIME_KIND_WALK; break;
	case ENEMY_STATE_WALK: anime_kind = BOSS1_ANIME_KIND_WALK; break;
	}

#ifdef DEBUG_MODE

#endif 
	m_animeInfo.ChangeAnime(anime_kind);

}

float CBoss1::GetAttackRadius()
{
	if (m_attackData.isUse)
		return m_attackData.attackRadius;
	else
		return ATTACK_COLLISION_RADIUS[m_attackKind];
}


VECTOR CBoss1::GetAttackCenterPos()
{
	VECTOR center_pos = GetCenterPos();

	//向いてる向きに少し進める
	VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };

	if (m_attackData.isUse)
	{
		//スカラー倍してたす
		add = CMyMath::VecScale(add, m_attackData.attackAdd);
	}
	else
	{
		//スカラー倍してたす
		add = CMyMath::VecScale(add, ATTACK_COLLISION_ADD[m_attackKind]);
	}

	return CMyMath::VecAdd(center_pos, add);
}

AttackType CBoss1::GetAttackType()
{
	return ATTACK_TYPE_HIT;
}

int CBoss1::GetAttackKind()
{
	return static_cast<int>(m_attackKind);
}