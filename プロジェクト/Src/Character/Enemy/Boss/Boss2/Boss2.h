#pragma once

#include "../Boss.h"

#include "../../Enemy.h"

#include "../../../../Draw/DrawManager.h"
#include "../../../../Common.h"

static const float BOSS2_CENTER_Y = 36.f;

static const float BOSS2_JUMP_CENTER_ADD_Y = 40.f;

//�A�j�����
enum Boss2AnimeKind
{
	BOSS2_ANIME_KIND_NONE = -1,

	BOSS2_ANIME_KIND_ATTACK_BEAM1,
	BOSS2_ANIME_KIND_ATTACK_BEAM2,
	BOSS2_ANIME_KIND_ATTACK_BEAM3,
	BOSS2_ANIME_KIND_ATTACK_JUMP,
	BOSS2_ANIME_KIND_ATTACK_KICK,
	BOSS2_ANIME_KIND_ATTACK_PUNCH1,
	BOSS2_ANIME_KIND_ATTACK_PUNCH2,
	BOSS2_ANIME_KIND_ATTACK_PUNCH_COMBO2,
	BOSS2_ANIME_KIND_ATTACK_PUNCH_COMBO4,
	BOSS2_ANIME_KIND_ATTACK_ROLL,
	BOSS2_ANIME_KIND_ATTACK_ROLL_KICK1,
	BOSS2_ANIME_KIND_ATTACK_ROLL_KICK2,
	BOSS2_ANIME_KIND_ATTACK_STRONG_PUNCH,
	BOSS2_ANIME_KIND_ATTACK_STRONG_SLASH,
	BOSS2_ANIME_KIND_AVOID,
	BOSS2_ANIME_KIND_AVOID_LEFT,
	BOSS2_ANIME_KIND_AVOID_RIGHT,
	BOSS2_ANIME_KIND_DEATH1,
	BOSS2_ANIME_KIND_DEATH2,
	BOSS2_ANIME_KIND_GET_UP,
	BOSS2_ANIME_KIND_HIT1,
	BOSS2_ANIME_KIND_HIT2,
	BOSS2_ANIME_KIND_IDLE,
	BOSS2_ANIME_KIND_POWER_UP,
	BOSS2_ANIME_KIND_WALK,

	BOSS2_ANIME_KIND_NUM,
};

enum Boss2AttackKind
{
	BOSS2_ATTACK_NONE = -1,

	BOSS2_ATTACK_BEAM1,		//Beam1
	BOSS2_ATTACK_BEAM1,		//Beam2
	BOSS2_ATTACK_KICK1,		//�L�b�N�P
	BOSS2_ATTACK_KICK2,		//�L�b�N2
	BOSS2_ATTACK_JUMP,		//�W�����v�U��
	BOSS2_ATTACK_SPECIAL,	//����U��

	BOSS2_ATTACK_NUM,
};



//�S�[�����G�N���X
class CBoss2 : public CBoss
{

private:
	Boss2AttackKind	m_attackKind;
	int				m_attackCount[BOSS2_ATTACK_NUM];
	int				m_attackMaxCount[BOSS2_ATTACK_NUM];
	int				m_notCollisionCount;
	float			m_jumpAttackSpd;


	CBoss2() :
		m_attackKind(BOSS2_ATTACK_NONE),
		m_attackMaxCount{},
		m_attackCount{},
		m_jumpAttackSpd(),
		m_notCollisionCount()
	{
		m_kind = OBJECT_KIND_PLAYER;
		m_enemyKind = ENEMY_KIND_BOSS2;
		m_bossKind = BOSS_KIND_2;
	}
private:

	//�_�ŏ���
	void StepFlash();

	void ExecIdle();
	void ExecAvoid();
	void ExecWalk();
	void ExecTrack();
	void ExecAttackReady();
	void ExecAttack();
	void ExecHit();
	void ExecDown();

	void Appear();
	void Appear(VECTOR start_pos, bool is_barrier = false);

	//�U������
	void ExecAttackHit();
	void ExecAttackAll();
	void ExecAttackChain();
	void ExecAttackDash();
	void ExecAttackJump();
	void ExecAttackSpecial();
	void (CBoss2::* ExecAttackFunc[BOSS2_ATTACK_NUM])() =
	{
		&CBoss2::ExecAttackHit,
		&CBoss2::ExecAttackAll,
		&CBoss2::ExecAttackChain,
		&CBoss2::ExecAttackDash,
		&CBoss2::ExecAttackJump,
		&CBoss2::ExecAttackSpecial,
	};

	//����U���֘A
	void AttackSpecialRand(float f);
	void AttackSpecialHoming();
	void AttackSpecialHomingRand(float f);

public:
	void Init()		override;			//������
	void Load()		override;			//���[�h
	void Load(int model_handle) override;
	void Step()		override;			//�X�e�b�v
	void Update()	override;			//�f�[�^���f����
	void Draw()		override;			//�`��
	void Fin()		override;			//�㏈��

	void Reset();

	//�A�j���ύX
	void ChangeAnime(Boss2AnimeKind kind);
	void ChangeAnimeToState();

	//�����蔻�菈��
	void HitFunc(EnemyHitKind hit_kind);

	//�����蔻�肷�邩�ǂ���(true�Ȃ瓖���蔻����Ƃ�)
	bool JudgeCollission(EnemyHitKind kind);


	//����
	static CBoss2* Create(int create_num)
	{
		CBoss2* golem_enemy = _NEW(CBoss2);

		golem_enemy->Init();
		golem_enemy->SetCreateNum(create_num);

		//DrawList�ɒǉ�
		golem_enemy->m_drawID = CDrawManager::AddData(golem_enemy);

		return golem_enemy;
	}


	/*
		Getter&Setter
	*/

	//���S���W
	VECTOR GetCenterPos() {
		VECTOR center_pos = m_pos;
		center_pos.y += BOSS2_CENTER_Y;

		if (m_attackKind == BOSS2_ATTACK_JUMP)
		{
			if (m_animeInfo.IsBetweenFrame(40, 67))
			{
				center_pos.y += BOSS2_JUMP_CENTER_ADD_Y;
			}
		}

		return center_pos;
	}

	//Hp
	int GetHP()const { return m_hp; }
	void SetHP(int hp) { m_hp = hp; }


	//�X�s�[�h
	float GetSpeed()const { return m_speed; }
	void SetSpeed(float speed) { m_speed = speed; }


	float GetAttackRadius();
	VECTOR GetAttackCenterPos();


	AttackType GetAttackType();

	int GetAttackKind();
};


