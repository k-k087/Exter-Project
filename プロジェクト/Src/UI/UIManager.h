#pragma once

#include "DxLib.h"

#include "UI.h"

#include "../Common.h"
#include "../GameManager/ManagerBase.h"


class CUIManager
{
private:

	static CUIManager* m_UIManager;
	CUI* m_UI;

	CUIManager() :
		m_UI(nullptr)
	{}

public:

	~CUIManager()
	{}

	void Init();			//������
	void Load();			//���[�h
	void Step();			//�X�e�b�v
	void Draw();			//�`��
	void Fin();				//�㏈��

	//�}�l�[�W���[����
	static CUIManager* CreateManager();
	//�}�l�[�W���[�Q�b�g
	static CUIManager* GetManager();
	//�}�l�[�W���[�폜
	void DeleteManager();


	//�����UI����
	void CreateUI(UIKind kind);
	CUI* GetUI();

};