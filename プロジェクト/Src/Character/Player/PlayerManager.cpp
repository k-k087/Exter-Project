#include "DxLib.h"

#include <cassert>

#include "PlayerManager.h"
#include "Player.h"

#include "../CharacterManager.h"

#include "../../Debug/Debug.h"
#include "../../Common.h"


CPlayerManager* CPlayerManager::m_playerManager = nullptr;

CPlayerManager::~CPlayerManager()
{
	/*DeleteManager();*/
}

void CPlayerManager::Init()
{

}

void CPlayerManager::Load()
{
	m_player->Load();
}

void CPlayerManager::Step()
{
	//�v���C���[�̃X�e�b�v��
	m_player->Step();
}

void CPlayerManager::StepStartPhase()
{
	m_player->StepStartPhase();
}

//�}�l�[�W���[����
CPlayerManager* CPlayerManager::CreateManager()
{
#ifdef DEBUG_MODE
	assert(!m_playerManager);
#endif
	m_playerManager = _NEW(CPlayerManager);

	//���X�g�ɒǉ�
	AddManager(m_playerManager);

	return m_playerManager;
}

//�}�l�[�W���[�擾
CPlayerManager* CPlayerManager::GetManager()
{
#ifdef DEBUG_MODE
	assert(m_playerManager);
#endif
	return m_playerManager;
}

//�}�l�[�W���[�폜
void CPlayerManager::DeleteManager()
{
	if (!m_playerManager) { return; }

	//�܂��͌㏈��
	Fin();
	
	if (m_playerManager)
	{
		//��ɒ��g�̃v���C���[�폜
		DeletePlayer();

		//�}�l�[�W���[�폜
		_DELETE(m_playerManager);

		m_playerManager = nullptr;
	}
}

void CPlayerManager::Fin()
{
	//���[�h�֘A�폜
	if (m_player->GetHandle() != -1)
	{
		MV1DeleteModel(m_player->GetHandle());

		m_player->SetHandle(-1);
	}
	if (m_player->GetSwordHandle() != -1)
	{
		MV1DeleteModel(m_player->GetSwordHandle());
		m_player->SetSwordHandle(-1);
	}
}

//�v���C���[����
void CPlayerManager::CreatePlayer()
{
	CPlayer* new_player = CPlayer::Create();

	//���𔽉f
	m_playerManager->SetPlayer(new_player);
}

//�v���C���[�폜
void CPlayerManager::DeletePlayer()
{
	if (m_playerManager->GetPlayer())
	{
		_DELETE(m_playerManager->GetPlayer());
		m_playerManager->SetPlayer(nullptr);
	}
}