#pragma once

#include "../Character.h"

#include "../../Anime/Anime.h"


//状態
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

//敵のヒット種類
enum EnemyHitKind
{
	ENEMY_HIT_NONE = -1,

	ENEMY_HIT_PLAYER,				//敵とプレイヤー
	ENEMY_HIT_ENEMY,				//敵と敵
	ENEMY_HIT_PLAYER_ATTACK,		//敵とプレイヤーの攻撃

	ENEMY_HORIZON_HIT_PLAYER,		//敵の視界とプレイヤー
	ENEMY_RANGE_HIT_PLAYER,			//敵攻撃範囲とプレイヤー
	ENEMY_ATTACK_HIT_PLAYER,		//敵攻撃とプレイヤー

	ENEMY_HIT_NUM,
};

//敵種類
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

//エネミー基底クラス
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
	int			m_invincibleVal;		//無敵間隔
	int			m_createNum;			//何番目に生成されたか
	int			m_appearCount;			//停止カウント
	int			m_attackCollCount;		//攻撃判定カウント

	int			m_barrierEffectID;		//バリアエフェクトハンドルID
	int			m_barrierHP;			//バリアHP
	int			m_barrierMaxHP;			//バリアマックスHP

	float		m_speed;				//スピード
	float		m_attackRange;			//攻撃範囲
	float		m_enemyHitRadius;		//敵との当たり判定用半径
	float		m_horizonRadius;		//視界半径

	bool		m_isTargeting;			//ターゲットされているか
	bool		m_isInvincible;			//無敵状態
	bool		m_isNotCollision;		//当たり判定をとらない
	bool		m_isAttack;				//攻撃判定
	bool		m_isAppear;				//出現しているか
	bool		m_isDraw;				//描画するか

	EnemyKind	m_enemyKind;			//敵種類
	EnemyState	m_state;				//状態
	CAnimeInfo	m_animeInfo;			//アニメ情報

	AttackData	m_attackData;			//攻撃情報

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

	//攻撃判定時間(ミリ秒)
	void LetsAttackColl(int time, float add, float radius);
	void StepAttackColl();

	//状態ごとの処理
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


	//当たり判定するかどうか(trueなら当たり判定をとる)
	virtual bool JudgeCollission(EnemyHitKind kind) = 0;
	virtual void Killing() = 0;

	//プレイヤーの方を向く処理
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

	//状態
	EnemyState GetState()const { return m_state; }
	void SetState(const EnemyState state) { m_state = state; }

	virtual int GetAttackKind() = 0;
};