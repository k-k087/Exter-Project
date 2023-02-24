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
	//		魔法種類							サイズ							拡大率				半径		スピード	最大距離
	{	MAGIC_KIND_GOLEM_MAGIC,		{   10.f,  10.f,  10.f   },		{   1.f,  1.f,  1.f   },	9.f,	1.5f,	600.f,	},
};

static const char* EFFECT_MAGIC_PATH[MAGIC_KIND_NUM] = 
{
	"Data/Effect/MagicBullet.efk",
};


//インスタンス
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
	//エフェクトロード
	for (int effect_index = 0; effect_index < MAGIC_KIND_NUM; ++effect_index)
	{
		m_magicEffectID[effect_index] = CEffekseerCtrl::LoadData(EFFECT_MAGIC_PATH[effect_index]);
	}
}
void CMagicManager::Step()
{
	//まずプレイヤーの魔法
	for (auto& player_magic : m_playerMagic)
	{
		player_magic->Step();
	}

	//次に敵の魔法
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
	//ロード関連
	for (auto& player_magic : m_playerMagic)
	{
		player_magic->Fin();
	}

	for (auto& enemy_magic : m_enemyMagic)
	{
		enemy_magic->Fin();
	}
}


//マネージャー削除
void CMagicManager::DeleteManager()
{
	if (!m_magicManager) { return; }

	//まずは後処理
	Fin();

	//魔法破棄
	DeleteAllMagic();

	//最後に配列クリア
	m_playerMagic.clear();
	m_enemyMagic.clear();

	//マネージャーを消す
	CGameManager::DeleteMGList(m_magicManager);
	_DELETE(m_magicManager);

	m_magicManager = nullptr;
}


//魔法生成
void CMagicManager::CreatePlayerMagic()
{
	//生成し、リストに追加
	for (int bullet_index = 0; bullet_index < PLAYER_MAGIC_NUM; ++bullet_index)
	{
		m_playerMagic.push_back(CMagic::Create());
	}
}
void CMagicManager::CreateEnemyMagic()
{
	//生成し、リストに追加
	for (int bullet_index = 0; bullet_index < ENEMY_MAGIC_NUM; ++bullet_index)
	{
		m_enemyMagic.push_back(CMagic::Create());
	}
}

//魔法発射処理
void CMagicManager::RequestPlayerMagic(MagicRequestInfo& player, MagicKind kind)
{
	//空いてるところを探す
	for (auto & player_magic : *m_magicManager->GetPlayerMagic())
	{
		if (!player_magic->GetIsUse())
		{
			//種類をみて、詳細情報をとってくる
			MagicDetailInfo detail_info = MAGIC_DETAIL_INFO[kind];

			player_magic->Request(player, detail_info, m_magicManager->GetMagicEffectID(kind));
			break;
		}
	}
}
void CMagicManager::RequestEnemyMagic(MagicRequestInfo& enemy, MagicKind kind)
{
	//空いてるところを探す
	for (auto & enemy_magic : *m_magicManager->GetEnemyMagic())
	{
		if (!enemy_magic->GetIsUse())
		{
			//種類をみて、詳細情報をとってくる
			MagicDetailInfo detail_info = MAGIC_DETAIL_INFO[kind];

			enemy_magic->Request(enemy, detail_info, m_magicManager->GetMagicEffectID(kind));
			break;
		}
	}
}

//弾すべて削除
void CMagicManager::DeleteAllMagic()
{
	//中身も消す
	if (m_playerMagic.size() != 0)
	{
		std::vector<CMagic*>::iterator player_magic = m_playerMagic.begin();

		for (; player_magic != m_playerMagic.end(); ++player_magic)
		{
			//念のため後処理
			(*player_magic)->Fin();

			_DELETE(*player_magic);
		}
	}

	//なかみも消す
	if (m_enemyMagic.size() != 0)
	{
		std::vector<CMagic*>::iterator enemy_magic = m_enemyMagic.begin();

		for (; enemy_magic != m_enemyMagic.end(); ++enemy_magic)
		{
			//念のため後処理
			(*enemy_magic)->Fin();

			_DELETE(*enemy_magic);
		}
	}
}
