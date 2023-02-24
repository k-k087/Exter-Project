#pragma once

#include "DxLib.h"

#include "ManagerBase.h"

class CGameManager
{
private:
	//�}�l�W���[�m�[�h
	//(�}�l�[�W���[�̎�ތ��܂��Ă邩��ʂɃ��X�g�Ǘ����Ȃ��Ă��������Ǖ׋��̂��߂ɂ��܂���)
	struct ManagerNode
	{
		CManagerBase* m_manager;	//�|�C���^
		ManagerNode* m_next;		//���̃A�h���X
		ManagerNode* m_prev;		//�O�̃A�h���X

		//�R���X�g���N�^
		ManagerNode() :
			m_manager(nullptr),
			m_next(nullptr),
			m_prev(nullptr)
		{}
	};

	ManagerNode* m_leadManagerNode;	//�擪�̃}�l�[�W���[
	static CGameManager* m_gameManager;	//�C���X�^���X

	CGameManager() :
		m_leadManagerNode(nullptr)
	{}

public:
	//������
	void Init();

	//���ׂẴ}�l�[�W���[�̃X�e�b�v
	static void StepAllManager();
	static void StepAllManagerN(CManagerBase::ManagerKind not_step_mg);
	static void StepAllManagerN(CManagerBase::ManagerKind not_step_mg1, CManagerBase::ManagerKind not_step_mg2);
	void Step();

	//���ׂĂ�Delete���񂷊֐�


	//���X�g�ǉ�
	static void AddMGList(CManagerBase* manager);
	void Add(CManagerBase* manager);

	//���X�g�폜�n
	static void DeleteMGList(CManagerBase* manager);
	void Delete(CManagerBase* manager);
	void DeleteAllList();

	//�����A�폜
	static void CreateManager();
	void DeleteManager();

	//Getter&Setter

	static CGameManager* GetGameManager()
	{
		if (!m_gameManager) { return nullptr; }
		return m_gameManager;
	}

	ManagerNode* GetLeadManagerNode()
	{
		if (m_leadManagerNode)
		{
			return m_leadManagerNode;
		}

		return nullptr;
	}
	void SetLeadManagerNode()
	{

	}


};