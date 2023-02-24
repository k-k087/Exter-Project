#include "DxLib.h"

#include <cassert>

#include "CameraManager.h"
#include "Camera.h"

#include "PlayCamera/PlayCamera.h"
#include "DebugCamera/DebugCamera.h"

#include "../Debug/Debug.h"
#include "../Input/InputManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Player/PlayerManager.h"
#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"
#include "../Common.h"


//カメラマネージャー宣言
CCameraManager* CCameraManager::m_cameraManager = nullptr;

//カメラマネージャークラス生成
CCameraManager* CCameraManager::CreateManager()
{
	//存在しなければつくる
	if (!m_cameraManager)
	{
		CCameraManager* camera_manager = _NEW(CCameraManager);

		m_cameraManager = camera_manager;

		//マネージャーリストに追加
		CGameManager::AddMGList(m_cameraManager);
	}

	m_cameraManager->SetCameraID(CAMERA_ID_PLAY);

	return m_cameraManager;
}


//初期化
void CCameraManager::Init()
{
	//存在しなければ作る
	if (!m_playCamera)
	{
		CPlayCamera* play_camera = CPlayCamera::Create();

		m_playCamera = play_camera;
	}

	//カメラクラスの初期化へ
	m_playCamera->Init();

	//存在しなければ作る
	if (!m_debugCamera)
	{
		CDebugCamera* debug_camera = CDebugCamera::Create();

		m_debugCamera = debug_camera;
	}

	//カメラクラスの初期化へ
	m_debugCamera->Init();
}

void CCameraManager::Load()
{
	//あるならロードへ
	if (m_playCamera) { m_playCamera->Load(); }
	if (m_debugCamera) { m_debugCamera->Load(); }
}

//ステップ
void CCameraManager::Step()
{
#ifdef DEBUG_MODE
	//デバックモード切り替え
	if (CInputManager::IsDown(KEY_INPUT_P, I_KEY))
	{
		ChangeCameraMode(CAMERA_ID_DEBUG);
		TakeOverDebugCamera();
	}

	//通常モード切替
	if (CInputManager::IsDown(KEY_INPUT_O, I_KEY))
	{
		ChangeCameraMode(CAMERA_ID_PLAY);
	}
#endif
	
	//現在のカメラモードへ
	switch (m_currentID)
	{
		//通常モード
	case CAMERA_ID_PLAY:
		m_playCamera->Step();
		m_playCamera->Update();
		break;

		//デバッグモード
	case CAMERA_ID_DEBUG:
		m_debugCamera->Step();
		m_debugCamera->Update();
		break;
	}
}

float CCameraManager::GetPlayCameraRotY()
{
	if (m_playCamera)
	{
		return m_playCamera->GetRot().y;
	}

	return 0.f;
}

//カメラモード切替
void CCameraManager::ChangeCameraMode(CameraID camera_id)
{
#ifdef DEBUG_MODE
	assert(CAMERA_ID_NONE < camera_id && camera_id < CAMERA_ID_NUM);
#endif

	//同じモードにしようとしてる際はreturn
	if (m_currentID == camera_id) { return; }

	m_currentID = camera_id;
}

//現在のカメラモード
CCameraManager::CameraID CCameraManager::GetCurrentCameraMode()
{
	return m_currentID;
}


CCameraManager* CCameraManager::GetManager()
{
	if (!m_cameraManager) { return nullptr; }

	return m_cameraManager;
}

//通常カメラの情報をデバッグカメラに引き継ぐ
void  CCameraManager::TakeOverDebugCamera()
{
	CPlayerManager* player_manager = CPlayerManager::GetManager();
	CPlayer* player = player_manager->GetPlayer();

	VECTOR camera_pos = m_playCamera->GetPos();
	VECTOR rot = player->GetRot();

	m_debugCamera->SetPos(camera_pos);
	m_debugCamera->SetRot(rot);
}

//カメラマネージャー削除
void CCameraManager::DeleteManager()
{
	if (!m_cameraManager) { return; }

	//中のカメラが残っているなら消す
	if (m_playCamera)
	{
		_DELETE(m_playCamera);
		m_playCamera = nullptr;
	}
	if (m_debugCamera)
	{
		_DELETE(m_debugCamera);
		m_debugCamera = nullptr;
	}

	//最後にマネージャーを消す
	CGameManager::DeleteMGList(m_cameraManager);
	_DELETE(m_cameraManager);

	m_cameraManager = nullptr;
}

//カメラ近づく処理
void CCameraManager::ApproachPlayCamera(float approach, int time)
{
	if (!m_playCamera) { return; }

	m_playCamera->ApproachCamera(approach, time);
}