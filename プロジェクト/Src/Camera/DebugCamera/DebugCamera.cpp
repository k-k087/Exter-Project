#include "DxLib.h"

#include "../Camera.h"
#include "../CameraManager.h"
#include "DebugCamera.h"

#include <Math.h>
#include "../../Character/Player/Player.h"
#include "../../Input/InputManager.h"

#include "../../Common.h"


static const float CAMERA_MOVE = 4.f;
static const float CAMERA_ROT_SPD = DX_PI_F / 90.f;
static const float CAMERA_ROT_MAX = DX_PI_F / 2.f;

static const float CAMERA_NEAR = 1.f;
static const float CAMERA_FAR = 1000000.f;

//������
void CDebugCamera::Init()
{
	m_pos = VGet(0.f, 40.f, -40.f);
}

//����
CDebugCamera* CDebugCamera::Create()
{
	CDebugCamera* camera = _NEW(CDebugCamera);

	//�J�����Z�b�g
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	return camera;
}

//���[�h
void CDebugCamera::Load()
{

}

//�X�e�b�v
void CDebugCamera::Step()
{
	VECTOR move = { 0.f };

	bool is_shift = CInputManager::IsDown(KEY_INPUT_LSHIFT, I_KEY);

	//�O�i
	if (CInputManager::IsDown(KEY_INPUT_W, I_KEY))
	{
		if (is_shift)
		{
			move.y += CAMERA_MOVE;
		}
		else
		{
			move.x += CAMERA_MOVE * sinf(m_rot.y);
			move.z += CAMERA_MOVE * cosf(m_rot.y);
		}
	}
	//��i
	if (CInputManager::IsDown(KEY_INPUT_S, I_KEY))
	{
		if (is_shift)
		{
			move.y -= CAMERA_MOVE;
		}
		{
			move.x -= CAMERA_MOVE * sinf(m_rot.y);
			move.z -= CAMERA_MOVE * cosf(m_rot.y);
		}
	}
	//�E�ړ�
	if (CInputManager::IsDown(KEY_INPUT_D, I_KEY))
	{
		move.x -= CAMERA_MOVE * sinf(m_rot.y - DX_PI_F / 2.f);
		move.z -= CAMERA_MOVE * cosf(m_rot.y - DX_PI_F / 2.f);
	}
	//���ړ�
	if (CInputManager::IsDown(KEY_INPUT_A,I_KEY))
	{
		move.x -= CAMERA_MOVE * sinf(m_rot.y + DX_PI_F / 2.f);
		move.z -= CAMERA_MOVE * cosf(m_rot.y + DX_PI_F / 2.f);
	}

	//�J�������]
	if (CInputManager::IsDown(KEY_INPUT_UP, I_KEY))
	{
		m_rot.x -= CAMERA_ROT_SPD;
#if 0
		//����l
		if (m_rot.x < -CAMERA_ROT_MAX)
		{
			m_rot.x = -CAMERA_ROT_MAX;
		}
#endif
	}
	//�J��������]
	if (CInputManager::IsDown(KEY_INPUT_DOWN, I_KEY))
	{
		m_rot.x += CAMERA_ROT_SPD;
#if 0
		//����l
		if (m_rot.x > CAMERA_ROT_MAX)
		{
			m_rot.x = CAMERA_ROT_MAX;
		}
#endif
	}

	//�J�����E��]
	if (CInputManager::IsDown(KEY_INPUT_RIGHT, I_KEY))
	{
		m_rot.y += CAMERA_ROT_SPD;
	}
	//�J��������]
	if (CInputManager::IsDown(KEY_INPUT_LEFT, I_KEY))
	{
		m_rot.y -= CAMERA_ROT_SPD;
	}

	m_pos = VAdd(m_pos, move);
}

//���W�Z�b�g
void CDebugCamera::Update()
{
	VECTOR camera_pos = m_pos;
	SetCameraPositionAndAngle(camera_pos, m_rot.x, m_rot.y, m_rot.z);
}