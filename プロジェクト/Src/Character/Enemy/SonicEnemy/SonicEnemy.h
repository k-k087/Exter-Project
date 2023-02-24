#pragma once

#include "DxLib.h"

#include "../Enemy.h"
#include "../../../Draw/DrawManager.h"

#include "../../../Common.h"

static const float SONIC_ENEMY_CENTER_Y = 25.f;

//�A�j�����
enum SonicEnemyAnimeKind
{
	SONIC_ENEMY_ANIME_KIND_NONE = -1,

	SONIC_ENEMY_ANIME_KIND_AVOID,
	SONIC_ENEMY_ANIME_KIND_BEAM_ATTACK,
	SONIC_ENEMY_ANIME_KIND_DEFAULT,
	SONIC_ENEMY_ANIME_KIND_DOWN,
	SONIC_ENEMY_ANIME_KIND_HIT,
	SONIC_ENEMY_ANIME_KIND_IDLE,
	SONIC_ENEMY_ANIME_KIND_SPEED_ATTACK,
	SONIC_ENEMY_ANIME_KIND_WALK,

	SONIC_ENEMY_ANIME_KIND_NUM,
};

enum SonicEnemyAttackKind
{
	SONIC_ENEMY_ATTACK_NONE = -1,

	SONIC_ENEMY_ATTACK_HIT,		//�q�b�g
	SONIC_ENEMY_ATTACK_BEAM,	//�r�[��

	SONIC_ENEMY_ATTACK_NUM,
};


//�\�j�b�N�G�N���X
class CSonicEnemy : public CEnemy
{

private:
	SonicEnemyAttackKind	m_attackKind;
	int						m_attackCount[SONIC_ENEMY_ATTACK_NUM];
	int						m_attackMaxCount[SONIC_ENEMY_ATTACK_NUM];
	float					m_avoidSpeed;
	VECTOR					m_memoryPos;

	int						m_paramI;

	CSonicEnemy() :
		m_attackKind(SONIC_ENEMY_ATTACK_NONE),
		m_attackMaxCount{},
		m_attackCount{},
		m_avoidSpeed(),
		m_paramI(),
		m_memoryPos{}
	{
		m_kind = OBJECT_KIND_PLAYER;
		m_enemyKind = ENEMY_KIND_SONIC;
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

	//�U������
	void ExecAttackHit();
	void ExecAttackBeam();
	void (CSonicEnemy::* ExecAttackFunc[SONIC_ENEMY_ATTACK_NUM])() =
	{
		&CSonicEnemy::ExecAttackHit,
		&CSonicEnemy::ExecAttackBeam,
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
	void ChangeAnime(SonicEnemyAnimeKind kind);
	void ChangeAnimeToState();

	//�����蔻�菈��
	void HitFunc(EnemyHitKind hit_kind);

	//�����蔻�肷�邩�ǂ���(true�Ȃ瓖���蔻����Ƃ�)
	bool JudgeCollission(EnemyHitKind kind);

	void Appear();
	void Appear(VECTOR start_pos, bool is_barrier = false);

	void Killing();


	//����
	static CSonicEnemy* Create(int create_num)
	{
		CSonicEnemy* sonic_enemy = _NEW(CSonicEnemy);

		sonic_enemy->Init();
		sonic_enemy->SetCreateNum(create_num);

		//DrawList�ɒǉ�
		sonic_enemy->m_drawID = CDrawManager::AddData(sonic_enemy);

		return sonic_enemy;
	}


	/*
		Getter&Setter
	*/

	//���S���W
	VECTOR GetCenterPos() {
		VECTOR center_pos = m_pos;
		center_pos.y += SONIC_ENEMY_CENTER_Y;
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

