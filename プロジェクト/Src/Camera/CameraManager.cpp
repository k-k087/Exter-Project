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


//�J�����}�l�[�W���[�錾
CCameraManager* CCameraManager::m_cameraManager = nullptr;

//�J�����}�l�[�W���[�N���X����
CCameraManager* CCameraManager::CreateManager()
{
	//���݂��Ȃ���΂���
	if (!m_cameraManager)
	{
		CCameraManager* camera_manager = _NEW(CCameraManager);

		m_cameraManager = camera_manager;

		//�}�l�[�W���[���X�g�ɒǉ�
		CGameManager::AddMGList(m_cameraManager);
	}

	m_cameraManager->SetCameraID(CAMERA_ID_PLAY);

	return m_cameraManager;
}


//������
void CCameraManager::Init()
{
	//���݂��Ȃ���΍��
	if (!m_playCamera)
	{
		CPlayCamera* play_camera = CPlayCamera::Create();

		m_playCamera = play_camera;
	}

	//�J�����N���X�̏�������
	m_playCamera->Init();

	//���݂��Ȃ���΍��
	if (!m_debugCamera)
	{
		CDebugCamera* debug_camera = CDebugCamera::Create();

		m_debugCamera = debug_camera;
	}

	//�J�����N���X�̏�������
	m_debugCamera->Init();
}

void CCameraManager::Load()
{
	//����Ȃ烍�[�h��
	if (m_playCamera) { m_playCamera->Load(); }
	if (m_debugCamera) { m_debugCamera->Load(); }
}

//�X�e�b�v
void CCameraManager::Step()
{
#ifdef DEBUG_MODE
	//�f�o�b�N���[�h�؂�ւ�
	if (CInputManager::IsDown(KEY_INPUT_P, I_KEY))
	{
		ChangeCameraMode(CAMERA_ID_DEBUG);
		TakeOverDebugCamera();
	}

	//�ʏ탂�[�h�ؑ�
	if (CInputManager::IsDown(KEY_INPUT_O, I_KEY))
	{
		ChangeCameraMode(CAMERA_ID_PLAY);
	}
#endif
	
	//���݂̃J�������[�h��
	switch (m_currentID)
	{
		//�ʏ탂�[�h
	case CAMERA_ID_PLAY:
		m_playCamera->Step();
		m_playCamera->Update();
		break;

		//�f�o�b�O���[�h
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

//�J�������[�h�ؑ�
void CCameraManager::ChangeCameraMode(CameraID camera_id)
{
#ifdef DEBUG_MODE
	assert(CAMERA_ID_NONE < camera_id && camera_id < CAMERA_ID_NUM);
#endif

	//�������[�h�ɂ��悤�Ƃ��Ă�ۂ�return
	if (m_currentID == camera_id) { return; }

	m_currentID = camera_id;
}

//���݂̃J�������[�h
CCameraManager::CameraID CCameraManager::GetCurrentCameraMode()
{
	return m_currentID;
}


CCameraManager* CCameraManager::GetManager()
{
	if (!m_cameraManager) { return nullptr; }

	return m_cameraManager;
}

//�ʏ�J�����̏����f�o�b�O�J�����Ɉ����p��
void  CCameraManager::TakeOverDebugCamera()
{
	CPlayerManager* player_manager = CPlayerManager::GetManager();
	CPlayer* player = player_manager->GetPlayer();

	VECTOR camera_pos = m_playCamera->GetPos();
	VECTOR rot = player->GetRot();

	m_debugCamera->SetPos(camera_pos);
	m_debugCamera->SetRot(rot);
}

//�J�����}�l�[�W���[�폜
void CCameraManager::DeleteManager()
{
	if (!m_cameraManager) { return; }

	//���̃J�������c���Ă���Ȃ����
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

	//�Ō�Ƀ}�l�[�W���[������
	CGameManager::DeleteMGList(m_cameraManager);
	_DELETE(m_cameraManager);

	m_cameraManager = nullptr;
}

//�J�����߂Â�����
void CCameraManager::ApproachPlayCamera(float approach, int time)
{
	if (!m_playCamera) { return; }

	m_playCamera->ApproachCamera(approach, time);
}