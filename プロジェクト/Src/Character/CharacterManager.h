#pragma once

#include "DxLib.h"

#include "Character.h"

#include "../GameManager/ManagerBase.h"
#include "CharacterCommon.h"

class CPlayer;

//�L�����N�^�[�}�l�[�W���[�N���X
class CCharacterManager : public CManagerBase
{
public:
	//�L�����N�^�[�}�l�[�W���[���
	enum CharaMGKind
	{
		CHARA_MG_KIND_NONE = -1,

		CHARA_MG_KIND_PLAYER,	//�v���C���[
		CHARA_MG_KIND_ENEMY,	//�G

		CHARA_MG_KIND_NUM,
	};
private:
	static CCharacterManager* m_characterManager;	//�}�l�[�W���[����
	CCharacterManager* m_Manager[CHARA_MG_KIND_NUM];	//�e�L�����}�l�[�W���[

protected:
	CharaMGKind m_charaKind;	//�L�������

	CCharacterManager() :
		m_Manager{ NULL }
	{
		m_kind = MG_KIND_CHARACTER;
	}

public:
	void Step() override;	//�X�e�b�v
	void Init();			//������
	void Load();			//���[�h
	void Fin();				//�㏈��

	//�}�l�[�W���[����
	static CCharacterManager* CreateManager();
	//�}�l�[�W���[�擾
	static CCharacterManager* GetManager();
	//�}�l�[�W���[�폜
	void DeleteManager();

	//�ǉ�
	static void AddManager(CCharacterManager* manager);
	//�폜
	void DeleteAllManager();

	//�G����
	static void CreateEnemy();
	//�G�̐擪���擾


	/*
		Getter&Setter
	*/

	//���
	CharaMGKind GetCharaKind()
	{
		return m_charaKind;
	}
	void SetCharaKind(CharaMGKind kind)
	{
		m_charaKind = kind;
	}

	//����̃}�l�[�W���[
	CCharacterManager* GetCharaManager(CharaMGKind kind)
	{
		return m_Manager[kind];
	}
	void SetCharaManager(CCharacterManager* manager)
	{
		CharaMGKind kind = manager->GetCharaKind();
		m_Manager[kind] = manager;
	}
};