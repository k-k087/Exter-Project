#pragma once

#include "DxLib.h"

#include "../GameManager/ManagerBase.h"
#include "Field.h"

class CField;

class CFieldManager : public CManagerBase
{
public:
private:
	CField* m_field;
	static CFieldManager* m_fieldManager;

	CFieldManager() :
		m_field{nullptr}
	{
		m_kind = MG_KIND_FIELD;
	}

public:
	void Init();			//������
	void Load();			//���[�h
	void Step();		//�X�e�b�v
	void Draw();			//�`��
	void Fin();				//�I������

	//�}�l�[�W���[����
	static CFieldManager* CreateManager();
	//�}�l�[�W���[�Q�b�g
	static CFieldManager* GetManager();
	//�}�l�[�W���[�폜
	void DeleteManager();

	CField* GetField()
	{
		return m_field;
	}
};