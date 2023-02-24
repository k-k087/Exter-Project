#pragma once

#include "../Boss.h"

#include "../../Enemy.h"

#include "../../../../Draw/DrawManager.h"
#include "../../../../Common.h"

static const float BOSS2_CENTER_Y = 36.f;

static const float BOSS2_JUMP_CENTER_ADD_Y = 40.f;

//アニメ種類
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
	BOSS2_ATTACK_KICK1,		//キック１
	BOSS2_ATTACK_KICK2,		//キック2
	BOSS2_ATTACK_JUMP,		//ジャンプ攻撃
	BOSS2_ATTACK_SPECIAL,	//特殊攻撃

	BOSS2_ATTACK_NUM,
};



//ゴーレム敵クラス
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

	//点滅処理
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

	//攻撃処理
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

	//特殊攻撃関連
	void AttackSpecialRand(float f);
	void AttackSpecialHoming();
	void AttackSpecialHomingRand(float f);

public:
	void Init()		override;			//初期化
	void Load()		override;			//ロード
	void Load(int model_handle) override;
	void Step()		override;			//ステップ
	void Update()	override;			//データ反映処理
	void Draw()		override;			//描画
	void Fin()		override;			//後処理

	void Reset();

	//アニメ変更
	void ChangeAnime(Boss2AnimeKind kind);
	void ChangeAnimeToState();

	//当たり判定処理
	void HitFunc(EnemyHitKind hit_kind);

	//当たり判定するかどうか(trueなら当たり判定をとる)
	bool JudgeCollission(EnemyHitKind kind);


	//生成
	static CBoss2* Create(int create_num)
	{
		CBoss2* golem_enemy = _NEW(CBoss2);

		golem_enemy->Init();
		golem_enemy->SetCreateNum(create_num);

		//DrawListに追加
		golem_enemy->m_drawID = CDrawManager::AddData(golem_enemy);

		return golem_enemy;
	}


	/*
		Getter&Setter
	*/

	//中心座標
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


	//スピード
	float GetSpeed()const { return m_speed; }
	void SetSpeed(float speed) { m_speed = speed; }


	float GetAttackRadius();
	VECTOR GetAttackCenterPos();


	AttackType GetAttackType();

	int GetAttackKind();
};


