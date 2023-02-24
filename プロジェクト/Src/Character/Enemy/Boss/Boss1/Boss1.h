#pragma once

#include "../Boss.h"

#include "../../Enemy.h"

#include "../../../../Draw/DrawManager.h"
#include "../../../../Common.h"

static const float BOSS1_CENTER_Y = 36.f;

static const float BOSS1_JUMP_CENTER_ADD_Y = 40.f;

//�A�j�����
enum Boss1AnimeKind
{
	BOSS1_ANIME_KIND_NONE = -1,

	BOSS1_ANIME_KIND_ATTACK,
	BOSS1_ANIME_KIND_ATTACK_ALL,
	BOSS1_ANIME_KIND_ATTACK_CHAIN,
	BOSS1_ANIME_KIND_ATTACK_DASH,
	BOSS1_ANIME_KIND_ATTACK_JUMP,
	BOSS1_ANIME_KIND_ATTACK_SPECIAL,
	BOSS1_ANIME_KIND_AVOID,
	BOSS1_ANIME_KIND_DEFAULT,
	BOSS1_ANIME_KIND_DOWN,
	BOSS1_ANIME_KIND_HIT,
	BOSS1_ANIME_KIND_IDLE,
	BOSS1_ANIME_KIND_WALK,

	BOSS1_ANIME_KIND_NUM,
};

enum Boss1AttackKind
{
	BOSS1_ATTACK_NONE = -1,

	BOSS1_ATTACK_HIT,		//�q�b�g
	BOSS1_ATTACK_ALL,		//��]
	BOSS1_ATTACK_CHAIN,		//�Q�A��
	BOOS1_ATTACK_DASH,		//�_�b�V��
	BOSS1_ATTACK_JUMP,		//�W�����v�U��
	BOSS1_ATTACK_SPECIAL,	//����U��

	BOSS1_ATTACK_NUM,
};



//�S�[�����G�N���X
class CBoss1 : public CBoss
{

private:
	Boss1AttackKind	m_attackKind;
	int				m_attackCount[BOSS1_ATTACK_NUM];
	int				m_attackMaxCount[BOSS1_ATTACK_NUM];
	int				m_notCollisionCount;
	float			m_jumpAttackSpd;


	CBoss1() :
		m_attackKind(BOSS1_ATTACK_NONE),
		m_attackMaxCount{},
		m_attackCount{},
		m_jumpAttackSpd(),
		m_notCollisionCount()
	{
		m_kind = OBJECT_KIND_PLAYER;
		m_enemyKind = ENEMY_KIND_BOSS1;
		m_bossKind = BOSS_KIND_1;
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
	void (CBoss1::* ExecAttackFunc[BOSS1_ATTACK_NUM])() =
	{
		&CBoss1::ExecAttackHit,
		&CBoss1::ExecAttackAll,
		&CBoss1::ExecAttackChain,
		&CBoss1::ExecAttackDash,
		&CBoss1::ExecAttackJump,
		&CBoss1::ExecAttackSpecial,
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
	void ChangeAnime(Boss1AnimeKind kind);
	void ChangeAnimeToState();

	//�����蔻�菈��
	void HitFunc(EnemyHitKind hit_kind);

	//�����蔻�肷�邩�ǂ���(true�Ȃ瓖���蔻����Ƃ�)
	bool JudgeCollission(EnemyHitKind kind);


	//����
	static CBoss1* Create(int create_num)
	{
		CBoss1* golem_enemy = _NEW(CBoss1);

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
		center_pos.y += BOSS1_CENTER_Y;

		if (m_attackKind == BOSS1_ATTACK_JUMP)
		{
			if (m_animeInfo.IsBetweenFrame(40, 67))
			{
				center_pos.y += BOSS1_JUMP_CENTER_ADD_Y;
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

