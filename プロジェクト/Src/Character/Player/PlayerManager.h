#pragma once

#include "DxLib.h"

#include "../CharacterManager.h"
#include "Player.h"

//プレイヤー管理クラス
class CPlayerManager : public CCharacterManager
{
private:
	static CPlayerManager* m_playerManager;
	CPlayer* m_player;

	CPlayerManager() :
		m_player(NULL)
	{
		m_charaKind= CHARA_MG_KIND_PLAYER;
	}

public:
	~CPlayerManager();

	void Init();			//初期化
	void Load();			//ロード
	void Step() override;	//ステップ
	void StepStartPhase();
	void Fin();				//後処理

	//マネージャー生成
	static CPlayerManager* CreateManager();
	//マネージャー取得
	static CPlayerManager* GetManager();
	//マネージャー削除
	void DeleteManager();

	//プレイヤー生成
	static void CreatePlayer();
	//プレイヤー削除
	static void DeletePlayer();

	/*
		Getter&Setter
	*/

	CPlayer* GetPlayer()
	{
		if (m_player)
		{
			return m_player;
		}

		return NULL;
	}
	void SetPlayer(CPlayer* player)
	{
		m_player = player;
	}

};
