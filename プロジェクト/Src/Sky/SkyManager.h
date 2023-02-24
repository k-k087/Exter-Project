#pragma once

#include "DxLib.h"

#include "../GameManager/ManagerBase.h"

class CSky;

class CSkyManager : public CManagerBase
{
private:
	CSky* m_sky;
	static CSkyManager* m_skyManager;

	CSkyManager()
	{
		m_sky = nullptr;
		m_kind = MG_KIND_SKY;
	}

public:
	void Init();			//������
	void Load();			//���[�h
	void Step() override;	//�X�e�b�v
	void Draw();			//�`��
	void Fin();				//�I������

	//�}�l�[�W���[����
	static CSkyManager* CreateManager();
	//�}�l�[�W���[�Q�b�g
	static CSkyManager* GetManager();
	//�}�l�[�W���[�폜
	void DeleteManager();
};
