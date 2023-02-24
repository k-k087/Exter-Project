#pragma once

#include "../Character.h"

#include "../../Anime/Anime.h"


//���
enum EnemyState
{
	ENEMY_STATE_NONE = -1,

	ENEMY_STATE_IDLE,
	ENEMY_STATE_AVOID,
	ENEMY_STATE_WALK,
	ENEMY_STATE_TRACK,
	ENEMY_STATE_ATTACK_READY,
	ENEMY_STATE_ATTACK,
	ENEMY_STATE_HIT,
	ENEMY_STATE_DOWN,

	ENEMY_STATE_NUM,
};

//�G�̃q�b�g���
enum EnemyHitKind
{
	ENEMY_HIT_NONE = -1,

	ENEMY_HIT_PLAYER,				//�G�ƃv���C���[
	ENEMY_HIT_ENEMY,				//�G�ƓG
	ENEMY_HIT_PLAYER_ATTACK,		//�G�ƃv���C���[�̍U��

	ENEMY_HORIZON_HIT_PLAYER,		//�G�̎��E�ƃv���C���[
	ENEMY_RANGE_HIT_PLAYER,			//�G�U���͈͂ƃv���C���[
	ENEMY_ATTACK_HIT_PLAYER,		//�G�U���ƃv���C���[

	ENEMY_HIT_NUM,
};

//�G���
enum EnemyKind
{
	ENEMY_KIND_NONE = -1,

	ENEMY_KIND_NORMAL,
	ENEMY_KIND_GOLEM,
	ENEMY_KIND_SONIC,



	ENEMY_KIND_BOSS1,
	ENEMY_KIND_BOSS2,
	
	ENEMY_KIND_NUM,
};

//�G�l�~�[���N���X
class CEnemy : public CCharacter
{
public:
	struct AttackData
	{
		bool  isUse;
		float attackAdd;
		float attackRadius; 

		AttackData() :
			attackAdd(), 
			attackRadius(),
			isUse(false)
		{};
	};
protected:
	int			m_invincibleVal;		//���G�Ԋu
	int			m_createNum;			//���Ԗڂɐ������ꂽ��
	int			m_appearCount;			//��~�J�E���g
	int			m_attackCollCount;		//�U������J�E���g

	int			m_barrierEffectID;		//�o���A�G�t�F�N�g�n���h��ID
	int			m_barrierHP;			//�o���AHP
	int			m_barrierMaxHP;			//�o���A�}�b�N�XHP

	float		m_speed;				//�X�s�[�h
	float		m_attackRange;			//�U���͈�
	float		m_enemyHitRadius;		//�G�Ƃ̓����蔻��p���a
	float		m_horizonRadius;		//���E���a

	bool		m_isTargeting;			//�^�[�Q�b�g����Ă��邩
	bool		m_isInvincible;			//���G���
	bool		m_isNotCollision;		//�����蔻����Ƃ�Ȃ�
	bool		m_isAttack;				//�U������
	bool		m_isAppear;				//�o�����Ă��邩
	bool		m_isDraw;				//�`�悷�邩

	EnemyKind	m_enemyKind;			//�G���
	EnemyState	m_state;				//���
	CAnimeInfo	m_animeInfo;			//�A�j�����

	AttackData	m_attackData;			//�U�����

	CEnemy() :
		m_animeInfo{},
		m_createNum(),
		m_barrierHP(),
		m_isNotCollision(false),
		m_attackRange(),
		m_barrierEffectID(-1),
		m_enemyHitRadius(),
		m_state(ENEMY_STATE_NONE),
		m_isTargeting(false),
		m_speed{ 0.f },
		m_barrierMaxHP(),
		m_attackData{},
		m_isDraw(true),
		m_isAppear(false),
		m_isAttack(false),
		m_invincibleVal(0),
		m_attackCollCount(),
		m_appearCount(),
		m_isInvincible(false)
	{}

public:

	virtual void HitFunc(EnemyHitKind hit_kind) = 0;
	virtual void Load(int model_handle) = 0;

	void ChangeAnime(int kind)
	{
		m_animeInfo.ChangeAnime(kind);
	}

	virtual void ChangeAnimeToState() = 0;

	virtual void Reset() = 0;

	virtual void Appear() = 0;
	virtual void Appear(VECTOR start_pos, bool is_barrier = false) = 0;

	bool AppearStep();

	//�U�����莞��(�~���b)
	void LetsAttackColl(int time, float add, float radius);
	void StepAttackColl();

	//��Ԃ��Ƃ̏���
	virtual void ExecIdle() = 0;
	virtual void ExecAvoid() = 0;
	virtual void ExecWalk() = 0;
	virtual void ExecTrack() = 0;
	virtual void ExecAttackReady() = 0;
	virtual void ExecAttack() = 0;
	virtual void ExecHit() = 0;
	virtual void ExecDown() = 0;
	void (CEnemy::*ExecFunc[ENEMY_STATE_NUM])() =
	{
		&CEnemy::ExecIdle,
		&CEnemy::ExecAvoid,
		&CEnemy::ExecWalk,
		&CEnemy::ExecTrack,
		&CEnemy::ExecAttackReady,
		&CEnemy::ExecAttack,
		&CEnemy::ExecHit,
		&CEnemy::ExecDown,
	};


	//�����蔻�肷�邩�ǂ���(true�Ȃ瓖���蔻����Ƃ�)
	virtual bool JudgeCollission(EnemyHitKind kind) = 0;
	virtual void Killing() = 0;

	//�v���C���[�̕�����������
	void FacingPlayer();
	void FacingSlowPlayer();
	void FacingSlowPlayer(float r);

	bool IsFacingPlayer();

	void NormalizeRot();

	void SetBarrier(VECTOR scale = { 1.f,1.f,1.f });


	/*
		Getter&Setter
	*/

	virtual VECTOR GetCenterPos() = 0;

	int GetCreateNum()const { return m_createNum; }
	void SetCreateNum(const int num) { m_createNum = num; }

	EnemyKind GetEnemyKind()const { return m_enemyKind; }
	void SetEnemyKind(const EnemyKind kind) { m_enemyKind = kind; }

	bool GetIsTargeting()const { return m_isTargeting; }
	void SetIsTargeting(const bool is_targeting) { m_isTargeting = is_targeting; }

	float GetHorizonRadius()const { return m_horizonRadius; }
	void SetHorizonRadius(const float radius) { m_horizonRadius = radius; }

	bool GetIsInvincible()const { return m_isInvincible; }
	void SetIsInvincible(bool flg) { m_isInvincible = flg; }

	bool GetIsAlive()const { return m_isAlive; }
	void SetIsAlive(const bool is_alive) { m_isAlive = is_alive; }

	float GetAttackRange()const { return m_attackRange; }
	void SetAttackRange(const float range) { m_attackRange = range; }

	virtual float GetAttackRadius() = 0;
	virtual VECTOR GetAttackCenterPos() = 0;

	bool GetIsAttack()const { return m_isAttack; }
	void SetIsAttack(const bool is_attack) { m_isAttack = is_attack; }

	float GetSpeed()const { return m_speed; }
	void SetSpeed(float speed) { m_speed = speed; }

	float GetEnemyHitRadius()const { return m_enemyHitRadius; }
	void SetEnemyHitRadius(const float radius) { m_enemyHitRadius = radius; }

	bool GetIsAppear()const { return m_isAppear; }
	void SetIsAppear(const bool is_appear) { m_isAppear = is_appear; }

	int GetAppearCount()const { return m_appearCount; }
	void SetAppearCount(const int stay_count) { m_appearCount = stay_count; }

	bool GetIsNotCollision()const { return m_isNotCollision; }
	void SetIsNotCollision(const bool is_collision) { m_isNotCollision = is_collision; }

	int GetBarrierEffectID()const { return m_barrierEffectID; }
	int GetBarrierHP()const { return m_barrierHP; }
	int GetBarrierMaxHP()const { return m_barrierMaxHP; }

	//���
	EnemyState GetState()const { return m_state; }
	void SetState(const EnemyState state) { m_state = state; }

	virtual int GetAttackKind() = 0;
};