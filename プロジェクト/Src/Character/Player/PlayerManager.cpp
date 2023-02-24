#include "DxLib.h"

#include <cassert>

#include "PlayerManager.h"
#include "Player.h"

#include "../CharacterManager.h"

#include "../../Debug/Debug.h"
#include "../../Common.h"


CPlayerManager* CPlayerManager::m_playerManager = nullptr;

CPlayerManager::~CPlayerManager()
{
	/*DeleteManager();*/
}

void CPlayerManager::Init()
{

}

void CPlayerManager::Load()
{
	m_player->Load();
}

void CPlayerManager::Step()
{
	//プレイヤーのステップへ
	m_player->Step();
}

void CPlayerManager::StepStartPhase()
{
	m_player->StepStartPhase();
}

//マネージャー生成
CPlayerManager* CPlayerManager::CreateManager()
{
#ifdef DEBUG_MODE
	assert(!m_playerManager);
#endif
	m_playerManager = _NEW(CPlayerManager);

	//リストに追加
	AddManager(m_playerManager);

	return m_playerManager;
}

//マネージャー取得
CPlayerManager* CPlayerManager::GetManager()
{
#ifdef DEBUG_MODE
	assert(m_playerManager);
#endif
	return m_playerManager;
}

//マネージャー削除
void CPlayerManager::DeleteManager()
{
	if (!m_playerManager) { return; }

	//まずは後処理
	Fin();
	
	if (m_playerManager)
	{
		//先に中身のプレイヤー削除
		DeletePlayer();

		//マネージャー削除
		_DELETE(m_playerManager);

		m_playerManager = nullptr;
	}
}

void CPlayerManager::Fin()
{
	//ロード関連削除
	if (m_player->GetHandle() != -1)
	{
		MV1DeleteModel(m_player->GetHandle());

		m_player->SetHandle(-1);
	}
	if (m_player->GetSwordHandle() != -1)
	{
		MV1DeleteModel(m_player->GetSwordHandle());
		m_player->SetSwordHandle(-1);
	}
}

//プレイヤー生成
void CPlayerManager::CreatePlayer()
{
	CPlayer* new_player = CPlayer::Create();

	//情報を反映
	m_playerManager->SetPlayer(new_player);
}

//プレイヤー削除
void CPlayerManager::DeletePlayer()
{
	if (m_playerManager->GetPlayer())
	{
		_DELETE(m_playerManager->GetPlayer());
		m_playerManager->SetPlayer(nullptr);
	}
}