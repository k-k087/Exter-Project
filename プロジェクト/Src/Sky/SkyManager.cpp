#include "DxLib.h"

#include <cassert>

#include "Sky.h"
#include "SkyManager.h"

#include "../Debug/Debug.h"
#include "../Input/InputManager.h"
#include "../Character/Player/Player.h"
#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"
#include "../Common.h"


//�}�l�[�W���[�錾
CSkyManager* CSkyManager::m_skyManager = NULL;

//�J�����}�l�[�W���[�N���X����
CSkyManager* CSkyManager::CreateManager()
{
	//���݂��Ȃ���΂���
	if (!m_skyManager)
	{
		m_skyManager = _NEW(CSkyManager);

		//�}�l�[�W���[���X�g�ɒǉ�
		CGameManager::AddMGList(m_skyManager);
	}

	return m_skyManager;
}



//������
void CSkyManager::Init()
{
	//���݂��Ȃ���΍��
	if (!m_sky)
	{
		m_sky = CSky::Create();
	}

	//�N���X�̏�������
	m_sky->Init();
}

//���[�h
void CSkyManager::Load()
{
	m_sky->Load();
}

//�X�e�b�v
void CSkyManager::Step()
{
#ifdef DEBUG_MODE

#endif

	m_sky->Step();
	m_sky->Update();
}

//�`��
void CSkyManager::Draw()
{
	m_sky->Draw();
}

CSkyManager* CSkyManager::GetManager()
{
	if (!m_skyManager) { return nullptr; }

	return m_skyManager;
}

//�}�l�[�W���[�폜
void CSkyManager::DeleteManager()
{
	if (!m_skyManager) { return; }

	//�����c���Ă���Ȃ����
	if (m_sky)
	{
		//��ɉ摜�폜
		if (m_sky->GetHandle() != -1)
		{
			MV1DeleteModel(m_sky->GetHandle());

			m_sky->SetHandle(-1);
		}

		_DELETE(m_sky);
	}

	//�Ō�Ƀ}�l�[�W���[������
	CGameManager::DeleteMGList(m_skyManager);
	_DELETE(m_skyManager);

	m_skyManager = nullptr;
}

