#pragma once

#include "DxLib.h"

#include "../../Common.h"

#include "../CharacterManager.h"
#include "Enemy.h"

#include "list"


using namespace std;

enum EnemyEffectKind
{
	ENEMY_EFFECT_KIND_NONE = -1,

	ENEMY_EFFECT_KIND_BARIIER,
	ENEMY_EFFECT_KIND_FIRE,
	ENEMY_EFFECT_KIND_MAGIC_APPEAR,
	ENEMY_EFFECT_KIND_RED_BEAM,
	ENEMY_EFFECT_KIND_SONIC_BOOM1,
	ENEMY_EFFECT_KIND_BIG_BLUE_GREEN,
	ENEMY_EFFECT_KIND_BLOW,

	ENEMY_EFFECT_KIND_NUM
};

//敵管理クラス
class CEnemyManager : public CCharacterManager
{
private:
	static CEnemyManager*	m_enemyManager;
	int						m_createCount;
	int						m_aliveNum;
	int						m_oldRank;
	int						m_currentRank;
	int						m_effectID[ENEMY_EFFECT_KIND_NUM];	//エフェクトID

	bool					m_paramB[5];
	bool					m_warningFlg;
	bool					m_warningFin;
	int						m_rankTime;

	list<CEnemy*>			m_enemyList;

	CEnemy*					m_bossEnemyP[2];

	CEnemyManager() :
		m_createCount(0),
		m_rankTime(0),
		m_paramB{ false },
		m_warningFlg(false),
		m_aliveNum(),
		m_effectID{ ENEMY_EFFECT_KIND_NONE },
		m_bossEnemyP{ nullptr }
	{
		m_enemyList.clear();
		m_charaKind = CHARA_MG_KIND_ENEMY;
	}

public:
	void Init();			//初期化
	void Load();			//ロード
	void Step();			//ステップ
	void Fin();				//後処理

	//マネージャー生成
	static CEnemyManager* CreateManager()
	{
#ifdef DEBUG_MODE
		assert(!m_enemyManager);
#endif
		m_enemyManager = _NEW(CEnemyManager);

		//リストに追加
		AddManager(m_enemyManager);

		return m_enemyManager;
	}
	//マネージャー取得
	static CEnemyManager* GetManager()
	{
#ifdef DEBUG_MODE
		assert(m_enemyManager);
#endif
		return m_enemyManager;
	}
	//マネージャー削除
	void DeleteManager();

	//プレイヤー生成
	void CreateEnemy();
	//プレイヤー削除
	void DeleteEnemy();

	//出現させる
	void AppearEnemy(EnemyKind enemy_kind, int appear_num);
	void AppearEnemy(EnemyKind enemy_kind, VECTOR start_pos, bool is_barrier = false, bool boss_flg = false);
	void AppearAllEnemy();

	void StartAppear(int stage_num);

	void SubAliveNum() { m_aliveNum--; if (m_aliveNum < 0) { m_aliveNum = 0; } };

	int RequestEffect(EnemyEffectKind effect_kind, VECTOR pos, bool isLoop, int start_frame = 0);

	void StepRank();

	bool IsDownBoss();

	void KillAllEnemy();

	/*
		Getter&Setter
	*/

	list<CEnemy*>* GetEnemyList()
	{
		return &m_enemyList;
	}

	int GetCreateCount()const { return m_createCount; }

	CEnemy** GetBossEnemyPointer()
	{
		return m_bossEnemyP;
	}
	void SetBossEnemyPointer(CEnemy* boss_p)
	{
		for (int boss_index = 0; boss_index < 2; boss_index++)
		{
			if (m_bossEnemyP[boss_index]) { continue; }

			m_bossEnemyP[boss_index] = boss_p;
			break;
		}
	}

	int GetCurrentRank()const { return m_currentRank; }
	void SetCurrentRank(const int rank) { m_currentRank = rank; }

	bool GetWarningFlg()const { return m_warningFlg; }
	void SetWarningFlg(const bool flg) { m_warningFlg = flg; }

	bool GetWarningFin()const { return m_warningFin; }
	void SetWarningFin(const bool flg) { m_warningFin = flg; }
};
