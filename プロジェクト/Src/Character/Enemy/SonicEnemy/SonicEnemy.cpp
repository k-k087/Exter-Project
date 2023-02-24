#include "DxLib.h"

#include <math.h>
#include <cassert>

#include "SonicEnemy.h"

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
#include "../../../Beam/BeamManager.h"
#include "../Effekseer.h"


static const CPlayAnimeData SONIC_ENEMY_ANIME_DATA[SONIC_ENEMY_ANIME_KIND_NUM] =
{
	{ SONIC_ENEMY_ANIME_KIND_AVOID,				1.5f,	-1, false, ANIME_TYPE_END	},
	{ SONIC_ENEMY_ANIME_KIND_BEAM_ATTACK,		0.4f,	-1, false, ANIME_TYPE_END	},
	{ SONIC_ENEMY_ANIME_KIND_DEFAULT,			1.0f,	-1, false, ANIME_TYPE_END	},
	{ SONIC_ENEMY_ANIME_KIND_DOWN,				1.0f,	-1, false, ANIME_TYPE_END	},
	{ SONIC_ENEMY_ANIME_KIND_HIT,				0.3f,	-1, false, ANIME_TYPE_END	},
	{ SONIC_ENEMY_ANIME_KIND_IDLE,				0.7f,	-1, false, ANIME_TYPE_LOOP	},
	{ SONIC_ENEMY_ANIME_KIND_SPEED_ATTACK,		0.4f,	-1, false, ANIME_TYPE_END	},
	{ SONIC_ENEMY_ANIME_KIND_WALK,				0.8f,	-1, false, ANIME_TYPE_LOOP	},

};

//	���a
static const float SONIC_ENEMY_RADIUS = 22.f;

//	�ړ���
static const float SONIC_ENEMY_MOVE_SPEED = 0.3f;

static const float SONIC_ENEMY_AVOID_SPEED_HIT = 6.f;
static const float SONIC_ENEMY_AVOID_SPEED_BEAM = 10.f;

//	���f���T�C�Y
static const VECTOR SONIC_ENEMY_SIZE = { 14.f,46.f,14.f };
static const VECTOR SONIC_ENEMY_SCALE = { 5.8f,5.8f,5.8f };

//	���G����
static const int SONIC_ENEMY_INVINCIBLE_TIME = 200;

//	�~���b
static const int ATTACK_MAX_COUNT = 2000;
static const int ATTACK_RAND_COUNT = 1000;
static const int ATTACK2_MAX_COUNT = 1500;
static const int ATTACK2_RAND_COUNT = 1000;

static const int ATTACK_MAX[SONIC_ENEMY_ATTACK_NUM] =
{
	ATTACK_MAX_COUNT, ATTACK2_MAX_COUNT
};
static const int ATTACK_RAND[SONIC_ENEMY_ATTACK_NUM] =
{
	ATTACK_RAND_COUNT, ATTACK2_RAND_COUNT
};

//	HP
static const int SONIC_ENEMY_HP = 220;
static const float PLAYER_FIND_RADIUS = 450.f;
static const float ATTACK_RANGE_RADIUS = 50.f;
static const float BEAM_COUNT_DISTANCE = 180.f;

static const float ATTACK_COLLISION_RADIUS = 25.f;
static const float ATTACK_COLLISION_ADD = 8.f;
static const float ATTACK_COLLISION_FRAME = 65.f;

//������
void CSonicEnemy::Init()
{
	m_size = SONIC_ENEMY_SIZE;
	m_hp = m_maxHP = SONIC_ENEMY_HP;
	m_radius = SONIC_ENEMY_RADIUS;
	m_enemyHitRadius = SONIC_ENEMY_RADIUS * 1.8f;
	m_horizonRadius = PLAYER_FIND_RADIUS;
	m_attackRange = ATTACK_RANGE_RADIUS;
	m_isAppear = false;
	m_appearCount = 0;

	for (int attack_index = 0; attack_index < SONIC_ENEMY_ATTACK_NUM; ++attack_index)
	{
		m_attackMaxCount[attack_index] = ATTACK_MAX[attack_index] + GetRand(ATTACK_RAND[attack_index]);
	}
}
void CSonicEnemy::Reset()
{
	m_appearCount = m_attackCount[0] = m_attackCount[1] = m_invincibleVal = 0;
	m_speed = 0.f;
	m_hp = m_maxHP = SONIC_ENEMY_HP;
	m_isAlive = true;
	m_isAppear = m_isAttack = m_isTargeting = false;
	m_move = m_oldPos = m_pos = m_rot = m_startPos = ZERO_VECTOR;
	m_attackKind = SONIC_ENEMY_ATTACK_NONE;
}

//���[�h
void CSonicEnemy::Load()
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
	m_animeInfo.SetAnimeData(SONIC_ENEMY_ANIME_DATA, SONIC_ENEMY_ANIME_KIND_NUM, m_handle);

	//�傫���Z�b�g
	MV1SetScale(m_handle, SONIC_ENEMY_SCALE);

	//�A�j���Đ�
	m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CSonicEnemy::Appear()
{
	Init();
	m_isAlive = true;

	m_pos.x = (float)GetRand(300); if (GetRand(1) == 1) { m_pos.x *= -1.f; }
	m_pos.z = (float)GetRand(300); if (GetRand(1) == 1) { m_pos.z *= -1.f; }
	m_startPos = m_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;


	//�A�j���Đ�
	m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CSonicEnemy::Appear(VECTOR start_pos, bool is_barrier)
{
	Init();
	m_isAlive = true;

	if (is_barrier)
	{
		m_barrierEffectID = CEnemyManager::GetManager()->
			RequestEffect(ENEMY_EFFECT_KIND_BARIIER, GetCenterPos(), true);
		CEffekseerCtrl::SetScale(m_barrierEffectID, VGet(1.5f, 1.5f, 1.5f));
		m_barrierHP = m_barrierMaxHP = 100;
	}


	m_startPos = m_pos = start_pos;
	m_move = ZERO_VECTOR;
	m_rot.y = DX_PI_F;


	//�A�j���Đ�
	m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}
void CSonicEnemy::Load(int model_handle)
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
	m_animeInfo.SetAnimeData(SONIC_ENEMY_ANIME_DATA, SONIC_ENEMY_ANIME_KIND_NUM, m_handle);

	//�傫���Z�b�g
	MV1SetScale(m_handle, SONIC_ENEMY_SCALE);

	//�A�j���Đ�
	m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_IDLE);
	m_state = ENEMY_STATE_IDLE;
}

//�X�e�b�v
void CSonicEnemy::Step()
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

	m_isAttack;

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

void CSonicEnemy::ChangeAnime(SonicEnemyAnimeKind kind)
{
	m_animeInfo.ChangeAnime(kind);
}

void CSonicEnemy::ExecIdle()
{
	//�ړ��ʂ��O��
	m_move = ZERO_VECTOR;

	//�v���C���[�̂ق�������
	FacingPlayer();

	//m_state = ENEMY_STATE_WALK;
	//m_animeInfo.ChangeAnime(NORMAL_ENEMY_ANIME_KIND_WALK);
}
void CSonicEnemy::ExecWalk()
{




}
void CSonicEnemy::ExecAvoid()
{
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);
	m_rot.y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
	//���K��
	vec = CMyMath::VecNormalize(vec);
	//�X�J���[�{
	vec = CMyMath::VecScale(vec, -m_avoidSpeed);
	//��񔽉f
	m_move = vec;


	//�A�j�����I��������U����
	if (m_animeInfo.IsEndAnime())
	{
		if (m_avoidSpeed == SONIC_ENEMY_AVOID_SPEED_BEAM)
		{
			m_state = ENEMY_STATE_ATTACK;
			m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_BEAM_ATTACK);
			m_attackKind = SONIC_ENEMY_ATTACK_BEAM;
			m_paramI = 1;
			m_animeInfo.SetReproductionSpeed(1.4f);

			//�J�E���g�p�ϐ�������
			m_attackCount[SONIC_ENEMY_ATTACK_BEAM] = 0;
			m_attackMaxCount[SONIC_ENEMY_ATTACK_BEAM] = ATTACK_MAX[SONIC_ENEMY_ATTACK_BEAM] + GetRand(ATTACK_RAND[SONIC_ENEMY_ATTACK_BEAM]);
		}
		else
		{
			m_state = ENEMY_STATE_ATTACK;
			m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_SPEED_ATTACK);
			m_attackKind = SONIC_ENEMY_ATTACK_HIT;

			//�J�E���g�p�ϐ�������
			m_attackCount[SONIC_ENEMY_ATTACK_HIT] = 0;
			m_attackMaxCount[SONIC_ENEMY_ATTACK_HIT] = ATTACK_MAX[SONIC_ENEMY_ATTACK_HIT] + GetRand(ATTACK_RAND[SONIC_ENEMY_ATTACK_HIT]);
		}
	}
}
void CSonicEnemy::ExecTrack()
{
	//�v���C���[�̕��������ĕ���
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	VECTOR vec = CMyMath::VecCreate(m_pos, player_pos);
	m_rot.y = CMyMath::VecDir(vec.x, vec.z) + DX_PI_F;
	//���K��
	vec = CMyMath::VecNormalize(vec);
	//�X�J���[�{
	vec = CMyMath::VecScale(vec, SONIC_ENEMY_MOVE_SPEED);
	//��񔽉f
	m_move = vec;

	//�r�[���U���J�E���g���Z
	//�v���C���[�Ƃ̋���������Ă�����Z
	if (CMyMath::VecLong(player_pos, m_pos) >= BEAM_COUNT_DISTANCE)
	{
		if(CCommon::GetProbability(70))
			m_attackCount[SONIC_ENEMY_ATTACK_BEAM] += FRAME_TIME;
		else
			m_attackCount[SONIC_ENEMY_ATTACK_HIT] += FRAME_TIME;
	}
	//������Ƌ߂�������_�b�V���U���J�E���g���Z
	else
	{
		//	�J�E���g���Z
		m_attackCount[SONIC_ENEMY_ATTACK_HIT] += FRAME_TIME;
	}

	//	�J�E���g�����܂�����r�[���U����
	if (m_attackCount[SONIC_ENEMY_ATTACK_BEAM] >= m_attackMaxCount[SONIC_ENEMY_ATTACK_BEAM])
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_BEAM_ATTACK);
		m_attackKind = SONIC_ENEMY_ATTACK_BEAM;

		//�J�E���g�p�ϐ�������
		m_attackCount[SONIC_ENEMY_ATTACK_BEAM] = 0;
		m_attackMaxCount[SONIC_ENEMY_ATTACK_BEAM] = ATTACK_MAX[SONIC_ENEMY_ATTACK_BEAM] + GetRand(ATTACK_RAND[SONIC_ENEMY_ATTACK_BEAM]);

	}

	//	�J�E���g�����܂�����Speed�U����
	if (m_attackCount[SONIC_ENEMY_ATTACK_HIT] >= m_attackMaxCount[SONIC_ENEMY_ATTACK_HIT])
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_SPEED_ATTACK);
		m_attackKind = SONIC_ENEMY_ATTACK_HIT;

		//�J�E���g�p�ϐ�������
		m_attackCount[SONIC_ENEMY_ATTACK_HIT] = 0;
		m_attackMaxCount[SONIC_ENEMY_ATTACK_HIT] = ATTACK_MAX[SONIC_ENEMY_ATTACK_HIT] + GetRand(ATTACK_RAND[SONIC_ENEMY_ATTACK_HIT]);
	}
}
void CSonicEnemy::ExecAttackReady()
{
	//�ړ��ʂ��O��
	m_move = ZERO_VECTOR;

	//	�v���C���[�̂ق�������
	FacingPlayer();

	//	�J�E���g���Z
	m_attackCount[SONIC_ENEMY_ATTACK_HIT] += FRAME_TIME;

	//	�J�E���g�����܂�����Speed�U����
	if (m_attackCount[SONIC_ENEMY_ATTACK_HIT] >= m_attackMaxCount[SONIC_ENEMY_ATTACK_HIT])
	{
		m_state = ENEMY_STATE_ATTACK;
		m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_SPEED_ATTACK);
		m_attackKind = SONIC_ENEMY_ATTACK_HIT;

		//�J�E���g�p�ϐ�������
		m_attackCount[SONIC_ENEMY_ATTACK_HIT] = 0;
		m_attackMaxCount[SONIC_ENEMY_ATTACK_HIT] = ATTACK_MAX[SONIC_ENEMY_ATTACK_HIT] + GetRand(ATTACK_RAND[SONIC_ENEMY_ATTACK_HIT]);
	}


	//�v���C���[���U���̎��͂��܂ɉ��
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();
	float coll_frame1 = player->GetAttackCollisionFrame() - 14;
	float coll_frame2 = coll_frame1 - 2;
	float current_frame = player->GetAnimeInfo()->m_currentFrame;
	if (player->GetState() == PLAYER_STATE_ATTACK &&
		player->GetAttackKind() >= PLAYER_ATTACK_NORMAL1 && player->GetAttackKind() <= PLAYER_ATTACK_NORMAL9 &&
		coll_frame2 <= current_frame && current_frame <= coll_frame1)
	{

		int sub_hp = m_maxHP - m_hp;
		int rand = 2 + 8 * sub_hp / m_maxHP;

		bool a = CCommon::GetProbability(rand);

		if (a)
		{
			m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_AVOID);
			m_state = ENEMY_STATE_AVOID;
			m_attackKind = SONIC_ENEMY_ATTACK_NONE;
			m_memoryPos = m_pos;

			bool b = CCommon::GetProbability(50);

			if (b)
			{
				m_avoidSpeed = SONIC_ENEMY_AVOID_SPEED_HIT;
			}
			else
			{
				m_avoidSpeed = SONIC_ENEMY_AVOID_SPEED_BEAM;
			}
		}
	}

}
void CSonicEnemy::ExecAttack()
{

	(this->*ExecAttackFunc[m_attackKind])();


	//�A�j�����I��������ҋ@��
	if (m_animeInfo.IsEndAnime())
	{
		m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;
		m_attackKind = SONIC_ENEMY_ATTACK_NONE;
		m_paramI = 0;
	}
}

void CSonicEnemy::ExecAttackHit()
{
	if (m_animeInfo.m_currentFrame < 36)
	{
		//�ړ��ʂ��O��
		m_move = ZERO_VECTOR;

		if (m_animeInfo.m_currentFrame < 25)
		{
			FacingPlayer();
		}

		return;
	}

	m_move = CMyMath::VecScale(m_move, 0.92f);

	if (m_animeInfo.IsSpecificFrame(37.f))
	{
		m_animeInfo.SetReproductionSpeed(4.f);
		//x,z�̈ړ��ʂ����߂�
		m_move.x = sinf(m_rot.y + DX_PI_F) * 20.2f;
		m_move.z = cosf(m_rot.y + DX_PI_F) * 20.2f;
	}



	//�G�t�F�N�g����
	if (m_animeInfo.IsSpecificFrame(60.f))
	{
		//VECTOR attack_center_pos = GetCenterPos();
		//VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		//add = CMyMath::VecScale(add, 40.f);
		//attack_center_pos = CMyMath::VecAdd(attack_center_pos, add);
		//attack_center_pos.y = 5.f;

		////�G�t�F�N�g���N�G�X�g
		//int handle = CEnemyManager::GetManager()->RequestEffect(ENEMY_EFFECT_KIND_FIRE,
		//	attack_center_pos, false);

		//CEffekseerCtrl::SetScale(handle, VGet(1.3f, 1.3f, 1.3f));
	}

	//�U������
	if (m_animeInfo.IsBetweenFrame(37.f, 90.f))
	{
		m_isAttack = true;
	}
}
void CSonicEnemy::ExecAttackBeam()
{

	//�ړ��ʂ��O��
	m_move = ZERO_VECTOR;
	//	�r���܂Ńv���C���[�̕�������
	//	�r�[������
	if (m_animeInfo.m_currentFrame < 30)
	{
		FacingPlayer();
	}

	if (m_animeInfo.IsSpecificFrame(30))
	{
		//�G�t�F�N�g�o��
		VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };
		add = CMyMath::VecScale(add, 30.f);
		VECTOR start_pos = m_pos;
		start_pos.y += 80.f;
		start_pos = CMyMath::VecAdd(start_pos, add);

		VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetCenterPos();

		VECTOR move_vec = CMyMath::VecCreate(start_pos, player_pos);
		move_vec = CMyMath::VecNormalize(move_vec);

		VECTOR beam_rot = m_rot;
		beam_rot.x = CMyMath::VecDir(move_vec.y, CMyMath::VecLong(VGet(move_vec.x, move_vec.z, 0.f)));

		int col_start = 1300;
		float eff_speed = 1.6f;
		if (m_paramI == 1)
		{ 
			col_start = 940; 
			eff_speed = 1.9f; 
		}

		//���N�G�X�g���
		BeamRequestInfo request_info = { start_pos,	beam_rot,	move_vec,	0,		col_start,	eff_speed,		0,		0.f, };

		CBeamManager::GetManager()->RequestEnemyBeam(request_info, BEAM_KIND_SONIC_BEAM, request_info.startPos);
	}

}
void CSonicEnemy::ExecHit()
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
		m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;
	}
}
void CSonicEnemy::ExecDown()
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
		m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_IDLE);
		m_state = ENEMY_STATE_IDLE;

		m_isInvincible = true;
	}

}

void CSonicEnemy::Fin()
{

}

//�_�ŏ���
void CSonicEnemy::StepFlash()
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
	if (m_invincibleVal >= SONIC_ENEMY_INVINCIBLE_TIME)
	{
		m_isInvincible = false;
		m_isDraw = true;

		m_invincibleVal = 0;
	}
}

//�f�[�^���f����
void CSonicEnemy::Update()
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
void CSonicEnemy::Draw()
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
	player_pos.y += SONIC_ENEMY_CENTER_Y;
	DrawSphere3D(player_pos, m_radius, 8, GetColor(255, 255, 255), GetColor(0, 0, 0), false);

#if 0

	DrawSphere3D(player_pos, PLAYER_FIND_RADIUS, 4, GetColor(50, 255, 50), GetColor(0, 0, 0), false);

#endif

#endif
}


bool CSonicEnemy::JudgeCollission(EnemyHitKind kind)
{
	//�܂������Ă��Ȃ��Ȃ�false��������
	if (!m_isAlive) { return false; }
	if (!m_isAppear) { return false; }

	switch (kind)
	{
		//�v���C���[
	case ENEMY_HIT_PLAYER:
		if (m_state == SONIC_ENEMY_ANIME_KIND_SPEED_ATTACK &&
			m_isAttack == true)
		{
			return false;
		}

		break;

		//�G���m
	case ENEMY_HIT_ENEMY:
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
			m_state == ENEMY_STATE_ATTACK ||
			m_state == ENEMY_STATE_AVOID)
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
void CSonicEnemy::HitFunc(EnemyHitKind hit_kind)
{
	switch (hit_kind)
	{
	case ENEMY_HIT_PLAYER:


		break;


	case ENEMY_HIT_PLAYER_ATTACK:

		CPlayer* player = CPlayerManager::GetManager()->GetPlayer();
		PlayerAttackInfo player_attack_info =
			player->GetPlayerAttackInfo();

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
				m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_HIT);
				m_animeInfo.SetReproductionSpeed(2.2f);
			}
		}
		else
		{
			m_hp -= player_attack_info.damage;

			//	�U�����
			switch (player->GetAttackKind())
			{
			case PLAYER_ATTACK_NORMAL1:
			case PLAYER_ATTACK_NORMAL2:
			{

			}
				break;

			case PLAYER_ATTACK_NORMAL3:
				
				if (m_state != ENEMY_STATE_ATTACK)
				{
					if (CCommon::GetProbability(10))
					{
						m_state = ENEMY_STATE_HIT;
						m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_HIT);
					}
				}
				break;

			case PLAYER_ATTACK_NORMAL4:
			{
				int combo_count = player->GetBoostComboCount();
				if (combo_count == 0)
				{
					
				}
				else if (combo_count == 1)
				{
					
				}
				else
				{
					m_state = ENEMY_STATE_HIT;
					m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_HIT);
					m_animeInfo.SetReproductionSpeed(1.4f);
				}

				break;
			}
			case PLAYER_ATTACK_NORMAL5:
			case PLAYER_ATTACK_NORMAL6:
				
				break;

			case PLAYER_ATTACK_NORMAL7:
	
				if (m_state != ENEMY_STATE_ATTACK)
				{
					m_state = ENEMY_STATE_HIT;
					m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_HIT);
					m_animeInfo.SetReproductionSpeed(2.0f);

					//�v���C���[�̂ق�������
					FacingPlayer();
				}
				break;

			case PLAYER_ATTACK_STRONG1:
				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_HIT);

				//	�v���C���[�̂ق�������
				FacingPlayer();

				break;
			case PLAYER_ATTACK_RISING:
				m_state = ENEMY_STATE_HIT;
				m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_HIT);

				//	�v���C���[�̂ق�������
				FacingPlayer();

				break;
			}
		}


		//	�O�����ɂȂ�����_�E������
		if (m_hp <= 0)
		{
			m_hp = 0;
			m_state = ENEMY_STATE_DOWN;
			m_animeInfo.PlayAnime(SONIC_ENEMY_ANIME_KIND_DOWN);
		}

		break;
	}
}

void CSonicEnemy::Killing()
{
	if (m_hp <= 0) { return; }

	m_hp = 0;
	m_state = ENEMY_STATE_DOWN;
	m_animeInfo.ChangeAnime(SONIC_ENEMY_ANIME_KIND_DOWN);
}

void CSonicEnemy::ChangeAnimeToState()
{
	SonicEnemyAnimeKind anime_kind = SONIC_ENEMY_ANIME_KIND_NONE;

	switch (m_state)
	{
	case ENEMY_STATE_ATTACK: anime_kind = SONIC_ENEMY_ANIME_KIND_SPEED_ATTACK; break;
	case ENEMY_STATE_ATTACK_READY: anime_kind = SONIC_ENEMY_ANIME_KIND_IDLE; break;
	case ENEMY_STATE_DOWN: anime_kind = SONIC_ENEMY_ANIME_KIND_DOWN; break;
	case ENEMY_STATE_HIT: anime_kind = SONIC_ENEMY_ANIME_KIND_HIT; break;
	case ENEMY_STATE_IDLE: anime_kind = SONIC_ENEMY_ANIME_KIND_IDLE; break;
	case ENEMY_STATE_TRACK: anime_kind = SONIC_ENEMY_ANIME_KIND_WALK; break;
	case ENEMY_STATE_WALK: anime_kind = SONIC_ENEMY_ANIME_KIND_WALK; break;
	}

#ifdef DEBUG_MODE

#endif 
	m_animeInfo.ChangeAnime(anime_kind);

}

float CSonicEnemy::GetAttackRadius()
{
	return ATTACK_COLLISION_RADIUS;
}


VECTOR CSonicEnemy::GetAttackCenterPos()
{
	VECTOR center_pos = GetCenterPos();

	//�����Ă�����ɏ����i�߂�
	VECTOR add = { sinf(m_rot.y + DX_PI_F), 0, cosf(m_rot.y + DX_PI_F) };

	//�X�J���[�{���Ă���
	add = CMyMath::VecScale(add, ATTACK_COLLISION_ADD);


	return CMyMath::VecAdd(center_pos, add);
}

AttackType CSonicEnemy::GetAttackType()
{
	return ATTACK_TYPE_HIT;
}