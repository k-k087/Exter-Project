#pragma once

#include "DxLib.h"

#include "../CharacterManager.h"
#include "Player.h"

//�v���C���[�Ǘ��N���X
class CPlayerManager : public CCharacterManager
{
private:
	static CPlayerManager* m_playerManager;
	CPlayer* m_player;

	CPlayerManager() :
		m_player(NULL)
	{
		m_charaKind= CHARA_MG_KIND_PLAYER;
	}

public:
	~CPlayerManager();

	void Init();			//������
	void Load();			//���[�h
	void Step() override;	//�X�e�b�v
	void StepStartPhase();
	void Fin();				//�㏈��

	//�}�l�[�W���[����
	static CPlayerManager* CreateManager();
	//�}�l�[�W���[�擾
	static CPlayerManager* GetManager();
	//�}�l�[�W���[�폜
	void DeleteManager();

	//�v���C���[����
	static void CreatePlayer();
	//�v���C���[�폜
	static void DeletePlayer();

	/*
		Getter&Setter
	*/

	CPlayer* GetPlayer()
	{
		if (m_player)
		{
			return m_player;
		}

		return NULL;
	}
	void SetPlayer(CPlayer* player)
	{
		m_player = player;
	}

};
