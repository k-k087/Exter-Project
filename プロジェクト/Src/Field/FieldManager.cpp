#include "DxLib.h"

#include <cassert>

#include "Field.h"
#include "FieldManager.h"

#include "../Debug/Debug.h"
#include "../Input/InputManager.h"
#include "../Character/Player/Player.h"
#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"
#include "../Common.h"


//�}�l�[�W���[�錾
CFieldManager* CFieldManager::m_fieldManager = nullptr;

//�J�����}�l�[�W���[�N���X����
CFieldManager* CFieldManager::CreateManager()
{
	//���݂��Ȃ���΂���
	if (!m_fieldManager)
	{
		m_fieldManager = _NEW(CFieldManager);

		//�}�l�[�W���[���X�g�ɒǉ�
		CGameManager::AddMGList(m_fieldManager);
	}

	return m_fieldManager;
}

//������
void CFieldManager::Init()
{
	
	m_field = CField::Create();
	m_field->Init();
}

//���[�h
void CFieldManager::Load()
{
	m_field->Load();
	
}

//�X�e�b�v
void CFieldManager::Step()
{
#ifdef DEBUG_MODE

#endif
	m_field->Step();
	
}

//�`��
void CFieldManager::Draw()
{

}

CFieldManager* CFieldManager::GetManager()
{
	if (!m_fieldManager) { return nullptr; }

	return m_fieldManager;
}

//�}�l�[�W���[�폜
void CFieldManager::DeleteManager()
{
	if (!m_fieldManager) { return; }

	//�����c���Ă���Ȃ����
	
	int handle = m_field->GetHandle();
	if (handle != -1)
	{
		MV1DeleteModel(handle);
		m_field->SetHandle(-1);
	}

	if (m_field)
		_DELETE(m_field);

	m_field = nullptr;
	
	

	//�Ō�Ƀ}�l�[�W���[������
	CGameManager::DeleteMGList(m_fieldManager);
	_DELETE(m_fieldManager);

	m_fieldManager = nullptr;
}