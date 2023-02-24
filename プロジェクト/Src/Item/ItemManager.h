#pragma once

#include "../Common.h"

#include <vector>

#include "Item.h"

#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"

using namespace std;

class CPlayer; class CEnemy;

//Item�Ǘ��N���X
class CItemManager : public CManagerBase
{
public:
	typedef void (CItemManager::*MMFunc)();

private:
	static CItemManager* m_itemManager;	//�C���X�^���X

	vector<CItem*> m_ItemVec;				//�A�C�e��

	CItemManager()
	{
		m_kind = MG_KIND_ITEM;
		m_ItemVec.clear();
	}

public:
	void Init();			//������
	void Load();			//���[�h
	void Step();			//�X�e�b�v
	void Update();			//�A�b�v�f�[�g
	void Fin();				//�㏈��


	//�}�l�[�W���[����
	static CItemManager* CreateManager()
	{
		//�Ȃ��Ȃ����
		if (!m_itemManager)
		{
			m_itemManager = _NEW(CItemManager);

			//���X�g�ɒǉ�
			CGameManager::AddMGList(m_itemManager);
		}

		return m_itemManager;
	}
	//�}�l�[�W���[�擾
	static CItemManager* GetManager()
	{
		if (m_itemManager)
		{
			return m_itemManager;
		}

		return NULL;
	}
	//�}�l�[�W���[�폜
	void DeleteManager();


	//�A�C�e������
	void CreateItem(ItemKind item_kind, int create_num);

	//�A�C�e�����ׂč폜
	void DeleteAllItem();

	//�A�C�e�����ˏ���
	static void RequestItem(ItemKind kind);

	/*
		Getter&Setter
	*/

	//�v���C���[�A�C�e��
	vector<CItem*>* GetItemVec() { return &m_ItemVec; }
};
