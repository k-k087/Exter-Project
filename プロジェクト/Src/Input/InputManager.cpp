#include "DxLib.h"

#include <cassert>

#include "Input.h"
#include "InputManager.h"
#include "KeyBoard/KeyBoard.h"
#include "Mouse/Mouse.h"
#include "Pad/Pad.h"

#include "../Debug/Debug.h"
#include "../GameManager/GameManager.h"

#include "../Common.h"


//インプットマネージャー宣言
CInputManager* CInputManager::m_inputManager = NULL;

//マネージャー生成
void CInputManager::CreateManager()
{
	//ないなら作る
	if (!m_inputManager)
	{
		m_inputManager = _NEW(CInputManager);
		
		//CGameManager::AddMGList(m_inputManager);

		//初期化へ
		m_inputManager->Init();
	}
}

//初期化
void CInputManager::Init()
{
	//入力デバイスごとにインスタンス生成
	m_input[I_KEY] = _NEW(CKeyBoard);
	m_input[I_MOUSE] = CMouse::CreateMouse();
	m_input[I_PAD] = CPad::CreatePad();

	//それぞれの初期化へ
	for (int input_index = 0; input_index < INPUT_KIND_NUM; input_index++)
	{
		m_input[input_index]->Init();
	}
}

//ステップ
void CInputManager::Step()
{
	//それぞれのステップへ
	for (int input_index = 0; input_index < INPUT_KIND_NUM; input_index++)
	{
		m_input[input_index]->Step();
	}
}

//後処理
void CInputManager::Fin()
{
	//削除
	for (int input_index = 0; input_index < INPUT_KIND_NUM; input_index++)
	{
		_DELETE(m_input[input_index]);
	}
}

//マネージャー取得
CInputManager* CInputManager::GetManager()
{
	if (!m_inputManager) { return nullptr; }
	return m_inputManager;
}

//マネージャー削除
void CInputManager::DeleteManager()
{	
	//あるなら消す
	if (m_inputManager)
	{
		//中身も消す
		Fin();
		
		_DELETE(m_inputManager);

		m_inputManager = nullptr;
	}
}

//今押された
bool CInputManager::IsPush(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);

	return m_inputManager->m_input[kind]->IsPush(code);
}
//押しているか
bool CInputManager::IsKeep(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);

	return m_inputManager->m_input[kind]->IsKeep(code);
}
//離された
bool CInputManager::IsRelease(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);

	return m_inputManager->m_input[kind]->IsRelease(code);
}
//単純に押されているか
bool CInputManager::IsDown(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);

	return m_inputManager->m_input[kind]->IsDown(code);
}