#pragma once

#include "DxLib.h"

#include "../Enemy.h"
#include "../../../Draw/DrawManager.h"

#include "../../../Common.h"

static const float GOLEM_ENEMY_CENTER_Y = 25.f;

//アニメ種類
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

	GOLEM_ENEMY_ATTACK_HIT,		//ヒット
	GOLEM_ENEMY_ATTACK_MAGIC,	//魔法

	GOLEM_ENEMY_ATTACK_NUM,
};


//ゴーレム敵クラス
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
	void ExecAttackMagic();
	void (CGolemEnemy::* ExecAttackFunc[GOLEM_ENEMY_ATTACK_NUM])() =
	{
		&CGolemEnemy::ExecAttackHit,
		&CGolemEnemy::ExecAttackMagic,
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
	void ChangeAnime(GolemEnemyAnimeKind kind);
	void ChangeAnimeToState();

	//当たり判定処理
	void HitFunc(EnemyHitKind hit_kind);

	//当たり判定するかどうか(trueなら当たり判定をとる)
	bool JudgeCollission(EnemyHitKind kind);

	void Killing();

	//生成
	static CGolemEnemy* Create(int create_num)
	{
		CGolemEnemy* golem_enemy = _NEW(CGolemEnemy);

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
		center_pos.y += GOLEM_ENEMY_CENTER_Y;
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

