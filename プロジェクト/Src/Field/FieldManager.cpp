#include "DxLib.h"

#include <cassert>

#include "Field.h"
#include "FieldManager.h"

#include "../Debug/Debug.h"
#include "../Input/InputManager.h"
#include "../Character/Player/Player.h"
#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"
#include "../Common.h"


//マネージャー宣言
CFieldManager* CFieldManager::m_fieldManager = nullptr;

//カメラマネージャークラス生成
CFieldManager* CFieldManager::CreateManager()
{
	//存在しなければつくる
	if (!m_fieldManager)
	{
		m_fieldManager = _NEW(CFieldManager);

		//マネージャーリストに追加
		CGameManager::AddMGList(m_fieldManager);
	}

	return m_fieldManager;
}

//初期化
void CFieldManager::Init()
{
	
	m_field = CField::Create();
	m_field->Init();
}

//ロード
void CFieldManager::Load()
{
	m_field->Load();
	
}

//ステップ
void CFieldManager::Step()
{
#ifdef DEBUG_MODE

#endif
	m_field->Step();
	
}

//描画
void CFieldManager::Draw()
{

}

CFieldManager* CFieldManager::GetManager()
{
	if (!m_fieldManager) { return nullptr; }

	return m_fieldManager;
}

//マネージャー削除
void CFieldManager::DeleteManager()
{
	if (!m_fieldManager) { return; }

	//中が残っているなら消す
	
	int handle = m_field->GetHandle();
	if (handle != -1)
	{
		MV1DeleteModel(handle);
		m_field->SetHandle(-1);
	}

	if (m_field)
		_DELETE(m_field);

	m_field = nullptr;
	
	

	//最後にマネージャーを消す
	CGameManager::DeleteMGList(m_fieldManager);
	_DELETE(m_fieldManager);

	m_fieldManager = nullptr;
}