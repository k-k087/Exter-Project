#include "DxLib.h"

#include <cassert>

#include "Sky.h"
#include "SkyManager.h"

#include "../Debug/Debug.h"
#include "../Input/InputManager.h"
#include "../Character/Player/Player.h"
#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"
#include "../Common.h"


//マネージャー宣言
CSkyManager* CSkyManager::m_skyManager = NULL;

//カメラマネージャークラス生成
CSkyManager* CSkyManager::CreateManager()
{
	//存在しなければつくる
	if (!m_skyManager)
	{
		m_skyManager = _NEW(CSkyManager);

		//マネージャーリストに追加
		CGameManager::AddMGList(m_skyManager);
	}

	return m_skyManager;
}



//初期化
void CSkyManager::Init()
{
	//存在しなければ作る
	if (!m_sky)
	{
		m_sky = CSky::Create();
	}

	//クラスの初期化へ
	m_sky->Init();
}

//ロード
void CSkyManager::Load()
{
	m_sky->Load();
}

//ステップ
void CSkyManager::Step()
{
#ifdef DEBUG_MODE

#endif

	m_sky->Step();
	m_sky->Update();
}

//描画
void CSkyManager::Draw()
{
	m_sky->Draw();
}

CSkyManager* CSkyManager::GetManager()
{
	if (!m_skyManager) { return nullptr; }

	return m_skyManager;
}

//マネージャー削除
void CSkyManager::DeleteManager()
{
	if (!m_skyManager) { return; }

	//中が残っているなら消す
	if (m_sky)
	{
		//先に画像削除
		if (m_sky->GetHandle() != -1)
		{
			MV1DeleteModel(m_sky->GetHandle());

			m_sky->SetHandle(-1);
		}

		_DELETE(m_sky);
	}

	//最後にマネージャーを消す
	CGameManager::DeleteMGList(m_skyManager);
	_DELETE(m_skyManager);

	m_skyManager = nullptr;
}

