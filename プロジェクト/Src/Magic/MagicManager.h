#pragma once

#include "../Common.h"

#include <vector>

#include "Magic.h"

#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"

using namespace std;

class CPlayer; class CEnemy;

//Magic管理クラス
class CMagicManager : public CManagerBase
{
public:
	typedef void (CMagicManager::*MMFunc)();

private:
	static CMagicManager* m_magicManager;	//インスタンス

	int				m_magicEffectID[MAGIC_KIND_NUM];

	vector<CMagic*> m_playerMagic;	//プレイヤーマジック
	vector<CMagic*> m_enemyMagic;	//敵マジック

	CMagicManager() :
		m_magicEffectID{}
	{
		m_kind = MG_KIND_MAGIC;
		m_playerMagic.clear();
		m_enemyMagic.clear();
	}

public:
	void Init();			//初期化
	void Load();			//ロード
	void Step();			//ステップ
	void Update();			//アップデート
	void Fin();				//後処理]
 


	//マネージャー生成
	static CMagicManager* CreateManager()
	{
		//ないならつくる
		if (!m_magicManager)
		{
			m_magicManager = _NEW(CMagicManager);

			//リストに追加
			CGameManager::AddMGList(m_magicManager);
		}

		return m_magicManager;
	}
	//マネージャー取得
	static CMagicManager* GetManager()
	{
		if (m_magicManager)
		{
			return m_magicManager;
		}

		return NULL;
	}
	//マネージャー削除
	void DeleteManager();


	//魔法生成
	void CreatePlayerMagic();
	void CreateEnemyMagic();



	//魔法すべて削除
	void DeleteAllMagic();

	//魔法発射処理
	static void RequestPlayerMagic(MagicRequestInfo& player, MagicKind kind);
	static void RequestEnemyMagic(MagicRequestInfo& enemy, MagicKind kind);


	/*
		Getter&Setter
	*/

	//プレイヤー魔法
	vector<CMagic*>* GetPlayerMagic() { return &m_playerMagic; }

	//敵魔法
	vector<CMagic*>* GetEnemyMagic() { return &m_enemyMagic; }

	//エフェクトID
	int GetMagicEffectID(MagicKind kind)const { return m_magicEffectID[kind]; }
};
