#include "DxLib.h"

#include <cassert>

#include "CharacterManager.h"
#include "Character.h"

#include "Player/Player.h"
#include "Player/PlayerManager.h"
#include "Enemy/EnemyManager.h"

#include "../Debug/Debug.h"
#include "../GameManager/GameManager.h"
#include "../Effekseer/Effekseer.h"

#include "../Common.h"


CCharacterManager* CCharacterManager::m_characterManager = nullptr;

//������
void CCharacterManager::Init()
{

}

//���[�h
void CCharacterManager::Load()
{

}

//�X�e�b�v
void CCharacterManager::Step()
{
	for (int chara_index = 0; chara_index < CHARA_MG_KIND_NUM; chara_index++)
	{
		//�}�l�[�W���[������Ȃ�Step��
		if (m_Manager[chara_index])
		{
			m_Manager[chara_index]->Step();
		}
	}
}

//�㏈��
void CCharacterManager::Fin()
{

}

//�}�l�[�W���[����
CCharacterManager* CCharacterManager::CreateManager()
{
	//�Ȃ��Ȃ���
	if (!m_characterManager)
	{
		//����
		m_characterManager = _NEW(CCharacterManager);

		//�}�l�[�W���[���X�g�ɒǉ�����
		CGameManager::AddMGList(m_characterManager);
	}

	return m_characterManager;
}

//�}�l�[�W���[�擾
CCharacterManager* CCharacterManager::GetManager()
{
	if (!m_characterManager) { return nullptr; }

	return m_characterManager;
}

//�ǉ�
void CCharacterManager::AddManager(CCharacterManager* manager)
{
	CharaMGKind kind = manager->GetCharaKind();

	//���݂��Ȃ��Ȃ�ǉ�����
	if (!m_characterManager->GetCharaManager(kind))
	{
		m_characterManager->SetCharaManager(manager);
	}
}

//���ׂẴ}�l�[�W���[�폜
void CCharacterManager::DeleteAllManager()
{
	if (!m_characterManager) { return; }

	CPlayerManager::GetManager()->DeleteManager();
	CEnemyManager::GetManager()->DeleteManager();

	//�Ō�ɃL�����}�l�[�W���[���g���폜
	CGameManager::DeleteMGList(m_characterManager);
	_DELETE(m_characterManager);

	m_characterManager = nullptr;
}


//�G����
void CCharacterManager::CreateEnemy()
{

}

//�G�̐擪���擾
