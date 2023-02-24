#include "DxLib.h"

#include <cassert>

#include "CharacterManager.h"
#include "Character.h"

#include "Player/Player.h"
#include "Player/PlayerManager.h"
#include "Enemy/EnemyManager.h"

#include "../Debug/Debug.h"
#include "../GameManager/GameManager.h"
#include "../Effekseer/Effekseer.h"

#include "../Common.h"


CCharacterManager* CCharacterManager::m_characterManager = nullptr;

//初期化
void CCharacterManager::Init()
{

}

//ロード
void CCharacterManager::Load()
{

}

//ステップ
void CCharacterManager::Step()
{
	for (int chara_index = 0; chara_index < CHARA_MG_KIND_NUM; chara_index++)
	{
		//マネージャーがあるならStepへ
		if (m_Manager[chara_index])
		{
			m_Manager[chara_index]->Step();
		}
	}
}

//後処理
void CCharacterManager::Fin()
{

}

//マネージャー生成
CCharacterManager* CCharacterManager::CreateManager()
{
	//ないなら作る
	if (!m_characterManager)
	{
		//生成
		m_characterManager = _NEW(CCharacterManager);

		//マネージャーリストに追加する
		CGameManager::AddMGList(m_characterManager);
	}

	return m_characterManager;
}

//マネージャー取得
CCharacterManager* CCharacterManager::GetManager()
{
	if (!m_characterManager) { return nullptr; }

	return m_characterManager;
}

//追加
void CCharacterManager::AddManager(CCharacterManager* manager)
{
	CharaMGKind kind = manager->GetCharaKind();

	//存在がないなら追加する
	if (!m_characterManager->GetCharaManager(kind))
	{
		m_characterManager->SetCharaManager(manager);
	}
}

//すべてのマネージャー削除
void CCharacterManager::DeleteAllManager()
{
	if (!m_characterManager) { return; }

	CPlayerManager::GetManager()->DeleteManager();
	CEnemyManager::GetManager()->DeleteManager();

	//最後にキャラマネージャー自身を削除
	CGameManager::DeleteMGList(m_characterManager);
	_DELETE(m_characterManager);

	m_characterManager = nullptr;
}


//敵生成
void CCharacterManager::CreateEnemy()
{

}

//敵の先頭情報取得
