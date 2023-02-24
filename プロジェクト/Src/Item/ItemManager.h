#pragma once

#include "../Common.h"

#include <vector>

#include "Item.h"

#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"

using namespace std;

class CPlayer; class CEnemy;

//Item管理クラス
class CItemManager : public CManagerBase
{
public:
	typedef void (CItemManager::*MMFunc)();

private:
	static CItemManager* m_itemManager;	//インスタンス

	vector<CItem*> m_ItemVec;				//アイテム

	CItemManager()
	{
		m_kind = MG_KIND_ITEM;
		m_ItemVec.clear();
	}

public:
	void Init();			//初期化
	void Load();			//ロード
	void Step();			//ステップ
	void Update();			//アップデート
	void Fin();				//後処理


	//マネージャー生成
	static CItemManager* CreateManager()
	{
		//ないならつくる
		if (!m_itemManager)
		{
			m_itemManager = _NEW(CItemManager);

			//リストに追加
			CGameManager::AddMGList(m_itemManager);
		}

		return m_itemManager;
	}
	//マネージャー取得
	static CItemManager* GetManager()
	{
		if (m_itemManager)
		{
			return m_itemManager;
		}

		return NULL;
	}
	//マネージャー削除
	void DeleteManager();


	//アイテム生成
	void CreateItem(ItemKind item_kind, int create_num);

	//アイテムすべて削除
	void DeleteAllItem();

	//アイテム発射処理
	static void RequestItem(ItemKind kind);

	/*
		Getter&Setter
	*/

	//プレイヤーアイテム
	vector<CItem*>* GetItemVec() { return &m_ItemVec; }
};
