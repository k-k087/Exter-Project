#include "DxLib.h"

#include <cassert>
#include <vector>

#include "Magic.h"
#include "MagicManager.h"

#include "../GameManager/GameManager.h"
#include "../Effekseer.h"

#include "../Debug/Debug.h"

#include "../Common.h"


static const int PLAYER_MAGIC_NUM = 10;
static const int ENEMY_MAGIC_NUM = 30;


MagicDetailInfo MAGIC_DETAIL_INFO[MAGIC_KIND_NUM] = 
{
	//		���@���							�T�C�Y							�g�嗦				���a		�X�s�[�h	�ő勗��
	{	MAGIC_KIND_GOLEM_MAGIC,		{   10.f,  10.f,  10.f   },		{   1.f,  1.f,  1.f   },	9.f,	1.5f,	600.f,	},
};

static const char* EFFECT_MAGIC_PATH[MAGIC_KIND_NUM] = 
{
	"Data/Effect/MagicBullet.efk",
};


//�C���X�^���X
CMagicManager* CMagicManager::m_magicManager = nullptr;

void CMagicManager::Init()
{
	CreatePlayerMagic();
	CreateEnemyMagic();

	for (auto& player_magic : m_playerMagic)
		player_magic->Init();
	
	for (auto& enemy_bullet : m_enemyMagic)
		enemy_bullet->Init();
	
}
void CMagicManager::Load()
{
	//�G�t�F�N�g���[�h
	for (int effect_index = 0; effect_index < MAGIC_KIND_NUM; ++effect_index)
	{
		m_magicEffectID[effect_index] = CEffekseerCtrl::LoadData(EFFECT_MAGIC_PATH[effect_index]);
	}
}
void CMagicManager::Step()
{
	//�܂��v���C���[�̖��@
	for (auto& player_magic : m_playerMagic)
	{
		player_magic->Step();
	}

	//���ɓG�̖��@
	for (auto& enemy_bullet : m_enemyMagic)
	{
		enemy_bullet->Step();
	}
}
void CMagicManager::Update()
{
	for (auto& player_magic : m_playerMagic)
		player_magic->Update();

	for (auto& enemy_bullet : m_enemyMagic)
		enemy_bullet->Update();
}
void CMagicManager::Fin()
{
	//���[�h�֘A
	for (auto& player_magic : m_playerMagic)
	{
		player_magic->Fin();
	}

	for (auto& enemy_magic : m_enemyMagic)
	{
		enemy_magic->Fin();
	}
}


//�}�l�[�W���[�폜
void CMagicManager::DeleteManager()
{
	if (!m_magicManager) { return; }

	//�܂��͌㏈��
	Fin();

	//���@�j��
	DeleteAllMagic();

	//�Ō�ɔz��N���A
	m_playerMagic.clear();
	m_enemyMagic.clear();

	//�}�l�[�W���[������
	CGameManager::DeleteMGList(m_magicManager);
	_DELETE(m_magicManager);

	m_magicManager = nullptr;
}


//���@����
void CMagicManager::CreatePlayerMagic()
{
	//�������A���X�g�ɒǉ�
	for (int bullet_index = 0; bullet_index < PLAYER_MAGIC_NUM; ++bullet_index)
	{
		m_playerMagic.push_back(CMagic::Create());
	}
}
void CMagicManager::CreateEnemyMagic()
{
	//�������A���X�g�ɒǉ�
	for (int bullet_index = 0; bullet_index < ENEMY_MAGIC_NUM; ++bullet_index)
	{
		m_enemyMagic.push_back(CMagic::Create());
	}
}

//���@���ˏ���
void CMagicManager::RequestPlayerMagic(MagicRequestInfo& player, MagicKind kind)
{
	//�󂢂Ă�Ƃ����T��
	for (auto & player_magic : *m_magicManager->GetPlayerMagic())
	{
		if (!player_magic->GetIsUse())
		{
			//��ނ��݂āA�ڍ׏����Ƃ��Ă���
			MagicDetailInfo detail_info = MAGIC_DETAIL_INFO[kind];

			player_magic->Request(player, detail_info, m_magicManager->GetMagicEffectID(kind));
			break;
		}
	}
}
void CMagicManager::RequestEnemyMagic(MagicRequestInfo& enemy, MagicKind kind)
{
	//�󂢂Ă�Ƃ����T��
	for (auto & enemy_magic : *m_magicManager->GetEnemyMagic())
	{
		if (!enemy_magic->GetIsUse())
		{
			//��ނ��݂āA�ڍ׏����Ƃ��Ă���
			MagicDetailInfo detail_info = MAGIC_DETAIL_INFO[kind];

			enemy_magic->Request(enemy, detail_info, m_magicManager->GetMagicEffectID(kind));
			break;
		}
	}
}

//�e���ׂč폜
void CMagicManager::DeleteAllMagic()
{
	//���g������
	if (m_playerMagic.size() != 0)
	{
		std::vector<CMagic*>::iterator player_magic = m_playerMagic.begin();

		for (; player_magic != m_playerMagic.end(); ++player_magic)
		{
			//�O�̂��ߌ㏈��
			(*player_magic)->Fin();

			_DELETE(*player_magic);
		}
	}

	//�Ȃ��݂�����
	if (m_enemyMagic.size() != 0)
	{
		std::vector<CMagic*>::iterator enemy_magic = m_enemyMagic.begin();

		for (; enemy_magic != m_enemyMagic.end(); ++enemy_magic)
		{
			//�O�̂��ߌ㏈��
			(*enemy_magic)->Fin();

			_DELETE(*enemy_magic);
		}
	}
}
