#include "DxLib.h"

#include <cassert>
#include <vector>

#include "Item.h"
#include "ItemManager.h"

#include "../GameManager/GameManager.h"
#include "../Effekseer.h"

#include "../Debug/Debug.h"

#include "../Common.h"


static const int PLAYER_ITEM_NUM = 10;

static const char* ITEM_PATH[ITEM_KIND_NUM] =
{
	"Data/Effect/ItemBullet.ef",
	" ",
};


//インスタンス
CItemManager* CItemManager::m_itemManager = nullptr;

void CItemManager::Init()
{
	CreateItem(ITEM_KIND_HP, 4);
	CreateItem(ITEM_KIND_MP, 4);

}
void CItemManager::Load()
{
	//モデルのロード
	int model_handle[ITEM_KIND_NUM] =
	{
		MV1LoadModel(ITEM_PATH[0]),
		MV1LoadModel(ITEM_PATH[1]),
	};

	//デュプリケートする
	for (const auto& item_vec : m_ItemVec)
		item_vec->Load(model_handle[item_vec->GetItemKind()]);

	//最後にDELETE
	for (int handle_index = 0; handle_index < ITEM_KIND_NUM; ++handle_index)
		MV1DeleteModel(model_handle[handle_index]);
}
void CItemManager::Step()
{
	for (const auto& item_vec : m_ItemVec)
		item_vec->Step();
}
void CItemManager::Update()
{
	for (const auto& item_vec : m_ItemVec)
		item_vec->Update();
}
void CItemManager::Fin()
{
	//ロード関連
	for (const auto& item_vec : m_ItemVec)
		item_vec->Fin();
}


//マネージャー削除
void CItemManager::DeleteManager()
{
	if (!m_itemManager) { return; }

	//まずは後処理
	Fin();

	//アイテム破棄
	DeleteAllItem();

	//最後に配列クリア
	m_ItemVec.clear();

	//マネージャーを消す
	CGameManager::DeleteMGList(m_itemManager);
	_DELETE(m_itemManager);

	m_itemManager = nullptr;
}


//アイテム生成
void CItemManager::CreateItem(ItemKind item_kind, int create_num)
{
	//生成し、リストに追加
	for (int bullet_index = 0; bullet_index < create_num; ++bullet_index)
	{
		m_ItemVec.push_back(CItem::Create(item_kind));
	}
}

//アイテム発射処理
void CItemManager::RequestItem(ItemKind kind)
{
	//空いてるところを探す
	for (auto & item_vec : *m_itemManager->GetItemVec())
	{
		if (!item_vec->GetIsUse())
		{


			//item_vec->Request();
			break;
		}
	}
}

//すべて削除
void CItemManager::DeleteAllItem()
{
	//中身も消す
	if (m_ItemVec.size() != 0)
	{
		std::vector<CItem*>::iterator player_item = m_ItemVec.begin();

		for (; player_item != m_ItemVec.end(); ++player_item)
		{
			//念のため後処理
			(*player_item)->Fin();

			_DELETE(*player_item);
		}
	}

}
