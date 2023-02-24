#include "DxLib.h"

#include <math.h>
#include <cassert>

#include "Player.h"

#include "../Enemy/EnemyManager.h"

#include "../../Common.h"
#include "../../Field/Field.h"
#include "../../Input/InputManager.h"
#include "../../Debug/Debug.h"
#include "../../Camera/CameraManager.h"
#include "../../Draw/DrawManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Sound/SoundManager.h"
#include "../../Anime/Anime.h"
#include "../../MyMath/MyMath.h"
#include "../../Common.h"
#include "../../Input/Pad/Pad.h"
#include "../../Scene/PlayScene/PlayScene.h"

#include "../Effekseer.h"


static const CPlayAnimeData PLAYER_ANIME_DATA[PLAYER_ANIME_KIND_NUM] =
{
	{ PLAYER_ANIME_KIND_1COMBO_ATTACK,				1.1f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_2COMBO_ATTACK,				0.7f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_3COMBO_ATTACK,				1.2f,	-1, false, ANIME_TYPE_END	},

	{ PLAYER_ANIME_KIND_2SLASH,						0.9f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_3SLASH_1,					1.1f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_3SLASH_2,					0.8f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_3ATTACK,					0.8f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_KICK,						0.4f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_OUT_SLASH,					0.5f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_ROT_JUMP_ATTACK,			0.5f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_ROT_UP_ATTACK,				1.0f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_SIDE1_ATTACK,				1.7f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_SIDE2_ATTACK,				1.02f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_UNDER_SLASH,				0.8f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_UNDER_ATTACK,				1.0f,	-1, false, ANIME_TYPE_END	},

	{ PLAYER_ANIME_KIND_AVOID,						1.0f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_DEATH,						1.0f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_DEFAULT,					1.0f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_GET_UP,						1.0f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_HIT,						0.3f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_IDLE,						1.0f,	-1, false, ANIME_TYPE_LOOP	},
	{ PLAYER_ANIME_KIND_KNOCK_BACK,					0.7f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_POWER_UP,					0.55f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_RUN,						0.5f,	-1, false, ANIME_TYPE_LOOP	},
	{ PLAYER_ANIME_KIND_SWORD_ATTACK1,				1.1f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_SWORD_ATTACK2,				0.9f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_SWORD_ATTACK3,				1.2f,	-1, false, ANIME_TYPE_END	},
	{ PLAYER_ANIME_KIND_WALK,						0.5f,	-1, false, ANIME_TYPE_LOOP	},
};

//プレイヤー半径
static const float PLAYER_RADIUS = 12.f;

static const float PLAYER_MAX_GRAVITY = MAX_GRAVITY;
static const float PLAYER_MAX_GRAVITY2 = MAX_GRAVITY / 3.f;

static const float PLAYER_AVOID_SPEED1 = 2.5f;
static const float PLAYER_AVOID_SPEED2 = 3.5f;

static const float PLAYER_WALK_MAX = 0.8f;

//プレイヤー移動量
static const float PLAYER_MOVE_MAX = 40.f;
static const float PLAYER_MOVE = 5.f;

static const float PLAYER_SPEED = 2.5f;

#ifdef DEBUG_MODE

static const float HEALING_MP = 0.02f;

#else
static const float HEALING_MP = 0.01f;

#endif

//プレイヤーモデルサイズ
static const VECTOR PLAYER_SIZE = { 14.f,46.f,14.f };
static const VECTOR PLAYER_SCALE = { 25.3f,25.3f,25.3f };
static const VECTOR PLAYER_CENTER_COLLISION = VScale(PLAYER_SIZE, 0.5f);
static const VECTOR PLAYER_SWORD_SCALE = { 0.04f,0.04f,0.04f };

//モデルパス
static const char* PLAYER_PATH = "Data/Model/Character/Player/Player.x";
static const char* PLAYER_SWORD_PATH = "Data/Model/Character/Player/Tou/Tou.x";

//無敵時間
static const int PLAYER_INVINCIBLE_TIME = 2000;

//ジャンプ力
static const float PLAYER_JUMP_POWER = 8.8f;

static const float RISING_MAX_LEN = 200.f;

//HP
static const int PLAYER_HP = 100;
static const float PLAYER_MP = 100.f;

//MP使用料
static const float PLAYER_STRONG_ATTACK1_MP = 40.f;
static const float PLAYER_STRONG_ATTACK2_MP = 40.f;
static const float PLAYER_STRONG_ATTACK3_MP = 1.f;
static const float PLAYER_NORMAL_ATTACK5_MP = 5.f;
static const float PLAYER_NORMAL_ATTACK6_MP = 5.f;
static const float PLAYER_NORMAL_ATTACK7_MP = 7.f;
static const float PLAYER_NORMAL_ATTACK8_MP = 8.f;



static const float SWORD_ATTACK1_COLLISION_FRAME = 20.f;
static const float SWORD_ATTACK2_COLLISION_FRAME = 10.f;
static const float SWORD_ATTACK3_COLLISION_FRAME = 20.f;
static const float SWORD_ATTACK4_COLLISION_FRAME = 21.f;
static const float SWORD_ATTACK5_COLLISION_FRAME = 26.f;
static const float SWORD_ATTACK6_COLLISION_FRAME = 17.f;
static const float SWORD_ATTACK7_COLLISION_FRAME = 25.f;
static const float SWORD_ATTACK8_COLLISION_FRAME = 20.f;
static const float SWORD_ATTACK9_COLLISION_FRAME = 20.f;
static const float SWORD_STRONG_ATTACK1_COLLISION_FRAME = 38.f;
static const float SWORD_STRONG_ATTACK2_COLLISION_FRAME = 20.f;
static const float SWORD_STRONG_ATTACK3_COLLISION_FRAME = 20.f;
static const float ATTACK_COLLISION_FRAME[PLAYER_ATTACK_NUM] = 
{
	SWORD_ATTACK1_COLLISION_FRAME,
	SWORD_ATTACK2_COLLISION_FRAME,
	SWORD_ATTACK3_COLLISION_FRAME,
	SWORD_ATTACK4_COLLISION_FRAME,
	SWORD_ATTACK5_COLLISION_FRAME,
	SWORD_ATTACK6_COLLISION_FRAME,
	SWORD_ATTACK7_COLLISION_FRAME,
	SWORD_ATTACK8_COLLISION_FRAME,
	SWORD_ATTACK9_COLLISION_FRAME,
	SWORD_STRONG_ATTACK1_COLLISION_FRAME,
	SWORD_STRONG_ATTACK2_COLLISION_FRAME,
	SWORD_STRONG_ATTACK3_COLLISION_FRAME,
	SWORD_ATTACK4_COLLISION_FRAME,
};


static const int SWORD_ATTACK1_DAMAGE = 10;
static const int SWORD_ATTACK2_DAMAGE = 10;
static const int SWORD_ATTACK3_DAMAGE = 10;
static const int SWORD_ATTACK4_DAMAGE = 6;
static const int SWORD_ATTACK5_DAMAGE = 12;
static const int SWORD_ATTACK6_DAMAGE = 13;
static const int SWORD_ATTACK7_DAMAGE = 25;
static const int SWORD_ATTACK8_DAMAGE = 0;
static const int SWORD_ATTACK9_DAMAGE = 0;
static const int SWORD_STRONG_ATTACK1_DAMAGE = 30;
static const int SWORD_STRONG_ATTACK2_DAMAGE = 30;
static const int SWORD_STRONG_ATTACK3_DAMAGE = 30;
static const int SWORD_RIZING_ATTACK_DAMAGE = 30;

static const PlayerAttackInfo PLAYER_ATTACK_INFO[PLAYER_ATTACK_NUM] =
{
	{	10,			10,		PLAYER_ATTACK_NORMAL1	},
	{	10,			10,		PLAYER_ATTACK_NORMAL2	},
	{	10,			15,		PLAYER_ATTACK_NORMAL3	},
	{	 6,			10,		PLAYER_ATTACK_NORMAL4	},
	{	12,			18,		PLAYER_ATTACK_NORMAL5	},
	{	13,			18,		PLAYER_ATTACK_NORMAL6	},
	{	10,			20,		PLAYER_ATTACK_NORMAL7	},
	{	 0,			 0,		PLAYER_ATTACK_NORMAL8	},
	{	 0,			 0,		PLAYER_ATTACK_NORMAL9	},
	{	40,		   100,		PLAYER_ATTACK_STRONG1	},
	{	40,		   100,		PLAYER_ATTACK_STRONG2	},
	{	40,		   100,		PLAYER_ATTACK_STRONG3	},
	{	40,		   100,		PLAYER_ATTACK_RISING	},
};



static const float SWORD_ATTACK1_COLLISION_ADD = 60.f;
static const float SWORD_ATTACK2_COLLISION_ADD = 40.f;
static const float SWORD_ATTACK3_COLLISION_ADD = 40.f;
static const float SWORD_ATTACK4_COLLISION_ADD = 40.f;
static const float SWORD_ATTACK5_COLLISION_ADD = 47.f;
static const float SWORD_ATTACK6_COLLISION_ADD = 50.f;
static const float SWORD_ATTACK7_COLLISION_ADD = 60.f;
static const float SWORD_ATTACK8_COLLISION_ADD = 40.f;
static const float SWORD_ATTACK9_COLLISION_ADD = 40.f;
static const float SWORD_STRONG_ATTACK1_COLLISION_ADD = 15.f;
static const float SWORD_STRONG_ATTACK2_COLLISION_ADD = 20.f;
static const float SWORD_STRONG_ATTACK3_COLLISION_ADD = 20.f;
static const float ATTACK_COLLISION_ADD[PLAYER_ATTACK_NUM] =
{
	SWORD_ATTACK1_COLLISION_ADD,
	SWORD_ATTACK2_COLLISION_ADD,
	SWORD_ATTACK3_COLLISION_ADD,
	SWORD_ATTACK4_COLLISION_ADD,
	SWORD_ATTACK5_COLLISION_ADD,
	SWORD_ATTACK6_COLLISION_ADD,
	SWORD_ATTACK7_COLLISION_ADD,
	SWORD_ATTACK8_COLLISION_ADD,
	SWORD_ATTACK9_COLLISION_ADD,
	SWORD_STRONG_ATTACK1_COLLISION_ADD,
	SWORD_STRONG_ATTACK2_COLLISION_ADD,
	SWORD_STRONG_ATTACK3_COLLISION_ADD,

};



static const float SWOAD_ATTACK1_COLLISION_RADIUS = 30.f;
static const float SWOAD_ATTACK2_COLLISION_RADIUS = 40.f;
static const float SWOAD_ATTACK3_COLLISION_RADIUS = 40.f;
static const float SWOAD_ATTACK4_COLLISION_RADIUS = 30.f;
static const float SWOAD_ATTACK5_COLLISION_RADIUS = 60.f;
static const float SWOAD_ATTACK6_COLLISION_RADIUS = 60.f;
static const float SWOAD_ATTACK7_COLLISION_RADIUS = 70.f;
static const float SWOAD_ATTACK8_COLLISION_RADIUS = 30.f;
static const float SWOAD_ATTACK9_COLLISION_RADIUS = 30.f;
static const float SWOAD_STRONG_ATTACK1_COLLISION_RADIUS = 115.f;
static const float SWOAD_STRONG_ATTACK2_COLLISION_RADIUS = 30.f;
static const float SWOAD_STRONG_ATTACK3_COLLISION_RADIUS = 30.f;
static const float SWOAD_RISING_ATTACK_COLLISION_RADIUS = 60.f;
static const float ATTACK_COLLISION_RADIUS[PLAYER_ATTACK_NUM] = 
{
	SWOAD_ATTACK1_COLLISION_RADIUS,
	SWOAD_ATTACK2_COLLISION_RADIUS,
	SWOAD_ATTACK3_COLLISION_RADIUS,
	SWOAD_ATTACK4_COLLISION_RADIUS,
	SWOAD_ATTACK5_COLLISION_RADIUS,
	SWOAD_ATTACK6_COLLISION_RADIUS,
	SWOAD_ATTACK7_COLLISION_RADIUS,
	SWOAD_ATTACK8_COLLISION_RADIUS,
	SWOAD_ATTACK9_COLLISION_RADIUS,
	SWOAD_STRONG_ATTACK1_COLLISION_RADIUS,
	SWOAD_STRONG_ATTACK2_COLLISION_RADIUS,
	SWOAD_STRONG_ATTACK3_COLLISION_RADIUS,
	SWOAD_RISING_ATTACK_COLLISION_RADIUS,
};


static const char* EFFECT_PATH[PLAYER_EFFECT_KIND_NUM] =
{
	"Data/Effect/Barrier.efk",
	"Data/Effect/BigFire.efk",
	"Data/Effect/Fire.efk",
	"Data/Effect/Hit1.efk",
	"Data/Effect/Hit2.efk",
	"Data/Effect/Meteor.efk",
	"Data/Effect/SonicBoom.efk",
	"Data/Effect/Sword1.efk",
	"Data/Effect/Sword1.efk",
	"Data/Effect/Thunder.efk",
	"Data/Effect/PowerUp.efk",
	"Data/Effect/Beam_Blue.efk",
	"Data/Effect/Roar.efk",
};

static const VECTOR EFFECT_SCALE[PLAYER_EFFECT_KIND_NUM] =
{
	{	10.f,	10.f,	10.f		},
	{	10.f,	10.f,	10.f		},
	{	15.f,	15.f,	15.f		},
	{	10.f,	10.f,	10.f		},
	{	10.f,	10.f,	10.f		},
	{	10.f,	10.f,	10.f		},
	{	10.f,	10.f,	10.f		},
	{	10.f,	10.f,	10.f		},
	{	10.f,	10.f,	10.f		},
	{	10.f,	10.f,	10.f		}, 
};


static const int KICK_EFF_COUNT = 500; 

static const float BOOST_SUB = 0.14f;

static const float AVOID_COLL_MIN = 3;
static const float AVOID_COLL_MAX = 20;

static const float NORMAL_ATTACK1_MOVE_FRAME = 11.f;



//生成
CPlayer* CPlayer::Create()
{
	CPlayer* player = _NEW(CPlayer);

	player->Init();

	//DrawListに追加
	player->m_drawID = CDrawManager::AddData(player);

	return player;
}

//初期化
void CPlayer::Init()
{
	//memset(this, 0, sizeof(CPlayer));
	m_size = PLAYER_SIZE;
	m_hp = m_maxHP = PLAYER_HP;
	m_mp = m_maxMP = PLAYER_MP;
	m_radius = PLAYER_RADIUS;
	m_isFloating = false;
	m_pos = ZERO_VECTOR;
}

//ロード
void CPlayer::Load()
{
	m_pos = ZERO_VECTOR;
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	VECTOR vec = { 0 };
	m_rot.y = DX_PI_F;

	SetPos(vec);

	//モデルロード
	if (m_handle == -1)
	{
		m_handle = MV1LoadModel(PLAYER_PATH);
	}
	if (m_swordHandle == -1)
	{
		m_swordHandle = MV1LoadModel(PLAYER_SWORD_PATH);
	}

	//エフェクトロード
	for (int effect_index = 0; effect_index < PLAYER_EFFECT_KIND_NUM; ++effect_index)
	{
		m_effectID[effect_index] = CEffekseerCtrl::LoadData(EFFECT_PATH[effect_index]);
	}


	//アニメ情報セット
	m_animeInfo.SetAnimeData(PLAYER_ANIME_DATA, PLAYER_ANIME_KIND_NUM, m_handle);

	//大きさセット
	MV1SetScale(m_handle, PLAYER_SCALE);
	MV1SetScale(m_swordHandle, PLAYER_SWORD_SCALE);

	//アニメ再生
	m_animeInfo.PlayAnime(PLAYER_ANIME_KIND_IDLE);
	m_state = PLAYER_STATE_IDLE;
	m_boostKind = PLAYER_BOOST_CHARGE;
}

//プレイヤーステップ
void CPlayer::Step()
{

	
#ifdef DEBUG_MODE
	//カメラマネジャー
	CCameraManager* camera_manager = CCameraManager::GetManager();
	//デバッグモードなら動かせない
	if (camera_manager->GetCurrentCameraMode() == CCameraManager::CAMERA_ID_DEBUG)return;

#endif

	//移動前座標記録
	m_oldPos = m_pos;

	//点滅処理
	StepFlash();
	DamagePlayer();

	//ブースト処理
	if (StepBoost()) { return; }

	//状態をみてしょりを変える
	(this->*ExecFunc[m_state])();

	//重力処理
	StepGravity();

	//移動量加算
	m_pos = VAdd(m_pos, m_move);

	//ライジング処理
	StepRising();

	if (m_pos.y < 0.f)
	{
		m_pos.y = 0.f;
		m_move.y = 0.f;
	}

	if (m_boostEffect != -1)
	{
		CEffekseerCtrl::SetPosition(m_boostEffect, m_pos);
	}

	if (m_kickEffectID != -1)
	{
		m_kickEffectCount += FRAME_TIME;

		if (m_kickEffectCount >= KICK_EFF_COUNT)
		{
			CEffekseerCtrl::Stop(m_kickEffectID);
			m_kickEffectCount = 0;
			m_kickEffectID = -1;
		}
	}


	//最後にアニメアップデート
	m_animeInfo.UpdateAnime();

	
}
void CPlayer::StepTutorial()
{
	//移動前座標記録
	m_oldPos = m_pos;

	//点滅処理
	StepFlash();
	DamagePlayer();

	//ブースト処理
	if (StepBoostTutorial()) { return; }

	//状態をみてしょりを変える
	(this->*ExecFunc[m_state])();

	//重力処理
	StepGravity();

	//移動量加算
	m_pos = VAdd(m_pos, m_move);

	if (m_pos.y < 0.f)
	{
		m_pos.y = 0.f;
		m_move.y = 0.f;
	}

	if (m_boostEffect != -1)
	{
		CEffekseerCtrl::SetPosition(m_boostEffect, m_pos);
	}

	if (m_kickEffectID != -1)
	{
		m_kickEffectCount += FRAME_TIME;

		if (m_kickEffectCount >= KICK_EFF_COUNT)
		{
			CEffekseerCtrl::Stop(m_kickEffectID);
			m_kickEffectCount = 0;
			m_kickEffectID = -1;
		}
	}

	//最後にアニメアップデート
	m_animeInfo.UpdateAnime();
}
bool CPlayer::IsTutorialNormalAttack()
{
	if (m_attackKind == PLAYER_ATTACK_NORMAL1)
		return true;

	return false;
}

bool CPlayer::IsTutorialNormalAttack2()
{
	if (m_attackKind == PLAYER_ATTACK_NORMAL5 ||
		m_attackKind == PLAYER_ATTACK_NORMAL6 || 
		m_attackKind == PLAYER_ATTACK_NORMAL7)
		return true;

	return false;
}

bool CPlayer::IsTutorialStrongAttack()
{
	if (m_attackKind == PLAYER_ATTACK_STRONG1 ||
		m_attackKind == PLAYER_ATTACK_RISING)
		return true;

	return false;
}
bool CPlayer::IsTutorialMove()
{
	if (CInputManager::IsDown(KEY_INPUT_D, I_KEY) || CInputManager::IsDown(KEY_INPUT_A, I_KEY) ||
		CInputManager::IsDown(KEY_INPUT_W, I_KEY) || CInputManager::IsDown(KEY_INPUT_S, I_KEY) ||
		CPad::GetPad()->IsLeftStick())
	{
		return true;
	}

	return false;
	
}

bool CPlayer::IsTutorialAvoid()
{
	if (CInputManager::IsPush(KEY_INPUT_SPACE, I_KEY) || 
		CInputManager::IsPush(KEY_INPUT_LSHIFT, I_KEY) || 
		CInputManager::IsPush(XINPUT_BUTTON_A, I_PAD))
	{
		return true;
	}

	return false;

}
bool CPlayer::IsTutorialBoostGo()
{
	if (m_boostKind == PLAYER_BOOST_NOW)
	{
		return true;
	}

	return false;
}
bool CPlayer::IsTutorialBoostAttack()
{
	if (m_attackKind == PLAYER_ATTACK_NORMAL4)
		return true;

	return false;
}

bool CPlayer::StepBoost()
{
	if (m_animeInfo.m_animeID == PLAYER_ANIME_KIND_POWER_UP)
	{
		//	アニメが終わったら次へ
		if (m_animeInfo.IsEndAnime())
		{
			m_boostKind = PLAYER_BOOST_NOW;
			m_state = PLAYER_STATE_IDLE;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_IDLE);
		}

		if (m_boostEffect != -1)
		{
			CEffekseerCtrl::SetPosition(m_boostEffect, m_pos);
		}

		if (m_kickEffectID != -1)
		{
			m_kickEffectCount += FRAME_TIME;

			if (m_kickEffectCount >= KICK_EFF_COUNT)
			{
				CEffekseerCtrl::Stop(m_kickEffectID);
				m_kickEffectCount = 0;
				m_kickEffectID = -1;
			}
		}

		//最後にアニメアップデート
		m_animeInfo.UpdateAnime();
		return true;
	}

	if (m_boostKind == PLAYER_BOOST_NOW)
	{
		m_boostNum -= BOOST_SUB;

		if (m_boostNum <= 0.f)
		{
			m_boostKind = PLAYER_BOOST_CHARGE;
			m_boostNum = 0.f;

			m_animeInfo.ResetAllReproductionSpeed();
			m_boostEffect = -1;
		}
	}

	return false;
}

bool CPlayer::StepBoostTutorial()
{
	if (m_animeInfo.m_animeID == PLAYER_ANIME_KIND_POWER_UP)
	{
		//	アニメが終わったら次へ
		if (m_animeInfo.IsEndAnime())
		{
			m_boostKind = PLAYER_BOOST_NOW;
			m_state = PLAYER_STATE_IDLE;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_IDLE);
		}

		if (m_boostEffect != -1)
		{
			CEffekseerCtrl::SetPosition(m_boostEffect, m_pos);
		}

		if (m_kickEffectID != -1)
		{
			m_kickEffectCount += FRAME_TIME;

			if (m_kickEffectCount >= KICK_EFF_COUNT)
			{
				CEffekseerCtrl::Stop(m_kickEffectID);
				m_kickEffectCount = 0;
				m_kickEffectID = -1;
			}
		}

		//最後にアニメアップデート
		m_animeInfo.UpdateAnime();
		return true;
	}

	if (m_boostKind == PLAYER_BOOST_NOW)
	{
		m_boostNum -= BOOST_SUB / 2.f;

		if (m_boostNum <= 1.f)
		{
			m_boostNum = 1.f;
		}
	}

	return false;
}


//ライジングステップ
void CPlayer::StepRising()
{
	if (!m_isRising) { return; }

}

void CPlayer::StepStartPhase()
{
	m_animeInfo.UpdateAnime();
}
void CPlayer::StepFinPhase()
{
	m_animeInfo.UpdateAnime();
}

void CPlayer::ChangeAnime(PlayerAnimeKind kind)
{
	m_animeInfo.ChangeAnime(kind);
}

void CPlayer::ExecIdle()
{
	//移動量を０に
	SetZeroMoveXY();

	if (m_attackKind != PLAYER_ATTACK_NONE)m_attackKind = PLAYER_ATTACK_NONE;

	//MP回復処理
	HealingMP();

	//入力処理
	InputPlayer();
	
}
void CPlayer::ExecWalk()
{
	//MP回復処理
	HealingMP();

	//入力処理
	InputPlayer();
}
void CPlayer::ExecRun()
{
	//MP回復処理
	HealingMP();

	//入力処理
	InputPlayer();
}
void CPlayer::ExecAvoid()
{
	//MP回復処理
	HealingMP();

	if (m_animeInfo.IsBetweenFrame(AVOID_COLL_MIN, AVOID_COLL_MAX))
	{
		m_isAvoid = true;
	}
	else
	{
		m_isAvoid = false;
	}

	//入力処理
	InputPlayer();

	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_IDLE);
		m_state = PLAYER_STATE_IDLE;
		m_isAvoid = false;
	}
}
void CPlayer::ExecAttack()
{
	(this->*ExecAttackFunc[m_attackKind])();

	//入力処理
	InputPlayer();

	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_IDLE);
		m_state = PLAYER_STATE_IDLE;
		m_attackKind = PLAYER_ATTACK_NONE;
		m_isFloating = false;
		m_boostComboCount = 0;
	}
}
void CPlayer::ExecNormalAttack1()
{
	//移動量を０に
	SetZeroMoveXY();

	if (m_animeInfo.m_currentFrame < NORMAL_ATTACK1_MOVE_FRAME)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 1.5f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 1.5f;

		LookLeftStick();
		SetKeyRot();

	}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_ATTACK1_COLLISION_FRAME))
	{
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}

	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(NORMAL_ATTACK1_MOVE_FRAME))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F - ONE_DEGREE * 10.f), 0, cosf(m_rot.y + DX_PI_F - ONE_DEGREE * 10.f) };
		add = CMyMath::VecScale(add, 30.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y -= 8.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD1], attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f;
		effect_rot.x -= DX_PI_F * 45.f / 180.f;
		effect_rot.z += ONE_DEGREE * 10.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetScale(handle, VGet(1.3f, 1.3f, 1.3f));
		CEffekseerCtrl::SetSpeed(handle, 1.2f);
	}
}
void CPlayer::ExecNormalAttack2()
{
	//移動量を０に
	SetZeroMoveXY();
	if (m_animeInfo.m_currentFrame < 7)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 1.5f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 1.5f;

		LookLeftStick();
		SetKeyRot();
	}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_ATTACK2_COLLISION_FRAME))
	{
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}

	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(7))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 40.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y += 20.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD1],
			attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f;
		effect_rot.x -= DX_PI_F * 230.f / 180.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetSpeed(handle, 1.3f);
	}
}
void CPlayer::ExecNormalAttack3()
{
	//移動量を０に
	SetZeroMoveXY();
	if (m_animeInfo.m_currentFrame < 14)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 1.0f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 1.0f;

		LookLeftStick();
		SetKeyRot();
	}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_ATTACK3_COLLISION_FRAME))
	{
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}

	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(14))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F + ONE_DEGREE * 30.f), 0, cosf(m_rot.y + DX_PI_F + ONE_DEGREE * 30.f) };
		add = CMyMath::VecScale(add, 40.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y += 0.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD1],
			attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f + ONE_DEGREE * 20.f;
		effect_rot.x += DX_PI_F / 2.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetSpeed(handle, 1.3f);
		CEffekseerCtrl::SetScale(handle, VGet(1.8f, 1.8f, 1.8f));
	}
}
void CPlayer::ExecNormalAttack4()
{
	if (m_animeInfo.IsBetweenFrame(0.f, 17.f) ||
		m_animeInfo.IsBetweenFrame(22.f, 26.f) ||
		m_animeInfo.IsBetweenFrame(40.f, 70.f))
	{
		LookLeftStick();
		SetKeyRot();
	}

	//移動量を０に
	SetZeroMoveXY();
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 4.f / 5.f)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 1.8f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 1.8f;
	}
	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_ATTACK4_COLLISION_FRAME))
	{
		m_boostComboCount = 0;
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}
	else if (m_animeInfo.IsSpecificFrame(38))
	{
		m_boostComboCount = 1;
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}
	else if (m_animeInfo.IsSpecificFrame(75))
	{
		m_boostComboCount = 2;
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}

	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(18))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F - ONE_DEGREE * 10.f), 0, cosf(m_rot.y + DX_PI_F - ONE_DEGREE * 10.f) };
		add = CMyMath::VecScale(add, 50.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y -= 8.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD1], attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f;
		effect_rot.x -= DX_PI_F * 45.f / 180.f;
		effect_rot.z += ONE_DEGREE * 10.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetScale(handle, VGet(2.1f, 2.1f, 1.1f));
		CEffekseerCtrl::SetSpeed(handle, 1.2f);
	}
	if (m_animeInfo.IsSpecificFrame(27))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 60.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y += 20.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD1],
			attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f + ONE_DEGREE * 10.f;
		effect_rot.x -= DX_PI_F * 230.f / 180.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetScale(handle, VGet(2.1f, 2.1f, 1.1f));
		CEffekseerCtrl::SetSpeed(handle, 1.0f);
	}
	if (m_animeInfo.IsSpecificFrame(72))
	{

		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 50.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y += 0.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_HIT1],
			attack_center_pos, false);

		CEffekseerCtrl::SetScale(handle, VGet(2.1f, 4.1f, 2.1f));
		CEffekseerCtrl::SetSpeed(handle, 1.0f);
	}
}


void CPlayer::ExecNormalAttack5()
{
	//移動量を０に
	SetZeroMoveXY();

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 2.f / 5.f)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 0.6f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 0.6f;
	}

	/*if (m_animeInfo.IsSpecificFrame(23.f))
	{
		m_animeInfo.SetReproductionSpeed(0.5f);
	}*/

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(30) || 
		m_animeInfo.IsSpecificFrame(50) )
	{
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}

	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(15))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F - ONE_DEGREE * 50.f), 0, cosf(m_rot.y + DX_PI_F - ONE_DEGREE * 50.f) };
		add = CMyMath::VecScale(add, 30.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y += 0.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD2],
			attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f - ONE_DEGREE * 20.f;
		effect_rot.x += DX_PI_F / 2.f + DX_PI_F;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetSpeed(handle, 1.2f);
		CEffekseerCtrl::SetScale(handle, VGet(2.8f, 2.8f, 0.5f));
	}

	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(40))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F + ONE_DEGREE * 30.f), 0, 
			cosf(m_rot.y + DX_PI_F + ONE_DEGREE * 30.f) };
		add = CMyMath::VecScale(add, 30.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y += 0.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD2],
			attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f + ONE_DEGREE * 50.f;
		effect_rot.x -= DX_PI_F / 2.f + DX_PI_F;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetSpeed(handle, 1.2f);
		CEffekseerCtrl::SetScale(handle, VGet(2.8f, 2.8f, 0.5f));
	}
}
void CPlayer::ExecNormalAttack6()
{
	//移動量を０に
	SetZeroMoveXY();

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 4.f / 5.f)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 1.3f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 1.3f;
	}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_ATTACK7_COLLISION_FRAME))
	{
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}


	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(11))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F + ONE_DEGREE * 40.f), 0, cosf(m_rot.y + DX_PI_F + ONE_DEGREE * 40.f) };
		add = CMyMath::VecScale(add, 60.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y += 20.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD2], attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f;
		effect_rot.x -= DX_PI_F * 70.f / 180.f + DX_PI_F;
		effect_rot.z += ONE_DEGREE * 10.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetScale(handle, VGet(3.3f, 3.3f, 0.5f));
		CEffekseerCtrl::SetSpeed(handle, 1.3f);
	}

	////移動量を０に
	//SetZeroMoveXY();
	//
	//if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 2.f / 5.f)
	//{
	//	//x,zの移動量を決める
	//	m_move.x = sinf(m_rot.y + DX_PI_F) * 1.8f;
	//	m_move.z = cosf(m_rot.y + DX_PI_F) * 1.8f;
	//}
	//
	////攻撃
	//if (m_animeInfo.IsSpecificFrame(SWORD_ATTACK6_COLLISION_FRAME ))
	//{
	//	m_isAttack = true;
	//}
	//
	//
	////エフェクト判定
	//if (m_animeInfo.IsSpecificFrame(14))
	//{
	//	VECTOR attack_center_pos = GetCenterPos();
	//	VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
	//	add = CMyMath::VecScale(add, 20.f);
	//	attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
	//
	//	VECTOR effect_rot = m_rot;
	//	effect_rot.y += DX_PI_F;
	//
	//	//エフェクトリクエスト
	//	m_kickEffectID = CEffekseerCtrl::Request2(m_effectID[PLAYER_EFFECT_KIND_SONIC_BOOM], attack_center_pos, 50 ,false);
	//
	//	CEffekseerCtrl::SetScale(m_kickEffectID, VGet(0.5f, 0.5f, 0.15f));
	//	CEffekseerCtrl::SetRot(m_kickEffectID, effect_rot);
	//	CEffekseerCtrl::SetSpeed(m_kickEffectID, 2.2f);
	//}
}
void CPlayer::ExecNormalAttack7()
{

	//移動量を０に
	SetZeroMoveXY();
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 4.f / 5.f)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 1.3f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 1.3f;
	}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(20))
	{
		m_boostComboCount = 0;
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}
	else if (m_animeInfo.IsSpecificFrame(40))
	{
		m_boostComboCount = 1;
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}
	//ガブリアス
	else if (m_animeInfo.IsSpecificFrame(66))
	{
		m_boostComboCount = 2;
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_SWORD1);
	}

	//エフェクト判定
	if (m_animeInfo.IsSpecificFrame(10))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 20.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y += 20.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD1],
			attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f;
		effect_rot.x -= DX_PI_F * 145.f / 180.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetScale(handle, VGet(2.2f, 2.2f, 0.5f));
		CEffekseerCtrl::SetSpeed(handle, 1.3f);
	}
	if (m_animeInfo.IsSpecificFrame(25))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 30.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y -= 8.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD1], attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f;
		effect_rot.x += DX_PI_F * 45.f / 180.f;
		effect_rot.z -= ONE_DEGREE * 30.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetScale(handle, VGet(2.2f, 2.2f, 0.6f));
		CEffekseerCtrl::SetSpeed(handle, 1.2f);
	}
	if (m_animeInfo.IsSpecificFrame(45))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 38.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y -= 8.f;

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD1], attack_center_pos, false);

		VECTOR effect_rot = m_rot;
		effect_rot.y += DX_PI_F / 2.f;
		effect_rot.x -= DX_PI_F * 45.f / 180.f;
		effect_rot.z -= ONE_DEGREE * 30.f;

		CEffekseerCtrl::SetRot(handle, effect_rot);
		CEffekseerCtrl::SetScale(handle, VGet(2.0f, 2.0f, 0.6f));
		CEffekseerCtrl::SetSpeed(handle, 1.2f);
	}

	////移動量を０に
	//SetZeroMoveXY();
	//
	//if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 4.f / 5.f)
	//{
	//	//x,zの移動量を決める
	//	m_move.x = sinf(m_rot.y + DX_PI_F) * 1.3f;
	//	m_move.z = cosf(m_rot.y + DX_PI_F) * 1.3f;
	//}
	//
	////攻撃判定
	//if (m_animeInfo.IsSpecificFrame(SWORD_ATTACK7_COLLISION_FRAME))
	//{
	//	m_isAttack = true;
	//
	//	CSoundManager::PlaySE(SE_KIND_SWORD1);
	//}//
	//
	//
	////エフェクト判定
	//if (m_animeInfo.IsSpecificFrame(11))
	//{
	//	VECTOR attack_center_pos = GetCenterPos();
	//	VECTOR add = { sinf(m_rot.y + DX_PI_F + ONE_DEGREE * 40.f), 0, cosf(m_rot.y + DX_PI_F + ONE_DEGREE * 40.f) };
	//	add = CMyMath::VecScale(add, 60.f);
	//	attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
	//	attack_center_pos.y += 20.f;
	//
	//	//エフェクトリクエスト
	//	int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_SWORD2], attack_center_pos, false);
	//
	//	VECTOR effect_rot = m_rot;
	//	effect_rot.y += DX_PI_F / 2.f;
	//	effect_rot.x -= DX_PI_F * 70.f / 180.f + DX_PI_F;
	//	effect_rot.z += ONE_DEGREE * 10.f;
	//
	//	CEffekseerCtrl::SetRot(handle, effect_rot);
	//	CEffekseerCtrl::SetScale(handle, VGet(3.3f, 3.3f, 0.5f));
	//	CEffekseerCtrl::SetSpeed(handle, 1.3f);
	//}
}
void CPlayer::ExecNormalAttack8() 
{
	//移動量を０に
	SetZeroMoveXY();

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 2.f / 5.f)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 1.0f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 1.0f;
	}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_ATTACK3_COLLISION_FRAME))
	{
		m_isAttack = true;
	}
}
void CPlayer::ExecNormalAttack9()
{
	//移動量を０に
	SetZeroMoveXY();
}

void CPlayer::ExecStrongAttack1()
{
	//移動量を０に
	SetZeroMoveXY();

	//if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 2.f / 3.f)
	//{
	//	//x,zの移動量を決める
	//	m_move.x = sinf(m_rot.y + DX_PI_F) * 3.0f;
	//	m_move.z = cosf(m_rot.y + DX_PI_F) * 3.0f;
	//}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_STRONG_ATTACK1_COLLISION_FRAME))
	{
		m_isAttack = true;

		CSoundManager::PlaySE(SE_KIND_ATTACK3);
	}

	if (m_animeInfo.IsSpecificFrame(22))
	{
		m_animeInfo.SetReproductionSpeed(3.f);
	}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_STRONG_ATTACK1_COLLISION_FRAME))
	{
		VECTOR attack_center_pos = GetAttackCenterPos();

		//エフェクトリクエスト
		int handle = CEffekseerCtrl::Request2(m_effectID[PLAYER_EFFECT_KIND_BIG_FIRE],
			attack_center_pos, 111, false);
		CEffekseerCtrl::SetSpeed(handle, 2.f);
		CEffekseerCtrl::SetScale(handle, VGet(1.5f, 1.5f, 1.5f));
	}
}
void CPlayer::ExecStrongAttack2()
{
	//移動量を０に
	SetZeroMoveXY();

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_STRONG_ATTACK2_COLLISION_FRAME))
	{
		m_isAttack = true;
	}
}
void CPlayer::ExecStrongAttack3()
{
	//移動量を０に
	SetZeroMoveXY();

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 4.f / 5.f)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 3.f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 3.f;
	}

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(SWORD_STRONG_ATTACK3_COLLISION_FRAME))
	{
		m_isAttack = true;
	}
}



void CPlayer::ExecRisingAttack()
{

	if (m_animeInfo.IsSpecificFrame(9.f))
	{
		m_animeInfo.SetReproductionSpeed(0.22f);
		CSoundManager::PlaySE(SE_KIND_SWORD_SUPER);
	}
	if (m_animeInfo.IsSpecificFrame(14.f))
	{
		m_animeInfo.SetReproductionSpeed(2.5f);
	}


	if (m_animeInfo.m_currentFrame < 22.f) 
	{ 
		LookLeftStick();
		SetKeyRot();
		return; 
	}

	if (m_animeInfo.IsSpecificFrame(22.f))
	{
		float vec_len = CMyMath::VecLong(VGet(sinf(m_rot.y + DX_PI_F), 0.f, cosf(m_rot.y + DX_PI_F)));

		MATRIX all_mat[4] =
		{
			CMyMath::GetTranslateMatrix(VGet(0.f, 0.f, 15.f)),
			CMyMath::GetPitchMatrix(m_rot.x),
			CMyMath::GetYawMatrix(m_rot.y),
			CMyMath::GetRollMatrix(m_rot.z),
		};

		MATRIX result = all_mat[3];
		for (int mat_index = 3; mat_index >= 0; --mat_index)
			result = CMyMath::MatMult(result, all_mat[mat_index]);

		//反映
		m_risingMove.x -= result.m[0][3] * vec_len;
		m_risingMove.z -= result.m[2][3] * vec_len;

		VECTOR nor_move = CMyMath::VecNormalize(m_risingMove);
		m_risingMove = CMyMath::VecScale(nor_move, RISING_MAX_LEN);


		m_risingPos = VAdd(m_risingPos, m_move);

		//プレイヤーからどれだけはなれているかで足す
		VECTOR player_pos = GetCenterPos();
		m_risingPos = CMyMath::VecAdd(player_pos, m_risingMove);

		//エフェクト先に計算
		VECTOR effect_pos = CMyMath::VecAdd(m_pos, CMyMath::VecScale(m_risingMove, 0.5f));
		effect_pos.y += 20.f;
		int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_HIT1],
			effect_pos, false);
		CEffekseerCtrl::SetSpeed(handle, 0.6f);
		CEffekseerCtrl::SetScale(handle, VGet(9.f, 0.3f, 0.1f));

		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * CMyMath::VecLong(m_risingMove)/3.f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * CMyMath::VecLong(m_risingMove)/3.f;
	}

	m_move = CMyMath::VecScale(m_move, 0.8f);
	float len = CMyMath::VecLong(m_pos, m_risingPos);
	if (len <= 20.f) 
	{ 
		m_pos = m_risingPos; 
		SetZeroMoveXY();
	}
	if (CMyMath::VecLong(m_move) < 1.f)
	{
		SetZeroMoveXY();
	}

	
	if (m_paramB == false)
	{
		VECTOR sa = CMyMath::VecCreate(m_risingStartPos, m_risingPos);
		VECTOR between_pos = CMyMath::VecAdd(m_risingStartPos, CMyMath::VecScale(sa, 0.8f));
		VECTOR old_pos = m_oldPos;
		VECTOR current_pos = CMyMath::VecAdd(m_pos, m_move);

		float old_len = CMyMath::VecLong(between_pos, old_pos);
		float current_len = CMyMath::VecLong(between_pos, current_pos);

		if (old_len < current_len)
		{
			m_isRisingAttack = true;
			m_paramB = true;
		}
	}
	

	if (m_animeInfo.IsEndAnime())
	{
		ResetRising();
		m_paramB = false;

	}
}




void CPlayer::ExecHit()
{
	m_move = ZERO_VECTOR;

	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		//無敵になる
		if (!m_isInvincible)
		{
			m_isInvincible = true;

			//drawフラグを折る
			m_isDraw = false;
		}

		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_IDLE);
		m_state = PLAYER_STATE_IDLE;
	}
}
void CPlayer::ExecKnockBack()
{
	//移動量を０に
	SetZeroMoveXY();

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 2.f / 5.f)
	{
		//x,zの移動量を決める
		m_move.x = -sinf(m_rot.y + DX_PI_F) * 4.0f;
		m_move.z = -cosf(m_rot.y + DX_PI_F) * 4.0f;

		return;
	}

	if (m_hp <= 0)
	{

	}

	//アニメが終了したら起き上がりへ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_GET_UP);
		m_state = PLAYER_STATE_GET_UP;
	}
}
void CPlayer::ExecGetUp()
{

	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		//無敵になる
		if (!m_isInvincible)
		{
			m_isInvincible = true;

			//drawフラグを折る
			m_isDraw = false;
		}

		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_IDLE);
		m_state = PLAYER_STATE_IDLE;
	}
}
void CPlayer::ExecDown()
{
	//入力処理
	InputPlayer();
}

void CPlayer::InputIdle()
{
	IFWalkAndRun();

	IFAvoid();

	IFNormalAttack();
	IFNormalAttack2();

	IFStrongAttack();
	IFRisingAttack();

	IFBoost();
}
void CPlayer::InputWalk()
{
	IFWalkAndRun();

	IFAvoid();

	IFNormalAttack();
	IFNormalAttack2();

	IFStrongAttack();
	IFRisingAttack();
	
	IFBoost();
}
void CPlayer::InputRun()
{
	IFWalkAndRun();

	IFAvoid();

	IFNormalAttack();
	IFNormalAttack2();

	IFStrongAttack();
	IFRisingAttack();

	IFBoost();
}
void CPlayer::InputAvoid()
{

}
void CPlayer::InputAttack()
{
	(this->*InputAttackFunc[m_attackKind])();
}
void CPlayer::InputNormalAttack1()
{
	//半分くらいまでアニメしたら次の攻撃へ
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 3.f / 7.f)
		return;

	bool is_attack = CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_X, I_PAD);
	bool is_attack2 = CInputManager::IsPush(MOUSE_INPUT_RIGHT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_Y, I_PAD);

	//攻撃2へ
	if (is_attack)
	{
		m_state = PLAYER_STATE_ATTACK;
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SWORD_ATTACK2);
		m_attackKind = PLAYER_ATTACK_NORMAL2;


		//攻撃していた時にスティックが入力されていたらそっち向きに
		LookLeftStick();

		//一番近い敵のむきへ
		//FacingNearEnemy();

		SetKeyRot();

		MemoryCombo(2);
	}
	//攻撃5へ
	if (is_attack2)
	{
		if (CheckMP(PLAYER_NORMAL_ATTACK5_MP))
		{
			//一番近い敵のむきへ
			//FacingNearEnemy();

			SetKeyRot();

			SetZeroMoveXY();
			m_state = PLAYER_STATE_ATTACK;
			m_attackKind = PLAYER_ATTACK_NORMAL5;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SIDE2_ATTACK, 13);

			MemoryCombo(5);
			SubMP(PLAYER_NORMAL_ATTACK5_MP);
		}


		//if (CheckMP(PLAYER_NORMAL_ATTACK7_MP))
		//{
		//	m_state = PLAYER_STATE_ATTACK;
		//	m_attackKind = PLAYER_ATTACK_NORMAL7;
		//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3SLASH_1);
		//
		//	//攻撃していた時にスティックが入力されていたらそっち向きに
		//	LookLeftStick();
		//
		//	//一番近い敵のむきへ
		//	//FacingNearEnemy();
		//
		//	SetKeyRot();
		//
		//	MemoryCombo(2);
		//	SubMP(PLAYER_NORMAL_ATTACK7_MP);
		//}
		//
		//if (CheckMP(PLAYER_NORMAL_ATTACK6_MP))
		//{
		//	m_state = PLAYER_STATE_ATTACK;
		//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_KICK, 10);
		//	m_attackKind = PLAYER_ATTACK_NORMAL6;
		//
		//
		//	//攻撃していた時にスティックが入力されていたらそっち向きに
		//	LookLeftStick();
		//
		//	//一番近い敵のむきへ
		//	FacingNearEnemy();
		//
		//SetKeyRot();
		//
		//	MemoryCombo(2);
		//	SubMP(PLAYER_NORMAL_ATTACK6_MP);
		//}
	}

	//いろいろできるようになる
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 5.f / 7.f)
		return;

	IFStrongAttack();
	IFRisingAttack();
	IFAvoid();
	IFBoost();

}
void CPlayer::InputNormalAttack2()
{
	//半分くらいまでアニメしたら次の攻撃へ
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 3.5f / 7.f)
		return;

	bool is_attack = CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_X, I_PAD);
	bool is_attack2 = CInputManager::IsPush(MOUSE_INPUT_RIGHT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_Y, I_PAD);

	//攻撃3へ
	if (is_attack)
	{
		m_state = PLAYER_STATE_ATTACK;
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SWORD_ATTACK3);
		m_attackKind = PLAYER_ATTACK_NORMAL3;


		//攻撃していた時にスティックが入力されていたらそっち向きに
		LookLeftStick();

		//一番近い敵のむきへ
		//FacingNearEnemy();

		SetKeyRot();

		MemoryCombo(3);
	}

	//攻撃6へ
	if (is_attack2)
	{
		if (m_boostKind != PLAYER_BOOST_NOW)
		{
			if (CheckMP(PLAYER_NORMAL_ATTACK6_MP))
			{
				m_state = PLAYER_STATE_ATTACK;
				m_attackKind = PLAYER_ATTACK_NORMAL6;
				m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_ROT_UP_ATTACK);

				//攻撃していた時にスティックが入力されていたらそっち向きに
				LookLeftStick();

				//一番近い敵のむきへ
				//FacingNearEnemy();

				SetKeyRot();

				MemoryCombo(6);
				SubMP(PLAYER_NORMAL_ATTACK7_MP);
			}
		}
		//７へ
		else
		{
			if (CheckMP(PLAYER_NORMAL_ATTACK7_MP))
			{
				m_state = PLAYER_STATE_ATTACK;
				m_attackKind = PLAYER_ATTACK_NORMAL7;
				m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3SLASH_1);

				//攻撃していた時にスティックが入力されていたらそっち向きに
				LookLeftStick();

				//一番近い敵のむきへ
				//FacingNearEnemy();

				SetKeyRot();

				MemoryCombo(7);
				SubMP(PLAYER_NORMAL_ATTACK7_MP);
			}
		}
	}

	//いろいろできるようになる
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 5.5f / 7.f)
		return;

	IFStrongAttack();
	IFRisingAttack();
	IFAvoid();
	IFBoost();

}
void CPlayer::InputNormalAttack3()
{
	//いろいろできるようになる
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 5.f / 7.f)
		return;

	bool is_attack = CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_X, I_PAD);
	bool is_attack2 = CInputManager::IsPush(MOUSE_INPUT_RIGHT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_Y, I_PAD);


	IFStrongAttack();
	IFRisingAttack();
	IFAvoid();
	IFBoost();

	//ブースト中なら7へ
	if (m_boostKind == PLAYER_BOOST_NOW)
	{
		if (is_attack2)
		{
			if (CheckMP(PLAYER_NORMAL_ATTACK7_MP))
			{
				m_state = PLAYER_STATE_ATTACK;
				m_attackKind = PLAYER_ATTACK_NORMAL7;
				m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3SLASH_1);

				//攻撃していた時にスティックが入力されていたらそっち向きに
				LookLeftStick();

				//一番近い敵のむきへ
				//FacingNearEnemy();

				SetKeyRot();

				MemoryCombo(7);
				SubMP(PLAYER_NORMAL_ATTACK7_MP);
			}
		}
	}
	//記憶されてなかったら6へ
	else if (!IsMemoryCombo(6))
	{
		if (is_attack2)
		{
			if (CheckMP(PLAYER_NORMAL_ATTACK6_MP))
			{
				m_state = PLAYER_STATE_ATTACK;
				m_attackKind = PLAYER_ATTACK_NORMAL6;
				m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_ROT_UP_ATTACK);

				//攻撃していた時にスティックが入力されていたらそっち向きに
				LookLeftStick();

				//一番近い敵のむきへ
				//FacingNearEnemy();

				SetKeyRot();

				MemoryCombo(6);
				SubMP(PLAYER_NORMAL_ATTACK6_MP);
			}
		}
	}

	//攻撃4へ
	if (m_boostKind != PLAYER_BOOST_NOW) { return; }
	
	if (is_attack)
	{
		m_state = PLAYER_STATE_ATTACK;
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3COMBO_ATTACK);
		m_attackKind = PLAYER_ATTACK_NORMAL4;


		//攻撃していた時にスティックが入力されていたらそっち向きに
		LookLeftStick();

		//一番近い敵のむきへ
		//FacingNearEnemy();

		MemoryCombo(4);
		SetKeyRot();
	}
	
}
void CPlayer::InputNormalAttack4()
{
	//いろいろできるようになる
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 6.f / 7.f)
		return;

	bool is_attack2 = CInputManager::IsPush(MOUSE_INPUT_RIGHT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_Y, I_PAD);

	IFStrongAttack();
	IFRisingAttack();
	IFAvoid();
	IFBoost();

	if (m_boostKind != PLAYER_BOOST_NOW) { return; }

	//記憶されてなかったら攻撃７へ
	if (is_attack2)
	{
		if (!IsMemoryCombo(7))
		{
			if (CheckMP(PLAYER_NORMAL_ATTACK7_MP))
			{
				m_state = PLAYER_STATE_ATTACK;
				m_attackKind = PLAYER_ATTACK_NORMAL7;
				m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3SLASH_1);

				//攻撃していた時にスティックが入力されていたらそっち向きに
				LookLeftStick();

				//一番近い敵のむきへ
				//FacingNearEnemy();

				SetKeyRot();

				MemoryCombo(7);
				SubMP(PLAYER_NORMAL_ATTACK7_MP);
			}
		}
	}
}

void CPlayer::InputNormalAttack5()
{
	//半分くらいまでアニメしたら次の攻撃へ
	if (m_animeInfo.m_currentFrame < 64.f)
		return;

	bool is_attack = CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_X, I_PAD);
	bool is_attack2 = CInputManager::IsPush(MOUSE_INPUT_RIGHT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_Y, I_PAD);


	//記憶されてなかったら攻撃2へ
	//されてたら３へ
	if (is_attack)
	{
		if (m_comboMemory[0] == 1 && m_comboMemory[1] == 5)
		{
			m_state = PLAYER_STATE_ATTACK;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SWORD_ATTACK2);
			m_attackKind = PLAYER_ATTACK_NORMAL2;


			//攻撃していた時にスティックが入力されていたらそっち向きに
			LookLeftStick();

			//一番近い敵のむきへ
			//FacingNearEnemy();

			SetKeyRot();

			MemoryCombo(2);
		}
		else if (m_comboMemory[0] == 1 && m_comboMemory[1] == 2 &&
			m_comboMemory[2] == 5)
		{
			m_state = PLAYER_STATE_ATTACK;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SWORD_ATTACK3);
			m_attackKind = PLAYER_ATTACK_NORMAL3;

			//攻撃していた時にスティックが入力されていたらそっち向きに
			LookLeftStick();

			//一番近い敵のむきへ
			//FacingNearEnemy();

			SetKeyRot();

			MemoryCombo(3);
		}
		//2へ
		else if (m_comboMemory[0] == 5 && m_comboMemory[1] == 0)
		{
			m_state = PLAYER_STATE_ATTACK;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SWORD_ATTACK2);
			m_attackKind = PLAYER_ATTACK_NORMAL2;


			//攻撃していた時にスティックが入力されていたらそっち向きに
			LookLeftStick();

			//一番近い敵のむきへ
			//FacingNearEnemy();

			SetKeyRot();

			MemoryCombo(2);
		}
	}

	//攻撃6へ
	if (is_attack2)
	{
		if (CheckMP(PLAYER_NORMAL_ATTACK6_MP))
		{
			m_state = PLAYER_STATE_ATTACK;
			m_attackKind = PLAYER_ATTACK_NORMAL6;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_ROT_UP_ATTACK);

			//攻撃していた時にスティックが入力されていたらそっち向きに
			LookLeftStick();

			//一番近い敵のむきへ
			//FacingNearEnemy();

			SetKeyRot();

			MemoryCombo(6);
			SubMP(PLAYER_NORMAL_ATTACK6_MP);
		}

		//if (CheckMP(PLAYER_NORMAL_ATTACK6_MP))
		//{
		//	m_state = PLAYER_STATE_ATTACK;
		//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_KICK, 10);
		//	m_attackKind = PLAYER_ATTACK_NORMAL6;
		//
		//
		//	//攻撃していた時にスティックが入力されていたらそっち向きに
		//	LookLeftStick();
		//
		//	//一番近い敵のむきへ
		//	FacingNearEnemy();
		//
		//S/etKeyRot();
		//
		//	MemoryCombo(2);
		//	SubMP(PLAYER_NORMAL_ATTACK6_MP);
		//}
	}

	//いろいろできるようになる
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 5.5f / 7.f)
		return;

	IFStrongAttack();
	IFRisingAttack();
	IFAvoid();
	IFBoost();
}
void CPlayer::InputNormalAttack6() 
{
	//いろいろできるようになる
	if (m_animeInfo.m_currentFrame < 41.5f)
		return;

	bool is_attack = CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_X, I_PAD);
	bool is_attack2 = CInputManager::IsPush(MOUSE_INPUT_RIGHT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_Y, I_PAD);


	//記憶してなっかたら攻撃３へ
	if (is_attack)
	{
		if (!IsMemoryCombo(3))
		{
			m_state = PLAYER_STATE_ATTACK;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SWORD_ATTACK3);
			m_attackKind = PLAYER_ATTACK_NORMAL3;


			//攻撃していた時にスティックが入力されていたらそっち向きに
			LookLeftStick();

			//一番近い敵のむきへ
			//FacingNearEnemy();

			SetKeyRot();

			MemoryCombo(3);
		}
	}

	//ブースト中なら攻撃7へ
	if (m_boostKind != PLAYER_BOOST_NOW) { return; }

	if (is_attack2)
	{
		if (CheckMP(PLAYER_NORMAL_ATTACK7_MP))
		{
			m_state = PLAYER_STATE_ATTACK;
			m_attackKind = PLAYER_ATTACK_NORMAL7;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3SLASH_1);

			//攻撃していた時にスティックが入力されていたらそっち向きに
			LookLeftStick();

			//一番近い敵のむきへ
			//FacingNearEnemy();

			SetKeyRot();

			MemoryCombo(7);
			SubMP(PLAYER_NORMAL_ATTACK7_MP);
		}
	}



	//if (m_boostKind == PLAYER_BOOST_NOW)
	//{
	//	if (is_attack)
	//	{
	//		if (CheckMP(PLAYER_NORMAL_ATTACK8_MP))
	//		{
	//			m_state = PLAYER_STATE_ATTACK;
	//			m_attackKind = PLAYER_ATTACK_NORMAL8;
	//			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_2COMBO_ATTACK);
	//
	//			//攻撃していた時にスティックが入力されていたらそっち向きに
	//			LookLeftStick();
	//
	//			//一番近い敵のむきへ
	//			//FacingNearEnemy();
	//
	//			SetKeyRot();
	//
	//			MemoryCombo(2);
	//			SubMP(PLAYER_NORMAL_ATTACK8_MP);
	//		}
	//	}
	//}

	IFStrongAttack();
	IFRisingAttack();
	IFAvoid();
	IFBoost();

	////半分くらいまでアニメしたら次の攻撃へ
	//if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 4.0f / 7.f)
	//	return;
	//
	//bool is_attack = CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_X, I_PAD);
	//bool is_attack2 = CInputManager::IsPush(MOUSE_INPUT_RIGHT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_Y, I_PAD);
	//
	////攻撃3へ
	//if (is_attack)
	//{
	//	m_state = PLAYER_STATE_ATTACK;
	//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SWORD_ATTACK3);
	//	m_attackKind = PLAYER_ATTACK_NORMAL3;
	//
	//
	//	//攻撃していた時にスティックが入力されていたらそっち向きに
	//	LookLeftStick();
	//
	//	//一番近い敵のむきへ
	//	//FacingNearEnemy();
	//
	//	SetKeyRot();
	//
	//	MemoryCombo(1);
	//
	//}
	//
	////攻撃7へ
	//if (is_attack2)
	//{
	//	if (CheckMP(PLAYER_NORMAL_ATTACK7_MP))
	//	{
	//		m_state = PLAYER_STATE_ATTACK;
	//		m_attackKind = PLAYER_ATTACK_NORMAL7;
	//		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3SLASH_1);
	//
	//		//攻撃していた時にスティックが入力されていたらそっち向きに
	//		LookLeftStick();
	//
	//		//一番近い敵のむきへ
	//		//FacingNearEnemy();
	//
	//		SetKeyRot();
	//
	//		MemoryCombo(2);
	//		SubMP(PLAYER_NORMAL_ATTACK7_MP);
	//	}
	//}
	//
	////いろいろできるようになる
	//if (m_animeInfo.m_currentFrame < 36.f)
	//	return;
	//
	//IFStrongAttack();
	//IFRisingAttack();
	//IFAvoid();
	//IFBoost();

}
void CPlayer::InputNormalAttack7()
{
	//いろいろできるようになる
	if (m_animeInfo.m_currentFrame < 80.5f)
		return;

	bool is_attack = CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_X, I_PAD);



	//攻撃8へ
	//if (m_boostKind == PLAYER_BOOST_NOW)
	//{
	//	if (is_attack)
	//	{
	//		if (CheckMP(PLAYER_NORMAL_ATTACK8_MP))
	//		{
	//			m_state = PLAYER_STATE_ATTACK;
	//			m_attackKind = PLAYER_ATTACK_NORMAL8;
	//			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_2COMBO_ATTACK);
	//
	//			//攻撃していた時にスティックが入力されていたらそっち向きに
	//			LookLeftStick();
	//
	//			//一番近い敵のむきへ
	//			//FacingNearEnemy();
	//
	//			SetKeyRot();
	//
	//			MemoryCombo(2);
	//			SubMP(PLAYER_NORMAL_ATTACK8_MP);
	//		}
	//	}
	//}

	//記憶していなかったら４へ
	if (m_boostKind == PLAYER_BOOST_NOW)
	{
		if (!IsMemoryCombo(4))
		{
			if (is_attack)
			{
				m_state = PLAYER_STATE_ATTACK;
				m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3COMBO_ATTACK);
				m_attackKind = PLAYER_ATTACK_NORMAL4;


				//攻撃していた時にスティックが入力されていたらそっち向きに
				LookLeftStick();

				//一番近い敵のむきへ
				//FacingNearEnemy();

				MemoryCombo(4);
				SetKeyRot();
			}
		}
	}

	/*IFStrongAttack();
	IFRisingAttack();
	IFAvoid();
	IFBoost();*/

}
void CPlayer::InputNormalAttack8()
{
	//いろいろできるようになる
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 5.5f / 7.f)
		return;

	IFStrongAttack();
	IFRisingAttack();
	IFAvoid();
	IFBoost();
}
void CPlayer::InputNormalAttack9()
{

}


void CPlayer::InputStrongAttack1()
{

}
void CPlayer::InputStrongAttack2()
{

}
void CPlayer::InputStrongAttack3()
{

}


void CPlayer::InputRisingAttack()
{

}


void CPlayer::InputHit()
{

}
void CPlayer::InputKnockBack()
{

}
void CPlayer::InputGetUp()
{

}
void CPlayer::InputDown()
{

}


//重力処理
void CPlayer::StepGravity()
{
	m_move.y -= GRAVITY;

	if (m_isFloating && m_move.y <= 0.f) { m_move.y = 0.f; }

	if (m_move.y <= -MAX_GRAVITY) { m_move.y = -MAX_GRAVITY; }
}



void CPlayer::IFWalkAndRun()
{
	bool is_right = CInputManager::IsDown(KEY_INPUT_D, I_KEY);
	bool is_left = CInputManager::IsDown(KEY_INPUT_A, I_KEY);
	bool is_up = CInputManager::IsDown(KEY_INPUT_W, I_KEY);
	bool is_down = CInputManager::IsDown(KEY_INPUT_S, I_KEY);

	PlayerAnimeKind anime_kind = PLAYER_ANIME_KIND_IDLE;

	VECTOR move = { 0.f, 0.f, 0.f };
	VECTOR nor_vec = { 0.f,0.f,0.f };

	//右
	if (is_right)
	{
		move.x = 1.f;
	}
	//左
	if (is_left)
	{
		move.x = -1.f;
	}
	//奥行
	if (is_up)
	{
		move.z = 1.f;
	}
	//手前
	if (is_down)
	{
		move.z = -1.f;
	}


	if (CPad::GetPad()->IsLeftStick())
	{
		//左スティック値で動く
		VECTOR left_stick = { CPad::GetPad()->GetStickBuf(LEFT_STICK_X),CPad::GetPad()->GetStickBuf(LEFT_STICK_Y), 0.f };

		float vec_len = CMyMath::VecLong(left_stick);

		//プレイヤーの向きをスティックの向きにする
		m_rot.y = CMyMath::VecDir(left_stick) + DX_PI_F + CCameraManager::GetManager()->GetPlayCameraRotY();

		MATRIX all_mat[4] =
		{
			CMyMath::GetTranslateMatrix(VGet(0.f, 0.f, PLAYER_SPEED)),
			CMyMath::GetPitchMatrix(m_rot.x),
			CMyMath::GetYawMatrix(m_rot.y),
			CMyMath::GetRollMatrix(m_rot.z),
		};

		MATRIX camera_mat = all_mat[3];
		for (int mat_index = 3; mat_index >= 0; --mat_index)
			camera_mat = CMyMath::MatMult(camera_mat, all_mat[mat_index]);

		//反映
		move.x = -camera_mat.m[0][3] * vec_len;
		move.z = -camera_mat.m[2][3] * vec_len;

		if (vec_len < -PLAYER_WALK_MAX || PLAYER_WALK_MAX < vec_len)
		{
			m_state = PLAYER_STATE_RUN;
			anime_kind = PLAYER_ANIME_KIND_RUN;
		}
		else
		{
			m_state = PLAYER_STATE_WALK;
			anime_kind = PLAYER_ANIME_KIND_WALK;
		}

	}
	else
	{
		m_state = PLAYER_STATE_IDLE;
		anime_kind = PLAYER_ANIME_KIND_IDLE;
	}

	if (is_right || is_left || is_up || is_down)
	{
		m_rot.y = CMyMath::VecDir(move.x, move.z) + DX_PI_F + CCameraManager::GetManager()->GetPlayCameraRotY();

		MATRIX all_mat[4] =
		{
			CMyMath::GetTranslateMatrix(VGet(0.f, 0.f, PLAYER_SPEED)),
			CMyMath::GetPitchMatrix(m_rot.x),
			CMyMath::GetYawMatrix(m_rot.y),
			CMyMath::GetRollMatrix(m_rot.z),
		};

		MATRIX camera_mat = all_mat[3];
		for (int mat_index = 3; mat_index >= 0; --mat_index)
			camera_mat = CMyMath::MatMult(camera_mat, all_mat[mat_index]);

		//反映
		move.x = -camera_mat.m[0][3];
		move.z = -camera_mat.m[2][3];

		move = CMyMath::VecNormalize(move);
		move = CMyMath::VecScale(move, PLAYER_SPEED);

		m_state = PLAYER_STATE_RUN;
		anime_kind = PLAYER_ANIME_KIND_RUN;
	}


#ifdef DEBUG_MODE
	assert(anime_kind != PLAYER_ANIME_KIND_NONE);
#endif

	m_animeInfo.ChangeAnime(anime_kind);
	SetMoveXY(move);
}
void CPlayer::IFAvoid()
{
	bool is_avoid = CInputManager::IsPush(KEY_INPUT_SPACE, I_KEY) || CInputManager::IsPush(KEY_INPUT_LSHIFT, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_A, I_PAD);

	//回避
	if (is_avoid)
	{
		LookLeftStick();

		SetKeyRot();

		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * PLAYER_AVOID_SPEED2;
		m_move.z = cosf(m_rot.y + DX_PI_F) * PLAYER_AVOID_SPEED2;

		m_state = PLAYER_STATE_AVOID;
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_AVOID);
	}
}
void CPlayer::IFNormalAttack()
{
	bool is_attack = CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_X, I_PAD);

	//攻撃
	if (is_attack)
	{
		//一番近い敵のむきへ
		//FacingNearEnemy();

		SetKeyRot();

		SetZeroMoveXY();
		m_state = PLAYER_STATE_ATTACK;
		m_attackKind = PLAYER_ATTACK_NORMAL1;
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SWORD_ATTACK1);

		InitCombo();
		MemoryCombo(1);
	}
}
void CPlayer::IFNormalAttack2()
{
	bool is_attack5 = CInputManager::IsPush(MOUSE_INPUT_RIGHT, I_MOUSE)|| CInputManager::IsPush(XINPUT_BUTTON_Y, I_PAD);
	/*bool is_attack6 = CInputManager::IsPush(KEY_INPUT_6, I_KEY);
	bool is_attack7 = CInputManager::IsPush(KEY_INPUT_7, I_KEY);
	bool is_attack8 = CInputManager::IsPush(KEY_INPUT_8, I_KEY);
	bool is_attack9 = CInputManager::IsPush(KEY_INPUT_9, I_KEY);*/

	//攻撃
	if (is_attack5)
	{
		if (CheckMP(PLAYER_NORMAL_ATTACK5_MP))
		{
			//一番近い敵のむきへ
			//FacingNearEnemy();

			SetKeyRot();

			SetZeroMoveXY();
			m_state = PLAYER_STATE_ATTACK;
			m_attackKind = PLAYER_ATTACK_NORMAL5;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_SIDE2_ATTACK);

			InitCombo();
			MemoryCombo(5);
			SubMP(PLAYER_NORMAL_ATTACK5_MP);
		}
	}
	//if (is_attack6)
	//{
	//	//一番近い敵のむきへ
	//	FacingNearEnemy();
	//
	//	SetZeroMoveXY();
	//	m_state = PLAYER_STATE_ATTACK;
	//	m_attackKind = PLAYER_ATTACK_NORMAL6;
	//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_KICK, 10);
	//
	//}
	//if (is_attack7)
	//{
	//	//一番近い敵のむきへ
	//	FacingNearEnemy();
	//
	//	SetZeroMoveXY();
	//	m_state = PLAYER_STATE_ATTACK;
	//	m_attackKind = PLAYER_ATTACK_NORMAL7;
	//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_ROT_UP_ATTACK);
	//}
	//if (is_attack8)
	//{
	//
	//}
	//if (is_attack9)
	//{
	//	//一番近い敵のむきへ
	//	FacingNearEnemy();
	//
	//	SetZeroMoveXY();
	//	m_state = PLAYER_STATE_ATTACK;
	//	m_attackKind = PLAYER_ATTACK_NORMAL9;
	//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_ROT_JUMP_ATTACK);
	//}
}

void CPlayer::IFStrongAttack()
{
	bool is_1_attack = CInputManager::IsPush(KEY_INPUT_Q, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_LEFT_SHOULDER, I_PAD);


	//１攻撃
	if (is_1_attack)
	{
		if (CheckMP(PLAYER_STRONG_ATTACK1_MP))
		{
			SetZeroMoveXY();
			m_state = PLAYER_STATE_ATTACK;
			m_attackKind = PLAYER_ATTACK_STRONG1;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_ROT_JUMP_ATTACK);

			SubMP(41.f);

			//カメラを近づける
			CCameraManager::GetManager()->ApproachPlayCamera(35, 1200);
		}
	}
	////2攻撃
	//if (is_2_attack)
	//{
	//	//if (CheckMP(PLAYER_STRONG_ATTACK2_MP))
	//	//{
	//	//	SetZeroMoveXY();
	//	//	m_state = PLAYER_STATE_ATTACK;
	//	//	m_attackKind = PLAYER_ATTACK_STRONG2;
	//	//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_2COMBO_ATTACK);
	//
	//	//	SubMP(PLAYER_STRONG_ATTACK2_MP);
	//	//}
	//}
	////3攻撃
	//if (is_3_attack)
	//{
	//	//if (CheckMP(PLAYER_STRONG_ATTACK3_MP))
	//	//{
	//	//	SetZeroMoveXY();
	//	//	m_move.y += 5.f;
	//	//	m_isFloating = true;
	//	//	m_state = PLAYER_STATE_ATTACK;
	//	//	m_attackKind = PLAYER_ATTACK_STRONG3;
	//	//	m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_3COMBO_ATTACK);
	//
	//	//	SubMP(PLAYER_STRONG_ATTACK3_MP);
	//	//}
	//}
}

void CPlayer::IFRisingAttack()
{
	//bool is_ready = CInputManager::IsPush(KEY_INPUT_C, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_LEFT_SHOULDER, I_PAD);
	bool is_attack = CInputManager::IsPush(KEY_INPUT_E, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_RIGHT_SHOULDER, I_PAD);

	//if (is_ready)
	//{
	//	//カメラ操作を止める
	//	CCameraManager::GetManager()->GetPlayCamera()->SetIsMoveRot(false);
	//
	//	m_isRising = true;
	//}

	if (is_attack)
	{
		if (CheckMP(PLAYER_STRONG_ATTACK1_MP))
		{
			SetZeroMoveXY();
			m_isRising = false;

			VECTOR rising_vec = { m_risingMove.x, m_risingMove.z, 0.f };

			m_state = PLAYER_STATE_ATTACK;
			m_attackKind = PLAYER_ATTACK_RISING;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_OUT_SLASH);

			m_risingStartPos = GetCenterPos();


			if (CPad::GetPad()->IsLeftStick())
			{
				//左スティック値で動く
				VECTOR stick = { CPad::GetPad()->GetStickBuf(LEFT_STICK_X),CPad::GetPad()->GetStickBuf(LEFT_STICK_Y), 0.f };

				//プレイヤーの向きをスティックの向きにする
				m_rot.y = CMyMath::VecDir(stick) + DX_PI_F + CCameraManager::GetManager()->GetPlayCameraRotY();
			}

			SetKeyRot();

			//カメラを近づける
			CCameraManager::GetManager()->ApproachPlayCamera(65, 3000);

			SubMP(41.f);

		}
		else
		{

		}
	}
}
void CPlayer::IFBoost()
{
	if (CInputManager::IsPush(KEY_INPUT_Z, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
	{
		if (m_boostKind == PLAYER_BOOST_GO)
		{
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_POWER_UP);

			m_move = ZERO_VECTOR;

			if (m_boostEffect == -1)
			{

				m_boostEffect = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_POWER_UP],
					m_pos, false);

				CEffekseerCtrl::SetSpeed(m_boostEffect, 1.2f);
				CEffekseerCtrl::SetScale(m_boostEffect, VGet(2.5f, 2.5f, 2.5f));

				m_animeInfo.SetAllReproductionSpeed(1.44f);

				//カメラを近づける
				CCameraManager::GetManager()->ApproachPlayCamera(40.f, 1000);
			}
		}
	}
	else
	{

	}
}

void CPlayer::SetKeyRot()
{
	VECTOR rot = { 0 };
	if (CInputManager::IsDown(KEY_INPUT_D, I_KEY)) { rot.x = 1.f; }
	if (CInputManager::IsDown(KEY_INPUT_A, I_KEY)) { rot.x = -1.f; }
	if (CInputManager::IsDown(KEY_INPUT_W, I_KEY)) { rot.z = 1.f; }
	if (CInputManager::IsDown(KEY_INPUT_S, I_KEY)) { rot.z = -1.f; }

	/*if (!CCommon::CheckZeroVector(rot))
	{
		m_rot.y = CMyMath::VecDir(rot.x, rot.z) + DX_PI_F;
	}*/

	if (!CCommon::CheckZeroVector(rot))
	{
		m_rot.y = CMyMath::VecDir(rot.x, rot.z) + DX_PI_F + CCameraManager::GetManager()->GetPlayCameraRotY();

		//MATRIX all_mat[4] =
		//{
		//	CMyMath::GetTranslateMatrix(VGet(0.f, 0.f, PLAYER_SPEED)),
		//	CMyMath::GetPitchMatrix(m_rot.x),
		//	CMyMath::GetYawMatrix(m_rot.y),
		//	CMyMath::GetRollMatrix(m_rot.z),
		//};
		//
		//MATRIX camera_mat = all_mat[3];
		//for (int mat_index = 3; mat_index >= 0; --mat_index)
		//	camera_mat = CMyMath::MatMult(camera_mat, all_mat[mat_index]);
		//
		////反映
		//move.x = -camera_mat.m[0][3];
		//move.z = -camera_mat.m[2][3];
		//
		//move = CMyMath::VecNormalize(move);
		//move = CMyMath::VecScale(move, PLAYER_SPEED);
		//
		//m_state = PLAYER_STATE_RUN;
		//anime_kind = PLAYER_ANIME_KIND_RUN;
	}
}

void CPlayer::ChargeBoost(const float charge_num)
{
	if (m_boostKind == PLAYER_BOOST_GO ||
		m_boostKind == PLAYER_BOOST_NOW)
	{
		return;
	}

	m_boostNum += charge_num;

	if (m_boostNum >= static_cast<float>(CPlayer::BOOST_MAX_NUM))
	{
		m_boostNum = static_cast<float>(CPlayer::BOOST_MAX_NUM);
		m_boostKind = PLAYER_BOOST_GO;
	}
}
void CPlayer::ResetRising()
{
	m_isRising = false;
	m_risingMove = m_risingPos = m_risingStartPos = ZERO_VECTOR;
	CCameraManager::GetManager()->GetPlayCamera()->SetIsMoveRot(true);

}
void CPlayer::StepStop()
{
	m_move = ZERO_VECTOR;

	m_animeInfo.UpdateAnime();
}
bool CPlayer::IsAttackRising()
{
	if (m_state != PLAYER_STATE_ATTACK) { return false; }
	if (m_attackKind != PLAYER_ATTACK_RISING) { return false; }

	//最後までまわったらtrueを返す
	return true;
}

void CPlayer::InputPlayer()
{
	//ここは共通の入力処理

	//入力処理
	(this->*InputFunc[m_state])();

#ifdef DEBUG_MODE
	if (CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY))
	{
		m_hp = 0;
		m_state = PLAYER_STATE_DEATH;
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_DEATH);
	}

	//if (CInputManager::IsPush(KEY_INPUT_Q, I_KEY))
	//{
	//	//エフェクトリクエスト
	//	int handle = CEffekseerCtrl::Request2(m_effectID[PLAYER_EFFECT_KIND_HIT1],
	//		GetCenterPos(), 0, false);
	//	CEffekseerCtrl::SetSpeed(handle, 1.1f);
	//	CEffekseerCtrl::SetScale(handle, VGet(1.f, 1.f, 10.f));
	//}

	//if (CInputManager::IsPush(KEY_INPUT_W, I_KEY))
	//{
	//	//エフェクトリクエスト
	//	int handle = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_THUNDER],
	//		GetCenterPos(), false);
	//	CEffekseerCtrl::SetSpeed(handle, 1.5f);
	/*}*/


	//if (CInputManager::IsPush(KEY_INPUT_U, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_UP, I_PAD))
	//{
	//	if (m_boostEffect == -1)
	//	{
	//		m_animeInfo.SetAllReproductionSpeed(1.3f);

	//		m_boostEffect = CEffekseerCtrl::Request(m_effectID[PLAYER_EFFECT_KIND_POWER_UP],
	//			m_pos, false);

	//		CEffekseerCtrl::SetSpeed(m_boostEffect, 1.2f);
	//		CEffekseerCtrl::SetScale(m_boostEffect, VGet(2.5f, 2.5f, 2.5f));
	//	}
	//}
	//if (CInputManager::IsPush(KEY_INPUT_I, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_DOWN, I_PAD))
	//{
	//	m_animeInfo.ResetAllReproductionSpeed();

	//	CEffekseerCtrl::Stop(m_boostEffect);
	//	m_boostEffect = -1;
	//}

	/*if (CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
	{
		m_move.y += PLAYER_JUMP_POWER;
	}*/


#endif
}

//プレイヤーをスティックの向きにする
void CPlayer::LookLeftStick()
{
	if (CPad::GetPad()->IsLeftStick())
	{
		VECTOR left_stick = { CPad::GetPad()->GetStickBuf(LEFT_STICK_X),CPad::GetPad()->GetStickBuf(LEFT_STICK_Y), 0.f };
		m_rot.y = CMyMath::VecDir(left_stick) + DX_PI_F + CCameraManager::GetManager()->GetPlayCameraRotY();
	}
}
//プレイヤーを一番近い敵の向きにする
void CPlayer::FacingNearEnemy()
{
	//スティックが入力されているならステック優先
	if (CPad::GetPad()->IsLeftStick()) { return; }

	float near_len = static_cast<float>(INFINITE);
	VECTOR near_vec;
	bool is_alive = false;

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//死んでるならcontinue
		if (!enemy_list->GetIsAlive()) { continue; }
		if (!enemy_list->GetIsAppear()) { continue; }

		VECTOR enemy_pos = enemy_list->GetPos();

		//ベクトルと長さを計算
		VECTOR vec = CMyMath::VecCreate(m_pos, enemy_pos);
		float len = CMyMath::VecLong(vec);

		//一番近ければ値変更
		if (near_len > len)
		{
			near_len = len;
			near_vec = vec;
		}

		is_alive = true;
	}

	//向き変更
	if(is_alive)
		m_rot = { 0.f, CMyMath::VecDir(near_vec.x, near_vec.z) + DX_PI_F, 0.f };
}

void CPlayer::SetZeroMoveXY()
{
	m_move.x = m_move.z = 0.f;
}
void CPlayer::SetMoveXY(float x, float y)
{
	m_move.x = x;
	m_move.y = y;
}
void CPlayer::SetMoveXY(VECTOR move)
{
	m_move.x = move.x;
	m_move.z = move.z;
}


void CPlayer::Fin()
{

}

//点滅処理
void CPlayer::StepFlash()
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
	if (m_invincibleVal >= PLAYER_INVINCIBLE_TIME)
	{
		m_isInvincible = false;
		m_isDraw = true;

		m_invincibleVal = 0;
	}
}
void CPlayer::DamagePlayer()
{
	/*if (m_state != PLAYER_STATE_HIT)
	{
		return;
	}

	if (m_animeInfo.IsEndAnime())
	{
		m_state = PLAYER_STATE_RUN;
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_RUN);
	}*/
}

//コンボ記憶
void CPlayer::MemoryCombo(int mem)
{
	int combo_index = 0;
	for (; m_comboMemory[combo_index] > 0; combo_index++) {};

	//大きすぎたらreturn
	if (combo_index >= CPlayer::COMBO_NUM) { return; }

	m_comboMemory[combo_index] = mem;
}
bool CPlayer::IsMemoryCombo(int num)
{
	for (int combo_index = 0; combo_index < CPlayer::COMBO_NUM; combo_index++)
	{
		if (num == m_comboMemory[combo_index])
		{
			return true;
		}
	}

	return false;
}
//コンボリセット
void CPlayer::InitCombo()
{
	memset(m_comboMemory, 0, sizeof(m_comboMemory));
}

//データ反映処理
void CPlayer::Update()
{
	//回転値反映
	MV1SetRotationXYZ(m_handle, m_rot);

	//座標セット
	MV1SetPosition(m_handle, m_pos);
	UpdateSword();

	//再生時間のセット
	MV1SetAttachAnimTime(m_handle, m_animeInfo.m_attachIndex,
		m_animeInfo.m_currentFrame);
}

//剣の更新処理
void CPlayer::UpdateSword()
{
	MATRIX scale = MGetScale(VGet(10.0f, 6.5f, 10.0f));
	MATRIX rot_y = MGetRotY(-DX_PI_F / 1.5f);
	MATRIX rot_z = MGetRotZ(DX_PI_F);
	MATRIX mat = MV1GetFrameLocalWorldMatrix(m_handle, 50);

	MATRIX ans = MMult(MMult(rot_z, rot_y), scale);
	ans = MMult(ans, mat);
	MV1SetMatrix(m_swordHandle, ans);
}

void CPlayer::HealingMP()
{
	m_mp += HEALING_MP;

	if (m_mp >= m_maxMP) { m_mp = m_maxMP; }
}
void CPlayer::HealingMP(float heal)
{
	m_mp += heal;

	if (m_mp >= m_maxMP) { m_mp = m_maxMP; }
}
bool CPlayer::CheckMP(const float mp)
{
	if (m_mp >= mp)return true;

	return false;
}

//プレイヤー座標セット＆描画
void CPlayer::Draw()
{
	if (m_isDraw)
	{
		//最後に描画
		MV1DrawModel(m_handle);
		MV1DrawModel(m_swordHandle);
	}

	if (m_isRising)
	{
		DrawLine3D(GetCenterPos(), m_risingPos, GetColor(255, 50, 50));
	}

#ifdef DEBUG_MODE

	VECTOR player_coll_center_pos = m_pos;
	player_coll_center_pos.y += 23.f;
	VECTOR player_coll_pos_up = player_coll_center_pos;
	player_coll_pos_up.y += 24.f;
	player_coll_pos_up.x += 7.f;
	player_coll_pos_up.z += 7.f;
	VECTOR player_coll_pos_down = player_coll_center_pos;
	player_coll_pos_down.y -= 23.f;
	player_coll_pos_down.x -= 7.f;
	player_coll_pos_down.z -= 7.f;

	DrawCube3D(player_coll_pos_up, player_coll_pos_down, GetColor(255, 50, 50),
		GetColor(0, 0, 0), false);

	VECTOR player_pos = m_pos;
	player_pos.y += PLAYER_CENTER_Y;
	DrawSphere3D(player_pos, m_radius, 8, GetColor(255, 50, 50), GetColor(0, 0, 0), false);

#endif
}

//プレイヤー当たり判定処理
void CPlayer::HitFunc(PlayerHitKind kind)
{
	switch (kind)
	{
	case PLAYER_HIT_ENEMY:

		break;

	case PLAYER_HIT_NORMAL_ENEMY_ATTACK:

		if (m_boostKind != PLAYER_BOOST_NOW)
		{
			m_state = PLAYER_STATE_KNOCK_BACK;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_KNOCK_BACK);
		}
		else
		{
			//無敵になる
			if (!m_isInvincible)
			{
				m_isInvincible = true;

				//drawフラグを折る
				m_isDraw = false;
			}
		}

		if (!CSceneManager::GetManager()->GetIsTutorial())
			m_hp -= 8;

		CSoundManager::PlaySE(SE_KIND_DAMAGE1);

		break;

	case PLAYER_HIT_GOLEM_MAGIC:
		if (m_boostKind != PLAYER_BOOST_NOW)
		{
			m_state = PLAYER_STATE_KNOCK_BACK;
			m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_KNOCK_BACK);
		}
		else
		{
			//無敵になる
			if (!m_isInvincible)
			{
				m_isInvincible = true;

				//drawフラグを折る
				m_isDraw = false;
			}
		}

		m_hp -= 8;

		CSoundManager::PlaySE(SE_KIND_HIT1);

		break;
	}

	if (m_hp <= 0) 
	{ 
		m_hp = 0; 
		m_state = PLAYER_STATE_DEATH;
		m_animeInfo.ChangeAnime(PLAYER_ANIME_KIND_DEATH);
	}

}
//当たり判定するかどうか(trueなら当たり判定をとる)
bool CPlayer::JudgeCollission(PlayerHitKind kind)
{
	switch (kind)
	{
	case PLAYER_HIT_ENEMY:

		break;

	case PLAYER_HIT_NORMAL_ENEMY_ATTACK:
		if (m_state == PLAYER_STATE_KNOCK_BACK ||
			m_state == PLAYER_STATE_GET_UP ||
			(m_state == PLAYER_STATE_ATTACK && m_attackKind == PLAYER_ATTACK_STRONG1) ||
			(m_state == PLAYER_STATE_ATTACK && m_attackKind == PLAYER_ATTACK_RISING) ||
			m_animeInfo.m_animeID == PLAYER_ANIME_KIND_POWER_UP)
			return false;

		if (m_isInvincible) { return false; }
		if (m_isAvoid) { return false; }
		
		break;

	}

	return true;
}

float CPlayer::GetAttackRadius()const
{
	return ATTACK_COLLISION_RADIUS[m_attackKind];
}

VECTOR CPlayer::GetAttackCenterPos()const
{
	VECTOR center_pos = GetCenterPos();

	//プレイヤーの向きに少し進める
	VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };

	if (m_attackKind == PLAYER_ATTACK_NORMAL7)
	{
		add.x = sinf(m_rot.y + DX_PI_F - ONE_DEGREE * 20);
		add.z = cosf(m_rot.y + DX_PI_F - ONE_DEGREE * 20);
	}

	//スカラー倍してたす
	add = CMyMath::VecScale(add, ATTACK_COLLISION_ADD[m_attackKind]);

	return CMyMath::VecAdd(center_pos, add);

}
float CPlayer::GetAttackCollisionFrame()const
{
	return ATTACK_COLLISION_FRAME[m_attackKind];
}

AttackType CPlayer::GetAttackType()
{
	AttackType type;

	switch (m_attackKind)
	{
	case PLAYER_ATTACK_NORMAL3:
	case PLAYER_ATTACK_STRONG1:
		type = ATTACK_TYPE_KNOCK_BACK; break;
	
	case PLAYER_ATTACK_STRONG2:
		if (m_attackCount == 2)	type = ATTACK_TYPE_KNOCK_BACK;
		else type = ATTACK_TYPE_HIT; break;

	default:
		type = ATTACK_TYPE_HIT; break;
	}

	return type;
}

VECTOR CPlayer::GetAddPos(float add)
{
	//プレイヤーの向きに少し進める
	VECTOR vec_add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };

	VECTOR pos = m_pos;

	vec_add = CMyMath::VecScale(vec_add, add);
	pos = CMyMath::VecAdd(pos, vec_add);

	return pos;
}

PlayerAttackInfo CPlayer::GetPlayerAttackInfo()
{
	PlayerAttackInfo attack_info = PLAYER_ATTACK_INFO[m_attackKind];

	if (attack_info.attackKind == PLAYER_ATTACK_NORMAL4 &&
		m_boostComboCount == 2)
	{
		attack_info.damage = attack_info.barrier_damage = 12;
	}

	return attack_info;
}
