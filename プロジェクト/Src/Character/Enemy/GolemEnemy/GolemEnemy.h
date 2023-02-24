#pragma once

#include "DxLib.h"

#include "../Enemy.h"
#include "../../../Draw/DrawManager.h"

#include "../../../Common.h"

static const float GOLEM_ENEMY_CENTER_Y = 25.f;

//�A�j�����
enum GolemEnemyAnimeKind
{
	GOLEM_ENEMY_ANIME_KIND_NONE = -1,

	GOLEM_ENEMY_ANIME_KIND_ATTACK,
	GOLEM_ENEMY_ANIME_KIND_ATTACK2,
	GOLEM_ENEMY_ANIME_KIND_DEFAULT,
	GOLEM_ENEMY_ANIME_KIND_DOWN,
	GOLEM_ENEMY_ANIME_KIND_HIT,
	GOLEM_ENEMY_ANIME_KIND_IDLE,
	GOLEM_ENEMY_ANIME_KIND_WALK,

	GOLEM_ENEMY_ANIME_KIND_NUM,
};

enum GolemEnemyAttackKind
{
	GOLEM_ENEMY_ATTACK_NONE = -1,

	GOLEM_ENEMY_ATTACK_HIT,		//�q�b�g
	GOLEM_ENEMY_ATTACK_MAGIC,	//���@

	GOLEM_ENEMY_ATTACK_NUM,
};


//�S�[�����G�N���X
class CGolemEnemy : public CEnemy
{

private:
	GolemEnemyAttackKind	m_attackKind;
	int						m_attackCount[GOLEM_ENEMY_ATTACK_NUM];
	int						m_attackMaxCount[GOLEM_ENEMY_ATTACK_NUM];

	CGolemEnemy() :
		m_attackKind(GOLEM_ENEMY_ATTACK_NONE),
		m_attackMaxCount{},
		m_attackCount{}
	{
		m_kind = OBJECT_KIND_PLAYER;
		m_enemyKind = ENEMY_KIND_GOLEM;
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
	void ExecAttackMagic();
	void (CGolemEnemy::* ExecAttackFunc[GOLEM_ENEMY_ATTACK_NUM])() =
	{
		&CGolemEnemy::ExecAttackHit,
		&CGolemEnemy::ExecAttackMagic,
	};

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
	void ChangeAnime(GolemEnemyAnimeKind kind);
	void ChangeAnimeToState();

	//�����蔻�菈��
	void HitFunc(EnemyHitKind hit_kind);

	//�����蔻�肷�邩�ǂ���(true�Ȃ瓖���蔻����Ƃ�)
	bool JudgeCollission(EnemyHitKind kind);

	void Killing();

	//����
	static CGolemEnemy* Create(int create_num)
	{
		CGolemEnemy* golem_enemy = _NEW(CGolemEnemy);

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
		center_pos.y += GOLEM_ENEMY_CENTER_Y;
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

	int GetAttackKind() { return m_attackKind; }
};

