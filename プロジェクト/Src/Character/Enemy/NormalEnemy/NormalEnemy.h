#pragma once

#include "DxLib.h"

#include "../Enemy.h"
#include "../../../Draw/DrawManager.h"

#include "../../../Common.h"

static const float NORMAL_ENEMY_CENTER_Y = 18.f;

//敵アニメ種類
enum NormalEnemyAnimeKind
{
	NORMAL_ENEMY_ANIME_KIND_NONE = -1,

	NORMAL_ENEMY_ANIME_KIND_ATTACK,
	NORMAL_ENEMY_ANIME_KIND_DOWN,
	NORMAL_ENEMY_ANIME_KIND_HIT,
	NORMAL_ENEMY_ANIME_KIND_IDLE,
	NORMAL_ENEMY_ANIME_KIND_WALK,

	NORMAL_ENEMY_ANIME_KIND_NUM,
};


//通常敵クラス
class CNormalEnemy : public CEnemy
{

private:
	int			m_attackCount;			//攻撃カウント用変数

	CNormalEnemy() : 
		m_attackCount()
	{
		m_kind = OBJECT_KIND_PLAYER;
		m_enemyKind = ENEMY_KIND_NORMAL;
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
	void ChangeAnime(NormalEnemyAnimeKind kind);
	void ChangeAnimeToState();

	//当たり判定処理
	void HitFunc(EnemyHitKind hit_kind);

	//当たり判定するかどうか(trueなら当たり判定をとる)
	bool JudgeCollission(EnemyHitKind kind);

	void Appear();
	void Appear(VECTOR start_pos, bool is_barrier = false);

	void Killing();


	//生成
	static CNormalEnemy* Create(int create_num)
	{
		CNormalEnemy* normal_enemy = _NEW(CNormalEnemy);

		normal_enemy->Init();
		normal_enemy->SetCreateNum(create_num);

		//DrawListに追加
		normal_enemy->m_drawID = CDrawManager::AddData(normal_enemy);

		return normal_enemy;
	}


	/*
		Getter&Setter
	*/

	//中心座標
	VECTOR GetCenterPos() {
		VECTOR center_pos = m_pos;
		center_pos.y += NORMAL_ENEMY_CENTER_Y;
		return center_pos;
	}

	//Hp
	int GetHP()const { return m_hp; }
	void SetHP(int hp) { m_hp = hp; }


	float GetAttackRadius();
	VECTOR GetAttackCenterPos();


	AttackType GetAttackType();

	int GetAttackKind() { return -1; }
};
