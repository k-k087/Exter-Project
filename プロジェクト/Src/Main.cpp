#include "DxLib.h"

#include <conio.h>

#include "System/System.h"
#include "Debug/Debug.h"
#include "Memory/MemoryManager.h"

#include "Sound/SoundManager.h"

#include "Common.h"

#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//メモリリークチェック用デバッグフラグ作成
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifdef DEBUG_MODE

	//メモリ管理クラス初期化
	CMemoryManager::CreateManager()->Init();

#endif

	

	//システム初期化
	if (!CSystem::Init()) { return -1; }

	//共通の初期化
	CSystem::InitSystem();



	/*
		ゲームメインシステム
	*/
	CSystem::SystemMain();



	//ゲーム後処理
	CSystem::Fin();

#ifdef DEBUG_MODE

	//メモリ管理クラスDelete
	CMemoryManager::GetManager()->DeleteManager();
#endif

	//DXライブラリの後処理
	DxLib_End();

	return 0;
}