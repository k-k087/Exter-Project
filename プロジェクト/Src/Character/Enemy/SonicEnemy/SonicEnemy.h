#pragma once

#include "DxLib.h"

#include "../Enemy.h"
#include "../../../Draw/DrawManager.h"

#include "../../../Common.h"

static const float SONIC_ENEMY_CENTER_Y = 25.f;

//アニメ種類
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

	SONIC_ENEMY_ATTACK_HIT,		//ヒット
	SONIC_ENEMY_ATTACK_BEAM,	//ビーム

	SONIC_ENEMY_ATTACK_NUM,
};


//ソニック敵クラス
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

	//攻撃処理
	void ExecAttackHit();
	void ExecAttackBeam();
	void (CSonicEnemy::* ExecAttackFunc[SONIC_ENEMY_ATTACK_NUM])() =
	{
		&CSonicEnemy::ExecAttackHit,
		&CSonicEnemy::ExecAttackBeam,
	};

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
	void ChangeAnime(SonicEnemyAnimeKind kind);
	void ChangeAnimeToState();

	//当たり判定処理
	void HitFunc(EnemyHitKind hit_kind);

	//当たり判定するかどうか(trueなら当たり判定をとる)
	bool JudgeCollission(EnemyHitKind kind);

	void Appear();
	void Appear(VECTOR start_pos, bool is_barrier = false);

	void Killing();


	//生成
	static CSonicEnemy* Create(int create_num)
	{
		CSonicEnemy* sonic_enemy = _NEW(CSonicEnemy);

		sonic_enemy->Init();
		sonic_enemy->SetCreateNum(create_num);

		//DrawListに追加
		sonic_enemy->m_drawID = CDrawManager::AddData(sonic_enemy);

		return sonic_enemy;
	}


	/*
		Getter&Setter
	*/

	//中心座標
	VECTOR GetCenterPos() {
		VECTOR center_pos = m_pos;
		center_pos.y += SONIC_ENEMY_CENTER_Y;
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

	int GetAttackKind() { return m_attackKind; }
};

