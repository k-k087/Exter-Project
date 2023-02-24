#include "DxLib.h"

#include "GameManager.h"
#include "ManagerBase.h"
#include "../Common.h"

#include "../Effekseer/Effekseer.h"

CGameManager* CGameManager::m_gameManager = nullptr;	//�C���X�^���X

void CGameManager::Init()
{
	
}

void CGameManager::StepAllManager()
{
	if (!m_gameManager->GetLeadManagerNode())return;

	ManagerNode* node = m_gameManager->GetLeadManagerNode();


	//���Ԃɂ��ׂẴX�e�b�v����
	for (; node; node = node->m_next)
	{
		node->m_manager->Step();
	}

}
void CGameManager::StepAllManagerN(CManagerBase::ManagerKind not_step_mg)
{
	if (!m_gameManager->GetLeadManagerNode())return;

	ManagerNode* node = m_gameManager->GetLeadManagerNode();


	//���Ԃɂ��ׂẴX�e�b�v����(�����̂͂܂킳�Ȃ�)
	for (; node; node = node->m_next)
	{
		if (node->m_manager->GetKind() == not_step_mg)
			continue;

		node->m_manager->Step();
	}
}
void CGameManager::StepAllManagerN(CManagerBase::ManagerKind not_step_mg1, CManagerBase::ManagerKind not_step_mg2)
{
	if (!m_gameManager->GetLeadManagerNode())return;

	ManagerNode* node = m_gameManager->GetLeadManagerNode();


	//���Ԃɂ��ׂẴX�e�b�v����(�����̂͂܂킳�Ȃ�)
	for (; node; node = node->m_next)
	{
		if (node->m_manager->GetKind() == not_step_mg1)
			continue;
		if (node->m_manager->GetKind() == not_step_mg2)
			continue;

		node->m_manager->Step();
	}
}
void CGameManager::Step()
{

}

void CGameManager::CreateManager()
{
	if (!m_gameManager)
	{
		m_gameManager = _NEW(CGameManager);
	}
}

void CGameManager::DeleteManager()
{
	if (!m_gameManager) { return; }
	
	_DELETE(m_gameManager);

	m_gameManager = nullptr;
	
}

//���X�g�ɒǉ����鏈��
void CGameManager::AddMGList(CManagerBase* manager)
{
	if (m_gameManager)
	{
		m_gameManager->Add(manager);
	}
}
void CGameManager::Add(CManagerBase* manager)
{
	CManagerBase::ManagerKind kind = manager->GetKind();

	//�V�����m�[�h�����

	ManagerNode* new_node = _NEW(ManagerNode);
	new_node->m_manager = manager;

	//�擪�����Ȃ���ΐ擪�ɂ���
	if (m_leadManagerNode == nullptr)
	{
		m_leadManagerNode = new_node;
	}
	//�擪������Ȃ�
	else
	{
		//�擪�擾
		ManagerNode* node = m_leadManagerNode;

		//�D��x���Ƀ��X�g�ɒǉ�
		//�����O�̃m�[�h�܂ŋ󃋁[�v
		for (; node->m_next && node->m_next->m_manager->GetKind() <= kind; node = node->m_next){}

		//���ԂȂ�
		if (node->m_next)
		{
			//next��prev���Ȃ��Ȃ����A�Ԃɓ����
			ManagerNode* next_node = node->m_next;

			node->m_next = new_node;
			new_node->m_prev = node;
			new_node->m_next = next_node;
			next_node->m_prev = new_node;
		}
		//�Ō�܂ŉ�����Ȃ�
		else if (!node->m_next)
		{
			//�Ō���ɒǉ�
			new_node->m_prev = node;
			node->m_next = new_node;
		}
	}

}

void CGameManager::DeleteMGList(CManagerBase* manager)
{
	if (m_gameManager)
	{
		m_gameManager->Delete(manager);
	}
}
void CGameManager::Delete(CManagerBase* manager)
{
	//�擪�擾
	ManagerNode* node = m_leadManagerNode;

	//�T��
	for (; node->m_manager->GetKind() != manager->GetKind(); node = node->m_next) {}

	//�Ō�̈������
	if (!node->m_prev && !node->m_next)
	{
		m_leadManagerNode = nullptr;
	}
	//�擪�Ȃ�
	else if (!node->m_prev)
	{
		m_leadManagerNode = node->m_next;
		m_leadManagerNode->m_prev = nullptr;
	}
	//���ԂȂ�
	else if (node->m_next&&node->m_prev)
	{
		ManagerNode* prev = node->m_prev;
		ManagerNode* next = node->m_next;

		prev->m_next = next;
		next->m_prev = prev;
		node->m_next = node->m_prev = nullptr;
	}
	//�Ō�Ȃ�
	else if (!node->m_next)
	{
		ManagerNode* prev = node->m_prev;
		prev->m_next = nullptr;
	}

	_DELETE(node);
	node = nullptr;
}
void CGameManager::DeleteAllList()
{

}