#pragma once
#include "DxLib.h"

//�J�����N���X
class CDebugCamera
{
private:
	VECTOR m_pos;
	VECTOR m_rot;
	float m_near, m_far;

	//�R���X�g���N�^
	CDebugCamera() :
		m_pos{ 0.f, 0.f, 0.f },
		m_rot{ 0.f, 0.f, 0.f }
	{}

public:

	//������
	void Init();
	//���[�h
	void Load();
	//�X�e�b�v
	void Step();
	//���W�Z�b�g
	void Update();

	//����
	static CDebugCamera* Create();

	/*
		Getter&Setter
	*/

	//���W
	VECTOR GetPos()const { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//��]�l
	VECTOR GetRot()const { return m_rot; }
	void SetRot(VECTOR rot) { m_rot = rot; }
};

////������
//void InitCameraInfo();
////����
//void CreateCamera();
////�㏈��
//void FinCamera();
////�����擾
//CCamera* GetCameraInfo();
