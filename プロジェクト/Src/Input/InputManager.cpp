#include "DxLib.h"

#include <cassert>

#include "Input.h"
#include "InputManager.h"
#include "KeyBoard/KeyBoard.h"
#include "Mouse/Mouse.h"
#include "Pad/Pad.h"

#include "../Debug/Debug.h"
#include "../GameManager/GameManager.h"

#include "../Common.h"


//�C���v�b�g�}�l�[�W���[�錾
CInputManager* CInputManager::m_inputManager = NULL;

//�}�l�[�W���[����
void CInputManager::CreateManager()
{
	//�Ȃ��Ȃ���
	if (!m_inputManager)
	{
		m_inputManager = _NEW(CInputManager);
		
		//CGameManager::AddMGList(m_inputManager);

		//��������
		m_inputManager->Init();
	}
}

//������
void CInputManager::Init()
{
	//���̓f�o�C�X���ƂɃC���X�^���X����
	m_input[I_KEY] = _NEW(CKeyBoard);
	m_input[I_MOUSE] = CMouse::CreateMouse();
	m_input[I_PAD] = CPad::CreatePad();

	//���ꂼ��̏�������
	for (int input_index = 0; input_index < INPUT_KIND_NUM; input_index++)
	{
		m_input[input_index]->Init();
	}
}

//�X�e�b�v
void CInputManager::Step()
{
	//���ꂼ��̃X�e�b�v��
	for (int input_index = 0; input_index < INPUT_KIND_NUM; input_index++)
	{
		m_input[input_index]->Step();
	}
}

//�㏈��
void CInputManager::Fin()
{
	//�폜
	for (int input_index = 0; input_index < INPUT_KIND_NUM; input_index++)
	{
		_DELETE(m_input[input_index]);
	}
}

//�}�l�[�W���[�擾
CInputManager* CInputManager::GetManager()
{
	if (!m_inputManager) { return nullptr; }
	return m_inputManager;
}

//�}�l�[�W���[�폜
void CInputManager::DeleteManager()
{	
	//����Ȃ����
	if (m_inputManager)
	{
		//���g������
		Fin();
		
		_DELETE(m_inputManager);

		m_inputManager = nullptr;
	}
}

//�������ꂽ
bool CInputManager::IsPush(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);

	return m_inputManager->m_input[kind]->IsPush(code);
}
//�����Ă��邩
bool CInputManager::IsKeep(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);

	return m_inputManager->m_input[kind]->IsKeep(code);
}
//�����ꂽ
bool CInputManager::IsRelease(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);

	return m_inputManager->m_input[kind]->IsRelease(code);
}
//�P���ɉ�����Ă��邩
bool CInputManager::IsDown(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);

	return m_inputManager->m_input[kind]->IsDown(code);
}