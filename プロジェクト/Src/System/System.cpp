#include "DxLib.h"

#include "System.h"

#include <crtdbg.h>

#include "../Common.h"

#include "../GameManager/GameManager.h"
#include "../Input/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../FPS/FPS.h"
#include "../Camera/CameraManager.h"
#include "../Character/CharacterManager.h"
#include "../Collision/CollisionManager.h"
#include "../Draw/DrawManager.h"
#include "../Field/FieldManager.h"
#include "../Memory/MemoryManager.h"
#include "../Sky/SkyManager.h"
#include "../Effekseer.h"
#include "../Beam/BeamManager.h"
#include "../Magic/MagicManager.h"
#include "../UI/UIManager.h"
#include "../Fade/Fade.h"

bool CSystem::m_isEnd = false;

void CSystem::FinGame()
{
	m_isEnd = true;
}


//初期化
bool CSystem::Init()
{
	SetMainWindowText("Exter'");


	//ウィンドウの状態を設定する
	ChangeWindowMode(true);
#ifdef DEBUG_MODE
	
#endif

	//ウィンドウサイズを変更
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

	//DXライブラリの初期化
	if (DxLib_Init() == -1) { return false; }

	//描画するスクリーンを設定
	SetDrawScreen(DX_SCREEN_BACK);

	//３Dモデル設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetTransColor(255, 0, 255);

	// Zソート判定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetTransColor(255, 0, 255);	// 透過色指定

	return true;
}

//メインシステム
void CSystem::SystemMain()
{
	//ゲームメインループ
	while (ProcessMessage() != -1)
	{
		Sleep(1);	//システムに処理を返す

		//FPS処理
		CFrameRate::Step();

		// 現在の時間が、前回のフレーム時より
		// 1000/60ミリ秒（1/60秒）以上経過していたら処理を実行する
		if (CFrameRate::IsPassageFrameTime())
		{
			//フレームレート情報更新
			CFrameRate::Update();

			if (m_isEnd)
			{
				CSystem::AllFin();
				//エスケープキーが押されたら終了
				break;
			}

			//画面に表示されたものを初期化（１ループの１度だけ行う）
			ClearDrawScreen();

			//共通のシステム
			StepSystem();

			/*
				===============現在のシーンの現在の処理へ======================
			*/
			CSceneManager::GetManager()->CurrrentSceneFunc();
			/*
				===========================================================
			*/

			CFadeManager::GetManager()->Draw();

			//FPS計算
			CFrameRate::Calc();

#ifdef DEBUG_MODE
			//FPS表示
			CFrameRate::Draw();
#endif 

			

			//フリップ関数（ループの終わりに１度だけ行う）
			ScreenFlip();

		}//フレームレートのif文終わり

	}//メインループの終わり
}

//共通の初期化
void CSystem::InitSystem()
{
	/*
		動的クラス
	*/
	
	//ゲームマネージャー初期化
	CGameManager::CreateManager();
	//入力制御初期化
	CInputManager::CreateManager();
	//シーンマネージャー生成
	CSceneManager::CreateManager();
	//フェードマネージャー生成
	CFadeManager::CreateManager();


	/*
		静的クラス
	*/

	//FPS初期化
	CFrameRate::Init();
}

//共通のステップ
void CSystem::StepSystem()
{
	//入力制御ステップ
	CInputManager::GetManager()->Step();

	//フェードアップデート
	CFadeManager::GetManager()->Update();
}

//後処理
void CSystem::Fin()
{
	AllFin();

	//最後に１回だけやる処理をここに書く
	CSoundManager::Fin();

	CInputManager::GetManager()->DeleteManager();
	CSceneManager::GetManager()->DeleteManager();
	CGameManager::GetGameManager()->DeleteManager();
}

int CSystem::AllFin()
{
	CSoundManager::Fin();
	CInputManager::GetManager()->DeleteManager();
	CSceneManager::GetManager()->DeleteManager();
	CFadeManager::GetManager()->DeleteManager();;

#ifdef DEBUG_MODE
	//ここにCheckが入る
	CMemoryManager::GetManager()->CheckComplete();

#endif

	CCameraManager::GetManager()->DeleteManager();
	CCharacterManager::GetManager()->DeleteAllManager();
	CDrawManager::GetManager()->DeleteManager();
	CFieldManager::GetManager()->DeleteManager();
	CInputManager::GetManager()->DeleteManager();
	CSceneManager::GetManager()->DeleteManager();
	CSkyManager::GetManager()->DeleteManager();
	CCollisionManager::GetManager()->DeleteManager();
	CSoundManager::Fin();
	CEffekseerCtrl::Exit();
	CBeamManager::GetManager()->DeleteManager();
	CMagicManager::GetManager()->DeleteManager();
	CUIManager::GetManager()->DeleteManager();
	CFadeManager::GetManager()->DeleteManager();

	CGameManager::GetGameManager()->DeleteManager();


	return -1;
}