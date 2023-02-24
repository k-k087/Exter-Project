#pragma once

#include "Camera.h"

#include "PlayCamera/PlayCamera.h"
#include "DebugCamera/DebugCamera.h"

#include "../GameManager/ManagerBase.h"


//�J�����N���X
class CCameraManager : public CManagerBase
{
public:
	//�J�����^�C�v
	enum CameraID
	{
		CAMERA_ID_NONE = -1,

		CAMERA_ID_PLAY,		//�v���C���̃J����
		CAMERA_ID_DEBUG,	//�f�o�b�O�p�J����

		CAMERA_ID_NUM,
	};

private:
	CPlayCamera* m_playCamera;		//�ʏ�J����
	CDebugCamera* m_debugCamera;	//�f�o�b�O�J����
	CameraID m_currentID;			//���݂̃J�������[�h

	static CCameraManager* m_cameraManager;//�J�����}�l�[�W���[����

	//�R���X�g���N�^
	CCameraManager() :
		m_playCamera(nullptr),
		m_debugCamera(nullptr),
		m_currentID(CAMERA_ID_NONE)
	{
		m_kind = MG_KIND_CAMERA;
	}

public:
	
	void Init();			//������
	void Load();			//���[�h
	void Step() override;	//�X�e�b�v
	void Fin();				//�I������

	//�J�������[�h�ؑ�
	void ChangeCameraMode(CameraID camera_id);	
	//�ʏ�J�����̈��p��
	void TakeOverDebugCamera();
	//���݂̃J�������[�h
	CameraID GetCurrentCameraMode();

	//�J�����߂Â�����
	void ApproachPlayCamera(float approach, int time);
	
	//�J�����}�l�[�W���[����
	static CCameraManager* CreateManager();
	//�J�����}�l�[�W���[�Q�b�g
	static CCameraManager* GetManager();
	//�J�����}�l�[�W���[�폜
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