#pragma once

#include "../Common.h"

#include <vector>

#include "Magic.h"

#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"

using namespace std;

class CPlayer; class CEnemy;

//Magic�Ǘ��N���X
class CMagicManager : public CManagerBase
{
public:
	typedef void (CMagicManager::*MMFunc)();

private:
	static CMagicManager* m_magicManager;	//�C���X�^���X

	int				m_magicEffectID[MAGIC_KIND_NUM];

	vector<CMagic*> m_playerMagic;	//�v���C���[�}�W�b�N
	vector<CMagic*> m_enemyMagic;	//�G�}�W�b�N

	CMagicManager() :
		m_magicEffectID{}
	{
		m_kind = MG_KIND_MAGIC;
		m_playerMagic.clear();
		m_enemyMagic.clear();
	}

public:
	void Init();			//������
	void Load();			//���[�h
	void Step();			//�X�e�b�v
	void Update();			//�A�b�v�f�[�g
	void Fin();				//�㏈��]
 


	//�}�l�[�W���[����
	static CMagicManager* CreateManager()
	{
		//�Ȃ��Ȃ����
		if (!m_magicManager)
		{
			m_magicManager = _NEW(CMagicManager);

			//���X�g�ɒǉ�
			CGameManager::AddMGList(m_magicManager);
		}

		return m_magicManager;
	}
	//�}�l�[�W���[�擾
	static CMagicManager* GetManager()
	{
		if (m_magicManager)
		{
			return m_magicManager;
		}

		return NULL;
	}
	//�}�l�[�W���[�폜
	void DeleteManager();


	//���@����
	void CreatePlayerMagic();
	void CreateEnemyMagic();



	//���@���ׂč폜
	void DeleteAllMagic();

	//���@���ˏ���
	static void RequestPlayerMagic(MagicRequestInfo& player, MagicKind kind);
	static void RequestEnemyMagic(MagicRequestInfo& enemy, MagicKind kind);


	/*
		Getter&Setter
	*/

	//�v���C���[���@
	vector<CMagic*>* GetPlayerMagic() { return &m_playerMagic; }

	//�G���@
	vector<CMagic*>* GetEnemyMagic() { return &m_enemyMagic; }

	//�G�t�F�N�gID
	int GetMagicEffectID(MagicKind kind)const { return m_magicEffectID[kind]; }
};
