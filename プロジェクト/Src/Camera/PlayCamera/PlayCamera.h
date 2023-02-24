#pragma once

#include "DxLib.h"

//�J�����N���X
class CPlayCamera
{
private:
	VECTOR	m_pos;
	VECTOR	m_oldPos;
	VECTOR	m_goalPos;
	VECTOR	m_oldGoalPos;
	VECTOR	m_approachVec;
	VECTOR	m_targetPos;
	VECTOR	m_upVec;
	VECTOR	m_rot;
	VECTOR	m_distance;
	VECTOR	m_approachDistance;
	float	m_near, m_far;
	float	m_cameraDistance[2];
	int		m_startCount;
	float	m_posYAdd;
	bool	m_isMoveRot;

	float	m_approachLen;
	int		m_approachTime;

	//�R���X�g���N�^
	CPlayCamera():
		m_approachLen(),
		m_approachVec{},
		m_approachTime(),
		m_distance{},
		m_posYAdd(),
		m_approachDistance{}
	{
		ZeroMemory(&m_pos, sizeof(VECTOR));
		ZeroMemory(&m_rot, sizeof(VECTOR));
		ZeroMemory(&m_targetPos, sizeof(VECTOR));
		m_startCount = 0;
		m_isMoveRot = true;
		m_cameraDistance[0] = m_cameraDistance[1] = 0;
		m_upVec = VGet(0.f, 1.f, 0.f);
	}

public:
	//������
	void Init();
	//���[�h
	void Load();
	//�X�e�b�v
	void Step();
	void StepFin();
	//���W�Z�b�g
	void Update();

	//����
	static CPlayCamera* Create();

	//�J�������߂Â�����
	void ApproachCamera(float approach, int time);

	/*
		Getter&Setter
	*/

	//���W
	VECTOR GetPos()const { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//�^�[�Q�b�g���W
	VECTOR GetTargetPos()const { return m_targetPos; }
	void SetTargetPos(VECTOR pos) { m_targetPos = pos; }

	//����
	VECTOR GetUpVec()const { return m_upVec; }
	void SetUpVec(VECTOR up_vec) { m_upVec = up_vec; }

	VECTOR GetRot()const { return m_rot; }

	bool GetIsMoveRot()const { return m_isMoveRot; }
	void SetIsMoveRot(const bool is_rot) { m_isMoveRot = is_rot; }

};

////������
//void InitCameraInfo();
////����
//void CreateCamera();
////�㏈��
//void FinCamera();
////�����擾
//CCamera* GetCameraInfo();
