#pragma once

#include "DxLib.h"

#include "Character.h"

#include "../GameManager/ManagerBase.h"
#include "CharacterCommon.h"

class CPlayer;

//キャラクターマネージャークラス
class CCharacterManager : public CManagerBase
{
public:
	//キャラクターマネージャー種類
	enum CharaMGKind
	{
		CHARA_MG_KIND_NONE = -1,

		CHARA_MG_KIND_PLAYER,	//プレイヤー
		CHARA_MG_KIND_ENEMY,	//敵

		CHARA_MG_KIND_NUM,
	};
private:
	static CCharacterManager* m_characterManager;	//マネージャー実態
	CCharacterManager* m_Manager[CHARA_MG_KIND_NUM];	//各キャラマネージャー

protected:
	CharaMGKind m_charaKind;	//キャラ種類

	CCharacterManager() :
		m_Manager{ NULL }
	{
		m_kind = MG_KIND_CHARACTER;
	}

public:
	void Step() override;	//ステップ
	void Init();			//初期化
	void Load();			//ロード
	void Fin();				//後処理

	//マネージャー生成
	static CCharacterManager* CreateManager();
	//マネージャー取得
	static CCharacterManager* GetManager();
	//マネージャー削除
	void DeleteManager();

	//追加
	static void AddManager(CCharacterManager* manager);
	//削除
	void DeleteAllManager();

	//敵生成
	static void CreateEnemy();
	//敵の先頭情報取得


	/*
		Getter&Setter
	*/

	//種類
	CharaMGKind GetCharaKind()
	{
		return m_charaKind;
	}
	void SetCharaKind(CharaMGKind kind)
	{
		m_charaKind = kind;
	}

	//特定のマネージャー
	CCharacterManager* GetCharaManager(CharaMGKind kind)
	{
		return m_Manager[kind];
	}
	void SetCharaManager(CCharacterManager* manager)
	{
		CharaMGKind kind = manager->GetCharaKind();
		m_Manager[kind] = manager;
	}
};