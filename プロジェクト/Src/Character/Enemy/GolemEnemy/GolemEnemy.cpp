#include "DxLib.h"

#include <math.h>
#include <cassert>

#include "GolemEnemy.h"

#include "../EnemyManager.h"

#include "../../Player/PlayerManager.h"
#include "../../Player/Player.h"

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
#include "../../../Magic/MagicManager.h"
#include "../Effekseer.h"


static const CPlayAnimeData GOLEM_ENEMY_ANIME_DATA[GOLEM_ENEMY_ANIME_KIND_NUM] =
{
	{ GOLEM_ENEMY_ANIME_KIND_ATTACK,			0.5f,	-1, false, ANIME_TYPE_END	},
	{ GOLEM_ENEMY_ANIME_KIND_ATTACK2,			0.2f,	-1, false, ANIME_TYPE_END	},
	{ GOLEM_ENEMY_ANIME_KIND_DEFAULT,			1.0f,	-1, false, ANIME_TYPE_END	},
	{ GOLEM_ENEMY_ANIME_KIND_DOWN,				1.0f,	-1, false, ANIME_TYPE_END	},
	{ GOLEM_ENEMY_ANIME_KIND_HIT,				0.4f,	-1, false, ANIME_TYPE_END	},
	{ GOLEM_ENEMY_ANIME_KIND_IDLE,				0.7f,	-1, false, ANIME_TYPE_LOOP	},
	{ GOLEM_ENEMY_ANIME_KIND_WALK,				0.8f,	-1, false, ANIME_TYPE_LOOP	},
};

//	���a
static const float GOLEM_ENEMY_RADIUS = 24.f;

//	�ړ���
static const float GOLEM_ENEMY_MOVE_SPEED = 0.3f;

//	���f���T�C�Y
static const VECTOR GOLEM_ENEMY_SIZE = { 14.f,46.f,14.f };
static const VECTOR GOLEM_ENEMY_SCALE = { 0.3f,0.3f,0.3f };

//	���G����
static const int GOLEM_ENEMY_INVINCIBLE_TIME = 200;

//	�~���b
static const int ATTACK_MAX_COUNT = 300;
static const int ATTACK_RAND_COUNT = 300;
static const int ATTACK2_MAX_COUNT = 1000;
static const int ATTACK2_RAND_COUNT = 1300;

static const int ATTACK_MAX[GOLEM_ENEMY_ATTACK_NUM] =
{
	ATTACK_MAX_COUNT, ATTACK2_MAX_COUNT
};
static const int ATTACK_RAND[GOLEM_ENEMY_ATTACK_NUM] =
{
	ATTACK_RAND_COUNT, ATTACK2_RAND_COUNT
};

//	HP
static const int GOLEM_ENEMY_HP = 200;

static const float PLAYER_FIND_RADIUS = 300.f;
static const float ATTACK_RANGE_RADIUS = 42.f;


static const float ATTACK_COLLISION_RADIUS = 45.f;
static const float ATTACK_COLLISION_ADD = 40.f;
static const float ATTACK_COLLISION_FRAME = 65.f;


//������
void CGolemEnemy::Init()
{
	m_size = GOLEM_ENEMY_SIZE;
	m_hp = m_maxHP = GOLEM_ENEMY_HP;
	m_radius = GOLEM_ENEMY_RADIUS;
	m_enemyHitRadius = GOLEM_ENEMY_RADIUS * 1.8f;
	m_horizonRadius = PLAYER_FIND_RADIUS;
	m_attackRange = ATTACK_RANGE_RADIUS;
	m_isAppear = false;
	m_appearCount = 0;

	for (int attack_index = 0; attack_index < GOLEM_ENEMY_ATTACK_NUM; ++attack_index)
	{
		m_attackMaxCount[attack_index] = ATTACK_MAX[attack_index] + GetRand(ATTACK_RAND[attack_index]);
	}
}
void CGolemEnemy::Reset()
{
	m_appearCount = m_attackCount[0] = m_attackCount[1] = m_invincibleVal = 0;
	m_speed = 0.f;
	m_hp = m_maxHP = GOLEM_ENEMY_HP;
	m_isAlive = true;
	m_isAppear = m_isAttack = m_isTargeting = false;
	m_move = m_oldPos = m_pos = m_rot = m_startPos = ZERO_VECTOR;
	m_attackKind = GOLEM_ENEMY_ATTACK_NONE;
}

void CGolemEnemy::Appear()
{
	Init();
	m_pos.x = (float)GetRand(300); if (GetRand(1) == 1) { m_pos.x *= -1.f; }
	m_pos.z = (float)GetRand(300); if (GetRand(1) == 1) { m_pos.z *= -1.f; }
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;
	m_isAlive = true;


	//�A�j���Đ�
	m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CGolemEnemy::Appear(VECTOR start_pos, bool is_barrier)
{
	Init();
	
	m_startPos = m_pos = start_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;
	m_isAlive = true;

	if (is_barrier)
	{
		m_barrierEffectID = CEnemyManager::GetManager()->
			RequestEffect(ENEMY_EFFECT_KIND_BARIIER, GetCenterPos(), true);
		CEffekseerCtrl::SetScale(m_barrierEffectID, VGet(1.4f, 1.4f, 1.4f));
		m_barrierHP = m_barrierMaxHP = 100;
	}


	//�A�j���Đ�
	m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}

//���[�h
void CGolemEnemy::Load()
{
	m_pos.x = (float)GetRand(500); if (GetRand(1) == 1) { m_pos.x *= -1.f; }
	m_pos.z = (float)GetRand(500); if (GetRand(1) == 1) { m_pos.z *= -1.f; }
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;

	//���f�����[�h
	if (m_handle == -1)
	{

	}

	//�A�j�����Z�b�g
	m_animeInfo.SetAnimeData(GOLEM_ENEMY_ANIME_DATA, GOLEM_ENEMY_ANIME_KIND_NUM, m_handle);

	//�傫���Z�b�g
	MV1SetScale(m_handle, GOLEM_ENEMY_SCALE);

	//�A�j���Đ�
	m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CGolemEnemy::Load(int model_handle)
{
	m_pos.x = (float)GetRand(500);
	m_pos.z = (float)GetRand(500);
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;

	//���f�����[�h
	if (m_handle == -1)
	{
		m_handle = MV1DuplicateModel(model_handle);
	}

	//�A�j�����Z�b�g
	m_animeInfo.SetAnimeData(GOLEM_ENEMY_ANIME_DATA, GOLEM_ENEMY_ANIME_KIND_NUM, m_handle);

	//�傫���Z�b�g
	MV1SetScale(m_handle, GOLEM_ENEMY_SCALE);

	//�A�j���Đ�
	m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}

//�X�e�b�v
void CGolemEnemy::Step()
{
	//�����Ă��Ȃ��Ȃ�return
	if (!m_isAlive)return;
	if (AppearStep()) { return; }
	if (!m_isAppear)return;
#ifdef DEBUG_MODE
	//�J�����}�l�W���[
	CCameraManager* camera_manager = CCameraManager::GetManager();
	//�f�o�b�O���[�h�Ȃ瓮�����Ȃ�
	if (camera_manager->GetCurrentCameraMode() == CCameraManager::CAMERA_ID_DEBUG)return;
#endif

	StepAttackColl();

	//�ړ��O���W�L�^
	m_oldPos = m_pos;

	//�_�ŏ���
	StepFlash();

	//��Ԃ��݂Ă�����ς���
	(this->*ExecFunc[m_state])();

	//�ړ��ʉ��Z
	m_pos = VAdd(m_pos, m_move);

	//�Ō�ɃA�j���A�b�v�f�[�g
	m_animeInfo.UpdateAnime();
}

void CGolemEnemy::ChangeAnime(GolemEnemyAnimeKind kind)
{
	m_animeInfo.ChangeAnime(kind);
}

void CGolemEnemy::ExecIdle()
{
	//�ړ��ʂ��O��
	m_move = ZERO_VECTOR;

	//�v���C���[�̂ق�������
	FacingPlayer();

	//m_state = ENEMY_STATE_WALK;
	//m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_WALK);
}
void CGolemEnemy::ExecWalk()
{




}
void CGolemEnemy::ExecAvoid()
{

}
void CGolemEnemy::ExecTrack()
{
	//�v���C���[�̕��������ĕ���
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);
	m_rot.y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
	//���K��
	vec = CMyMath::VecNormalize(vec);
	//�X�J���[�{
	vec = CMyMath::VecScale(vec, GOLEM_ENEMY_MOVE_SPEED);
	//��񔽉f
	m_move = vec;

	//���@�U���J�E���g���Z
	m_attackCount[GOLEM_ENEMY_ATTACK_MAGIC] += FRAME_TIME;

	//	�J�E���g�����܂����疂�@�U����
	if (m_attackCount[GOLEM_ENEMY_ATTACK_MAGIC] >= m_attackMaxCount[GOLEM_ENEMY_ATTACK_MAGIC])
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(GOLEM_ENEMY_ANIME_KIND_ATTACK2);
		m_attackKind = GOLEM_ENEMY_ATTACK_MAGIC;

		//�J�E���g�p�ϐ�������
		m_attackCount[GOLEM_ENEMY_ATTACK_MAGIC] = 0;
		m_attackMaxCount[GOLEM_ENEMY_ATTACK_MAGIC] = 
			 ATTACK_MAX[GOLEM_ENEMY_ATTACK_MAGIC] * m_hp / m_maxHP + 
			GetRand(ATTACK_RAND[GOLEM_ENEMY_ATTACK_MAGIC]);
	}
}
void CGolemEnemy::ExecAttackReady()
{
	//�ړ��ʂ��O��
	m_move = ZERO_VECTOR;

	//	�v���C���[�̂ق�������
	FacingPlayer();

	//	�J�E���g���Z
	m_attackCount[GOLEM_ENEMY_ATTACK_HIT] += FRAME_TIME;

	//	�J�E���g�����܂�����Hit�U����
	if (m_attackCount[GOLEM_ENEMY_ATTACK_HIT] >= m_attackMaxCount[GOLEM_ENEMY_ATTACK_HIT])
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(GOLEM_ENEMY_ANIME_KIND_ATTACK);
		m_attackKind = GOLEM_ENEMY_ATTACK_HIT;

		//�J�E���g�p�ϐ�������
		m_attackCount[GOLEM_ENEMY_ATTACK_HIT] = 0;
		m_attackMaxCount[GOLEM_ENEMY_ATTACK_HIT] = 
			ATTACK_MAX[GOLEM_ENEMY_ATTACK_HIT] * m_hp / m_maxHP +
			GetRand(ATTACK_RAND[GOLEM_ENEMY_ATTACK_HIT]);
	}
}
void CGolemEnemy::ExecAttack()
{
	//�ړ��ʂ��O��
	m_move = ZERO_VECTOR;

	(this->*ExecAttackFunc[m_attackKind])();


	//�A�j�����I��������ҋ@��
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(GOLEM_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;
		m_attackKind = GOLEM_ENEMY_ATTACK_NONE;
	}
}

void CGolemEnemy::ExecAttackHit()
{
	//����
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 1.f / 5.f)
	{
		//x,z�̈ړ��ʂ����߂�
		m_move.x = sinf(m_rot.y + DX_PI_F) * 0.2f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 0.2f;
	}

	//	�r���܂Ńv���C���[�̕�������
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 2.f / 5.f)
		FacingPlayer();

	//�G�t�F�N�g����
	if (m_animeInfo.IsSpecificFrame(60.f))
	{
		VECTOR attack_center_pos = GetCenterPos();
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 40.f);
		attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		attack_center_pos.y = 5.f;

		//�G�t�F�N�g���N�G�X�g
		int handle = CEnemyManager::GetManager()->RequestEffect(ENEMY_EFFECT_KIND_FIRE,
			attack_center_pos, false);

		CEffekseerCtrl::SetScale(handle, VGet(1.3f, 1.3f, 1.3f));
	}

	//�U������
	if (m_animeInfo.IsSpecificFrame(ATTACK_COLLISION_FRAME))
	{
		m_isAttack = true;
	}
}
void CGolemEnemy::ExecAttackMagic()
{
	//	�r���܂Ńv���C���[�̕�������
	//	���@����
	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 2.f / 5.f)
	{
		FacingPlayer();
	}
	
	if (m_animeInfo.IsSpecificFrame(19))
	{
		//�G�t�F�N�g�o��
		VECTOR add = { sinf(m_rot.y + DX_PI_F + ONE_DEGREE * 60.f), 0, cosf(m_rot.y + DX_PI_F + ONE_DEGREE * 60.f) };
		add = CMyMath::VecScale(add, 30.f);
		VECTOR start_pos = m_pos;
		start_pos.y += 50.f;
		start_pos = CMyMath::VecAdd(start_pos, add);

		VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
		player_pos.y += PLAYER_CENTER_Y;

		VECTOR move_vec = CMyMath::VecCreate(start_pos, player_pos);
		move_vec = CMyMath::VecNormalize(move_vec);
		//move_vec.y = 0.f;

		//���N�G�X�g���
		MagicRequestInfo request_info = {	start_pos,	m_rot,	move_vec,	0,	0.f,	};

		CMagicManager::GetManager()->RequestEnemyMagic(request_info, MAGIC_KIND_GOLEM_MAGIC);
	}

}
void CGolemEnemy::ExecHit()
{
	//�ړ��ʂ��O��
	m_move = ZERO_VECTOR;

	//	�v���C���[�̂ق�������
	FacingPlayer();

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime / 4.f)
	{
		//x,z�̈ړ��ʂ����߂�
		m_move.x = -sinf(m_rot.y + DX_PI_F) * 0.9f;
		m_move.z = -cosf(m_rot.y + DX_PI_F) * 0.9f;

		return;
	}

	//�A�j�����I��������ҋ@��
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(GOLEM_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;
	}
}
void CGolemEnemy::ExecDown()
{
	//�ړ��ʂ��O��
	m_move = ZERO_VECTOR;

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime / 4.f)
	{
		//x,z�̈ړ��ʂ����߂�
		m_move.x = -sinf(m_rot.y + DX_PI_F) * 3.0f;
		m_move.z = -cosf(m_rot.y + DX_PI_F) * 3.0f;

		return;
	}

	if (m_animeInfo.m_currentFrame < m_animeInfo.m_frameTime * 3.f / 5.f) { return; }

	//�������O�Ȃ�isAlive��false��
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


	//�A�j�����I��������ҋ@��
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(GOLEM_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;

		m_isInvincible = true;
	}

}

void CGolemEnemy::Fin()
{

}

//�_�ŏ���
void CGolemEnemy::StepFlash()
{
	if (!m_isInvincible)
		return;

	m_invincibleVal += FRAME_TIME;

	//�_�Ő؂�ւ�
	if (m_invincibleVal % 200 >= 100)
	{
		m_isDraw = true;
	}
	else
	{
		m_isDraw = false;
	}

	//��莞�Ԃ���������Ƃɖ߂�
	if (m_invincibleVal >= GOLEM_ENEMY_INVINCIBLE_TIME)
	{
		m_isInvincible = false;
		m_isDraw = true;

		m_invincibleVal = 0;
	}
}

//�f�[�^���f����
void CGolemEnemy::Update()
{
	//�����Ă��Ȃ��Ȃ�return
	if (!m_isAlive)return;
	if (!m_isAppear)return;

	//��]�l���f
	MV1SetRotationXYZ(m_handle, m_rot);

	//���W�Z�b�g
	MV1SetPosition(m_handle, m_pos);

	//�Đ����Ԃ̃Z�b�g
	MV1SetAttachAnimTime(m_handle, m_animeInfo.m_attachIndex,
		m_animeInfo.m_currentFrame);

	//�G�t�F�N�g�A�b�v�f�[�g
	if (m_barrierEffectID != -1)
	{
		CEffekseerCtrl::SetPosition(m_barrierEffectID, GetCenterPos());
	}
}

//���W�Z�b�g���`��
void CGolemEnemy::Draw()
{
	//�����Ă��Ȃ��Ȃ�return
	if (!m_isAlive)return;
	if (!m_isAppear)return;

	if (m_isDraw)
	{
		//�Ō�ɕ`��
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
	player_pos.y += GOLEM_ENEMY_CENTER_Y;
	DrawSphere3D(player_pos, m_radius, 8, GetColor(255, 255, 255), GetColor(0, 0, 0), false);

#if 0

	DrawSphere3D(player_pos, PLAYER_FIND_RADIUS, 4, GetColor(50, 255, 50), GetColor(0, 0, 0), false);

#endif

#endif
}


bool CGolemEnemy::JudgeCollission(EnemyHitKind kind)
{
	//�܂������Ă��Ȃ��Ȃ�false��������
	if (!m_isAlive) { return false; }
	if (!m_isAppear) { return false; }

	switch (kind)
	{
		//�G���m
	case ENEMY_HIT_ENEMY:
		break;

		//�v���C���[
	case ENEMY_HIT_PLAYER:
		break;

		//�v���C���[�U��
	case ENEMY_HIT_PLAYER_ATTACK:
		//�_�E�����Ă�Ƃ��△�G��Ԃ̂Ƃ��͓����蔻����Ƃ�Ȃ�
		if (m_state == ENEMY_STATE_DOWN || m_isInvincible)
			return false; break;

		//���E
	case ENEMY_HORIZON_HIT_PLAYER:
		//�q�b�g�A�_�E���A�U�������A�U���̂Ƃ��͓����蔻����Ƃ�Ȃ�
		if (m_state == ENEMY_STATE_HIT ||
			m_state == ENEMY_STATE_DOWN ||
			m_state == ENEMY_STATE_ATTACK)
			return false; break;

	case ENEMY_RANGE_HIT_PLAYER:
		//�G���v���C���[���݂��ĂȂ��Ƃ��͂Ƃ�Ȃ�
		if (m_state != ENEMY_STATE_TRACK)
			return false; break;

	case ENEMY_ATTACK_HIT_PLAYER:
		if (!m_isAttack)return false; break;
	}

	//�Ō�܂ŉ������true��������
	return true;
}

//�����蔻�菈��
void CGolemEnemy::HitFunc(EnemyHitKind hit_kind)
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

		//	�܂��o���A������Ȃ�o���A�Ƀ_���[�W
		if (m_barrierEffectID != -1)
		{
			m_barrierHP -= player_attack_info.barrier_damage;

			if (m_barrierHP <= 0)
			{
				CEffekseerCtrl::Stop(m_barrierEffectID);
				m_barrierEffectID = -1;

				//	��������HP�Ƀ_���[�W
				m_hp -= CMyMath::Abs(m_barrierHP);
				m_barrierHP = 0;

				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_HIT);
				m_animeInfo.SetReproductionSpeed(2.2f);
			}
		}
		//	���_��
		else
		{
			m_hp -= player_attack_info.damage;

			//	�U�����
			switch (player_attack_info.attackKind)
			{
			case PLAYER_ATTACK_NORMAL1:
			case PLAYER_ATTACK_NORMAL2:
			{
				
			}
				break;

			case PLAYER_ATTACK_NORMAL3:
			{

			}
				break;

			case PLAYER_ATTACK_NORMAL4:
			{
				int combo_count = player->GetBoostComboCount();
				if (combo_count == 0)
				{
					if (CCommon::GetProbability(20))
					{
						m_state = ENEMY_STATE_HIT;
						m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_HIT);
					}
				}
				else if (combo_count == 1)
				{
					if (CCommon::GetProbability(20))
					{
						m_state = ENEMY_STATE_HIT;
						m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_HIT);
					}
				}
				else
				{
					m_state = ENEMY_STATE_HIT;
					m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_HIT);

				}

				break;
			}
			case PLAYER_ATTACK_NORMAL5:
			case PLAYER_ATTACK_NORMAL6:
			{

			}
				break;

			case PLAYER_ATTACK_NORMAL7:
			{
				if (m_state != ENEMY_STATE_ATTACK)
				{

					m_state = ENEMY_STATE_HIT;
					m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_HIT);
					m_animeInfo.SetReproductionSpeed(2.2f);

					//�v���C���[�̂ق�������
					FacingPlayer();
				}
			}
				break;

			case PLAYER_ATTACK_STRONG1:
			{
				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_HIT);

				//	�v���C���[�̂ق�������
				FacingPlayer();
			}
				break;

			case PLAYER_ATTACK_RISING:
			{

				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_HIT);

				//	�v���C���[�̂ق�������
				FacingPlayer();
			}

				break;
			}
		}

		//	�O�����ɂȂ�����_�E������
		if (m_hp <= 0)
		{
			m_hp = 0;
			m_state = ENEMY_STATE_DOWN;
			m_animeInfo.PlayAnime(GOLEM_ENEMY_ANIME_KIND_DOWN);
		}

		break;
	}
}

void CGolemEnemy::Killing()
{
	if (m_hp <= 0) { return; }

	m_hp = 0;
	m_state = ENEMY_STATE_DOWN;
	m_animeInfo.ChangeAnime(GOLEM_ENEMY_ANIME_KIND_DOWN);
}

void CGolemEnemy::ChangeAnimeToState()
{
	GolemEnemyAnimeKind anime_kind = GOLEM_ENEMY_ANIME_KIND_NONE;

	switch (m_state)
	{
	case ENEMY_STATE_ATTACK: anime_kind = GOLEM_ENEMY_ANIME_KIND_ATTACK; break;
	case ENEMY_STATE_ATTACK_READY: anime_kind = GOLEM_ENEMY_ANIME_KIND_IDLE; break;
	case ENEMY_STATE_DOWN: anime_kind = GOLEM_ENEMY_ANIME_KIND_DOWN; break;
	case ENEMY_STATE_HIT: anime_kind = GOLEM_ENEMY_ANIME_KIND_HIT; break;
	case ENEMY_STATE_IDLE: anime_kind = GOLEM_ENEMY_ANIME_KIND_IDLE; break;
	case ENEMY_STATE_TRACK: anime_kind = GOLEM_ENEMY_ANIME_KIND_WALK; break;
	case ENEMY_STATE_WALK: anime_kind = GOLEM_ENEMY_ANIME_KIND_WALK; break;
	}

#ifdef DEBUG_MODE

#endif 
	m_animeInfo.ChangeAnime(anime_kind);

}

float CGolemEnemy::GetAttackRadius()
{
	return ATTACK_COLLISION_RADIUS;
}


VECTOR CGolemEnemy::GetAttackCenterPos()
{
	VECTOR center_pos = GetCenterPos();

	//�����Ă�����ɏ����i�߂�
	VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };

	//�X�J���[�{���Ă���
	add = CMyMath::VecScale(add, ATTACK_COLLISION_ADD);


	return CMyMath::VecAdd(center_pos, add);
}

AttackType CGolemEnemy::GetAttackType()
{
	return ATTACK_TYPE_HIT;
}