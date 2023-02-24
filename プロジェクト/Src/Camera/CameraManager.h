#pragma once

#include "Camera.h"

#include "PlayCamera/PlayCamera.h"
#include "DebugCamera/DebugCamera.h"

#include "../GameManager/ManagerBase.h"


//カメラクラス
class CCameraManager : public CManagerBase
{
public:
	//カメラタイプ
	enum CameraID
	{
		CAMERA_ID_NONE = -1,

		CAMERA_ID_PLAY,		//プレイ中のカメラ
		CAMERA_ID_DEBUG,	//デバッグ用カメラ

		CAMERA_ID_NUM,
	};

private:
	CPlayCamera* m_playCamera;		//通常カメラ
	CDebugCamera* m_debugCamera;	//デバッグカメラ
	CameraID m_currentID;			//現在のカメラモード

	static CCameraManager* m_cameraManager;//カメラマネージャー実態

	//コンストラクタ
	CCameraManager() :
		m_playCamera(nullptr),
		m_debugCamera(nullptr),
		m_currentID(CAMERA_ID_NONE)
	{
		m_kind = MG_KIND_CAMERA;
	}

public:
	
	void Init();			//初期化
	void Load();			//ロード
	void Step() override;	//ステップ
	void Fin();				//終了処理

	//カメラモード切替
	void ChangeCameraMode(CameraID camera_id);	
	//通常カメラの引継ぎ
	void TakeOverDebugCamera();
	//現在のカメラモード
	CameraID GetCurrentCameraMode();

	//カメラ近づく処理
	void ApproachPlayCamera(float approach, int time);
	
	//カメラマネージャー生成
	static CCameraManager* CreateManager();
	//カメラマネージャーゲット
	static CCameraManager* GetManager();
	//カメラマネージャー削除
	void DeleteManager();

	float GetPlayCameraRotY();

	CPlayCamera* GetPlayCamera()
	{
		return m_playCamera;
	}

	/*
		Getter&Setter
	*/

	CameraID GetCameraID() { return m_currentID; }
	void SetCameraID(CameraID camera_id) { m_currentID = camera_id; }
};