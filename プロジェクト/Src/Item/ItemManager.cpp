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


//�C���X�^���X
CItemManager* CItemManager::m_itemManager = nullptr;

void CItemManager::Init()
{
	CreateItem(ITEM_KIND_HP, 4);
	CreateItem(ITEM_KIND_MP, 4);

}
void CItemManager::Load()
{
	//���f���̃��[�h
	int model_handle[ITEM_KIND_NUM] =
	{
		MV1LoadModel(ITEM_PATH[0]),
		MV1LoadModel(ITEM_PATH[1]),
	};

	//�f���v���P�[�g����
	for (const auto& item_vec : m_ItemVec)
		item_vec->Load(model_handle[item_vec->GetItemKind()]);

	//�Ō��DELETE
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
	//���[�h�֘A
	for (const auto& item_vec : m_ItemVec)
		item_vec->Fin();
}


//�}�l�[�W���[�폜
void CItemManager::DeleteManager()
{
	if (!m_itemManager) { return; }

	//�܂��͌㏈��
	Fin();

	//�A�C�e���j��
	DeleteAllItem();

	//�Ō�ɔz��N���A
	m_ItemVec.clear();

	//�}�l�[�W���[������
	CGameManager::DeleteMGList(m_itemManager);
	_DELETE(m_itemManager);

	m_itemManager = nullptr;
}


//�A�C�e������
void CItemManager::CreateItem(ItemKind item_kind, int create_num)
{
	//�������A���X�g�ɒǉ�
	for (int bullet_index = 0; bullet_index < create_num; ++bullet_index)
	{
		m_ItemVec.push_back(CItem::Create(item_kind));
	}
}

//�A�C�e�����ˏ���
void CItemManager::RequestItem(ItemKind kind)
{
	//�󂢂Ă�Ƃ����T��
	for (auto & item_vec : *m_itemManager->GetItemVec())
	{
		if (!item_vec->GetIsUse())
		{


			//item_vec->Request();
			break;
		}
	}
}

//���ׂč폜
void CItemManager::DeleteAllItem()
{
	//���g������
	if (m_ItemVec.size() != 0)
	{
		std::vector<CItem*>::iterator player_item = m_ItemVec.begin();

		for (; player_item != m_ItemVec.end(); ++player_item)
		{
			//�O�̂��ߌ㏈��
			(*player_item)->Fin();

			_DELETE(*player_item);
		}
	}

}
