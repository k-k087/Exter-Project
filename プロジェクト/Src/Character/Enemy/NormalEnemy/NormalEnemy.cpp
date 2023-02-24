#include "DxLib.h"

#include <math.h>
#include <cassert>

#include "NormalEnemy.h"

#include "../../Player/PlayerManager.h"
#include "../../Player/Player.h"

#include "../EnemyManager.h"

#include "../../../Common.h"
#include "../../../Field/Field.h"
#include "../../../Input/InputManager.h"
#include "../../../Debug/Debug.h"
#include "../../../Camera/CameraManager.h"
#include "../../../Draw/DrawManager.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Sound/SoundManager.h"
#include "../../../Anime/Anime.h"
#include "../../../MyMath/MyMath.h"

#include "../../Effekseer/Effekseer.h"


static const CPlayAnimeData NORMAL_ENEMY_ANIME_DATA[NORMAL_ENEMY_ANIME_KIND_NUM] =
{
	{ NORMAL_ENEMY_ANIME_KIND_ATTACK,			0.5f,	-1, false, ANIME_TYPE_END	},
	{ NORMAL_ENEMY_ANIME_KIND_DOWN,				1.0f,	-1, false, ANIME_TYPE_END	},
	{ NORMAL_ENEMY_ANIME_KIND_HIT,				0.7f,	-1, false, ANIME_TYPE_END	},
	{ NORMAL_ENEMY_ANIME_KIND_IDLE,				1.0f,	-1, false, ANIME_TYPE_LOOP	},
	{ NORMAL_ENEMY_ANIME_KIND_WALK,				1.0f,	-1, false, ANIME_TYPE_LOOP	},
};

//	半径
static const float NORMAL_ENEMY_RADIUS = 13.f;

//	移動量
static const float NORMAL_ENEMY_MOVE_SPEED = 1.0f;

//	モデルサイズ
static const VECTOR NORMAL_ENEMY_SIZE = { 14.f,46.f,14.f };
static const VECTOR NORMAL_ENEMY_SCALE = { 0.8f,0.8f,0.8f };


//	無敵時間
static const int NORMAL_ENEMY_INVINCIBLE_TIME = 200;

//	ミリ秒
static const int ATTACK_MAX_COUNT = 2000;

//	HP
static const int NORMAL_ENEMY_HP = 50;

static const float PLAYER_FIND_RADIUS = 400.f;
static const float ATTACK_RANGE_RADIUS = 30.f;




static const float ATTACK_COLLISION_RADIUS = 15.f;
static const float ATTACK_COLLISION_ADD = 15.f;
static const float ATTACK_COLLISION_FRAME = 19.f;


//初期化
void CNormalEnemy::Init()
{
	m_size = NORMAL_ENEMY_SIZE;
	m_hp = m_maxHP = NORMAL_ENEMY_HP;
	m_radius = NORMAL_ENEMY_RADIUS;
	m_enemyHitRadius = NORMAL_ENEMY_RADIUS * 1.8f;
	m_horizonRadius = PLAYER_FIND_RADIUS;
	m_attackRange = ATTACK_RANGE_RADIUS;
	m_isAppear = false;
	m_appearCount = 0;
	m_barrierEffectID = -1;
}

void CNormalEnemy::Reset()
{
	m_appearCount = m_attackCount = m_invincibleVal = 0;
	m_speed = 0.f;
	m_hp = m_maxHP = NORMAL_ENEMY_HP;
	m_isAlive = true;
	m_isAppear = m_isAttack = m_isTargeting = false;
	m_move = m_oldPos = m_pos = m_rot = m_startPos = ZERO_VECTOR;
}
void CNormalEnemy::Appear()
{
	Init();
	if (CCommon::GetProbability(50))
	{
		m_barrierEffectID = CEnemyManager::GetManager()->
			RequestEffect(ENEMY_EFFECT_KIND_BARIIER, GetCenterPos(), true);
		CEffekseerCtrl::SetScale(m_barrierEffectID, VGet(0.7f, 0.7f, 0.7f));
		m_barrierHP = m_barrierMaxHP = 100;
	}

	m_pos.x = (float)GetRand(300); if (GetRand(1) == 1) { m_pos.x *= -1.f; }
	m_pos.z = (float)GetRand(300); if (GetRand(1) == 1) { m_pos.z *= -1.f; }
	m_startPos = m_oldPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;
	m_isAlive = true;

	//アニメ再生
	m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CNormalEnemy::Appear(VECTOR start_pos, bool is_barrier)
{
	Init();

	if (is_barrier)
	{
		m_barrierEffectID = CEnemyManager::GetManager()->
			RequestEffect(ENEMY_EFFECT_KIND_BARIIER, GetCenterPos(), true);
		CEffekseerCtrl::SetScale(m_barrierEffectID, VGet(0.7f, 0.7f, 0.7f));
		m_barrierHP = m_barrierMaxHP = 100;
	}

	m_pos = start_pos;
	m_startPos = m_oldPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;
	m_isAlive = true;

	//アニメ再生
	m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}


//ロード
void CNormalEnemy::Load()
{
	m_pos.x = (float)GetRand(500); if (GetRand(1) == 1) { m_pos.x *= -1.f; }
	m_pos.z = (float)GetRand(500); if (GetRand(1) == 1) { m_pos.z *= -1.f; }
	m_startPos = m_oldPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;

	/*if (CCommon::GetProbability(50))
	{
		m_barrierEffectID = CEnemyManager::GetManager()->
			RequestEffect(ENEMY_EFFECT_KIND_BARIIER, GetCenterPos(), true);
	}*/

	//モデルロード
	if (m_handle == -1)
	{
	}

	//アニメ情報セット
	m_animeInfo.SetAnimeData(NORMAL_ENEMY_ANIME_DATA, NORMAL_ENEMY_ANIME_KIND_NUM, m_handle);

	//大きさセット
	//MV1SetScale(m_handle, NORMAL_ENEMY_SCALE);
	//大きさセット
	MV1SetScale(m_handle, VGet(2.f, 2.f, 2.f));

	//アニメ再生
	m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CNormalEnemy::Load(int model_handle)
{
	m_pos.x = (float)GetRand(500);
	m_pos.z = (float)GetRand(500);
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;

	//モデルロード
	if (m_handle == -1)
	{
		m_handle = MV1DuplicateModel(model_handle);
	}

	//アニメ情報セット
	m_animeInfo.SetAnimeData(NORMAL_ENEMY_ANIME_DATA, NORMAL_ENEMY_ANIME_KIND_NUM, m_handle);

	//大きさセット
	MV1SetScale(m_handle, NORMAL_ENEMY_SCALE);
	//大きさセット
	//MV1SetScale(m_handle, VGet(17.f, 17.f, 17.f));

	//アニメ再生
	m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}

//ステップ
void CNormalEnemy::Step()
{
	//生きていないならreturn
	if (!m_isAlive)return;
	if (AppearStep()) { return; }
	if (!m_isAppear) { return; }

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

	//状態をみてしょりを変える
	(this->*ExecFunc[m_state])();

	//移動量加算
	m_pos = VAdd(m_pos, m_move);

	//最後にアニメアップデート
	m_animeInfo.UpdateAnime();
}

void CNormalEnemy::ChangeAnime(NormalEnemyAnimeKind kind)
{
	m_animeInfo.ChangeAnime(kind);
}

void CNormalEnemy::ExecIdle()
{
	//移動量を０に
	m_move = ZERO_VECTOR;
	
	//プレイヤーのほうを見る
	FacingPlayer();

	//m_state = ENEMY_STATE_WALK;
	//m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_WALK);
}
void CNormalEnemy::ExecWalk()
{
	
	

	
}
void CNormalEnemy::ExecAvoid()
{

}
void CNormalEnemy::ExecTrack()
{
	//プレイヤーの方を向いて歩く
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);
	m_rot.y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
	//正規化
	vec = CMyMath::VecNormalize(vec);
	//スカラー倍
	vec = CMyMath::VecScale(vec, NORMAL_ENEMY_MOVE_SPEED);
	//情報反映
	m_move = vec;
}
void CNormalEnemy::ExecAttackReady()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	//	プレイヤーのほうを向く
	FacingPlayer();

	//	カウント加算
	m_attackCount += FRAME_TIME;

	//	カウントがたまったら攻撃へ
	if (m_attackCount >= ATTACK_MAX_COUNT)
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_ATTACK);

		m_attackCount = 0;
	}
}
void CNormalEnemy::ExecAttack()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	//動く
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 1.f / 5.f)
	{
		//x,zの移動量を決める
		m_move.x = sinf(m_rot.y + DX_PI_F) * 0.2f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 0.2f;
	}

	//	途中までプレイヤーの方を向く
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 2.f / 5.f)
		FacingPlayer();

	//攻撃判定
	if (m_animeInfo.IsSpecificFrame(ATTACK_COLLISION_FRAME))
		m_isAttack = true;
	

	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;
	}
}
void CNormalEnemy::ExecHit()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	//	プレイヤーのほうを向く
	FacingPlayer();

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime / 4.f)
	{
		//x,zの移動量を決める
		m_move.x = -sinf(m_rot.y + DX_PI_F) * 0.3f;
		m_move.z = -cosf(m_rot.y + DX_PI_F) * 0.3f;

		return;
	}

	//	カウント少し加算
	m_attackCount += 1;

	//	カウントがたまったら確率で攻撃へ
	if (m_attackCount >= ATTACK_MAX_COUNT)
	{
		if (CCommon::GetProbability(8))
		{
			m_state = ENEMY_STATE_ATTACK;
			m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_ATTACK);
			m_animeInfo.SetReproductionSpeed(0.7f);

			m_attackCount = 0;
		}
	}

	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;
	}
}
void CNormalEnemy::ExecDown()
{
	//移動量を０に
	m_move = ZERO_VECTOR;

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime  / 4.f)
	{
		//x,zの移動量を決める
		m_move.x = -sinf(m_rot.y + DX_PI_F) * 3.0f;
		m_move.z = -cosf(m_rot.y + DX_PI_F) * 3.0f;

		return;
	}

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 3.f / 5.f) { return; }
	
	//ｈｐが０ならisAliveをfalseに
	if (m_hp <= 0)
	{
		m_isAlive = m_isAppear = false;
		if (m_barrierEffectID != -1)
		{
			CEffekseerCtrl::Stop(m_barrierEffectID);
			m_barrierEffectID = -1;
		}
		CEnemyManager::GetManager()->SubAliveNum();
	}
	

	//アニメが終了したら待機へ
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;

		m_isInvincible = true;
	}

}

void CNormalEnemy::Fin()
{

}

//点滅処理
void CNormalEnemy::StepFlash()
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
	if (m_invincibleVal >= NORMAL_ENEMY_INVINCIBLE_TIME)
	{
		m_isInvincible = false;
		m_isDraw = true;

		m_invincibleVal = 0;
	}
}

//データ反映処理
void CNormalEnemy::Update()
{
	//生きていないならreturn
	if (!m_isAlive)return;
	if (!m_isAppear) { return; }

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
void CNormalEnemy::Draw()
{
	//生きていないならreturn
	if (!m_isAlive)return;
	if (!m_isAppear) { return; }

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
	player_pos.y += NORMAL_ENEMY_CENTER_Y;
	DrawSphere3D(player_pos, m_radius, 8, GetColor(255, 255, 255), GetColor(0, 0, 0), false);

#if 0

	DrawSphere3D(player_pos, PLAYER_FIND_RADIUS, 4, GetColor(50, 255, 50), GetColor(0, 0, 0), false);

#endif

#endif
}


bool CNormalEnemy::JudgeCollission(EnemyHitKind kind)
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
			return false; 

		break;

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
void CNormalEnemy::HitFunc(EnemyHitKind hit_kind)
{

	switch (hit_kind)
	{
	case ENEMY_HIT_PLAYER:


		break;


	case ENEMY_HIT_PLAYER_ATTACK:
	{
		CPlayer* player = CPlayerManager::GetManager()->GetPlayer();
		PlayerAttackInfo player_attack_info = 
			player->GetPlayerAttackInfo();

		//	まずバリアがあるならバリアにダメージ
		if (m_barrierEffectID != -1)
		{
			m_barrierHP -= player_attack_info.barrier_damage;

			if (m_barrierHP <= 0)
			{
				CEffekseerCtrl::Stop(m_barrierEffectID);
				m_barrierEffectID = -1;

				//	超えた分HPにダメージ
				m_hp -= static_cast<int>(CMyMath::Abs(static_cast<float>(m_barrierHP)));
				m_barrierHP = 0;

				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_HIT);
				m_animeInfo.SetReproductionSpeed(1.3f);
			}
		}
		//	肉ダメ
		else
		{
			m_hp -= player_attack_info.damage;


			//	攻撃種類をみて状態を変える
			switch (player_attack_info.attackKind)
			{
			case PLAYER_ATTACK_NORMAL1:
			case PLAYER_ATTACK_NORMAL2:
			{
				if (m_state != ENEMY_STATE_ATTACK)
				{
					m_state = ENEMY_STATE_HIT;
					m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_HIT);

					//プレイヤーのほうを向く
					FacingPlayer();
				}
			}
			break;

			case PLAYER_ATTACK_NORMAL3:
			{
				if (m_state != ENEMY_STATE_ATTACK)
				{
					if (player->GetBoostKind() != PLAYER_BOOST_NOW)
					{
						m_state = ENEMY_STATE_DOWN;
						m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_DOWN);
					}
					else
					{
						m_state = ENEMY_STATE_HIT;
						m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_HIT);
					}

					//プレイヤーのほうを向く
					FacingPlayer();
				}
				break;
			}
			case PLAYER_ATTACK_NORMAL4:
			{
				int combo_count = player->GetBoostComboCount();
				if (combo_count == 0 || 
					combo_count == 1 )
				{
					m_state = ENEMY_STATE_HIT;
					m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_HIT);
				}
				else if (combo_count == 2)
				{
					m_state = ENEMY_STATE_DOWN;
					m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_DOWN);
				}

				break;
			}
			case PLAYER_ATTACK_NORMAL5:
			case PLAYER_ATTACK_NORMAL6:
			{
				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_HIT);

				//プレイヤーのほうを向く
				FacingPlayer();

				break;
			}
			case PLAYER_ATTACK_NORMAL7:
			{
				m_state = ENEMY_STATE_DOWN;
				m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_DOWN);

				//プレイヤーのほうを向く
				FacingPlayer();

				break;
			}
			case PLAYER_ATTACK_STRONG1:
			{
				m_state = ENEMY_STATE_DOWN;
				m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_DOWN);

				//	プレイヤーのほうを向く
				FacingPlayer();

				break;
			}

			case PLAYER_ATTACK_RISING:
			{
				m_state = ENEMY_STATE_DOWN;
				m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_DOWN);

				//	プレイヤーのほうを向く
				FacingPlayer();
				break;
			}

			}
		}


		//０いかになったらダウンする
		if (m_hp <= 0)
		{
			m_hp = 0;
			m_state = ENEMY_STATE_DOWN;
			m_animeInfo.PlayAnime(NORMAL_ENEMY_ANIME_KIND_DOWN);
		}
	}
	break;

	}
	
}

void CNormalEnemy::Killing()
{
	if (m_hp <= 0) { return; }

	m_hp = 0;
	m_state = ENEMY_STATE_DOWN;
	m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_DOWN);
}

void CNormalEnemy::ChangeAnimeToState()
{
	NormalEnemyAnimeKind anime_kind = NORMAL_ENEMY_ANIME_KIND_NONE;

	switch (m_state)
	{
	case ENEMY_STATE_ATTACK: anime_kind = NORMAL_ENEMY_ANIME_KIND_ATTACK; break;
	case ENEMY_STATE_ATTACK_READY: anime_kind = NORMAL_ENEMY_ANIME_KIND_IDLE; break;
	case ENEMY_STATE_DOWN: anime_kind = NORMAL_ENEMY_ANIME_KIND_DOWN; break;
	case ENEMY_STATE_HIT: anime_kind = NORMAL_ENEMY_ANIME_KIND_HIT; break;
	case ENEMY_STATE_IDLE: anime_kind = NORMAL_ENEMY_ANIME_KIND_IDLE; break;
	case ENEMY_STATE_TRACK: anime_kind = NORMAL_ENEMY_ANIME_KIND_WALK; break;
	case ENEMY_STATE_WALK: anime_kind = NORMAL_ENEMY_ANIME_KIND_WALK; break;
	}

#ifdef DEBUG_MODE
	
#endif 
	m_animeInfo.ChangeAnime(anime_kind);

}

float CNormalEnemy::GetAttackRadius()
{
	return ATTACK_COLLISION_RADIUS;
}
	

VECTOR CNormalEnemy::GetAttackCenterPos()
{
	VECTOR center_pos = GetCenterPos();

	//向いてる向きに少し進める
	VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };

	//スカラー倍してたす
	add = CMyMath::VecScale(add, ATTACK_COLLISION_ADD);

	return CMyMath::VecAdd(center_pos, add);
}

AttackType CNormalEnemy::GetAttackType()
{
	return ATTACK_TYPE_HIT;
}