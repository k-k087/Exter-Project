#pragma once

#include "DxLib.h"

//�I�u�W�F�N�g�N���X (���W�ƕ`�悪�������)
class CObject
{
public:
	//�I�u�W�F�N�g���
	enum ObjectKind
	{
		OBJECT_KIND_NONE = -1,

		OBJECT_KIND_SKY,		//��
		OBJECT_KIND_FIELD,		//�t�B�[���h
		OBJECT_KIND_PLAYER,		//�v���C��-

		OBJECT_KIND_NUM,
	};

public:
	virtual void Init() = 0;	//������
	virtual void Load() = 0;	//���[�h
	virtual void Step() = 0;	//�X�e�b�v�֐�
	virtual void Update() = 0;	//�A�b�v�f�[�g
	virtual void Draw() = 0;	//�`��֐�
	virtual void Fin() = 0;		//�㏈��

protected:
	VECTOR		m_pos;			//���W
	VECTOR		m_rot;			//��]�l
	VECTOR		m_startPos;		//�������W
	VECTOR		m_oldPos;		//1�t���[���O�̍��W
	VECTOR		m_move;			//�ړ���
	VECTOR		m_size;			//���f���T�C�Y
	ObjectKind	m_kind;			//���
	float		m_radius;		//���f�����a
	int			m_handle;		//�n���h��
	int			m_drawID;		//�`��pID

	//�R���X�g���N�^
	CObject() :
		m_handle(-1),
		m_pos{ 0.f, 0.f, 0.f },
		m_rot{ 0.f, 0.f, 0.f },
		m_oldPos{ 0.f, 0.f, 0.f },
		m_startPos{ 0.f, 0.f, 0.f },
		m_move{ 0.f, 0.f, 0.f },
		m_size{ 0.f, 0.f, 0.f },
		m_radius(0.f),
		m_drawID(-1)
	{}

public:
	
	/*
		Getter&Setter
	*/

	//���W
	VECTOR GetPos()const
	{
		return m_pos;
	}
	void SetPos(const VECTOR& pos)
	{
		m_pos = pos;
	}

	//1�t���[���O�̍��W
	VECTOR GetOldPos()const 
	{
		return m_oldPos; 
	}
	void SetOldPos(const VECTOR& old_pos)
	{ 
		m_oldPos = old_pos; 
	}

	//�X�^�[�g���W
	VECTOR GetStartPos()const
	{
		return m_startPos;
	}
	void SetStartPos(const VECTOR& start_pos)
	{
		m_startPos = start_pos;
	}

	//�ړ���
	VECTOR GetMove()const 
	{ 
		return m_move;
	}
	void SetMove(const VECTOR& move)
	{ 
		m_move = move; 
	}

	//��]�l
	VECTOR GetRot()const
	{
		return m_rot;
	}
	void SetRot(const VECTOR& rot)
	{
		m_rot = rot;
	}

	//�T�C�Y
	VECTOR GetSize()const
	{
		return m_size;
	}
	void SetSize(const VECTOR& size)
	{
		m_size = size;
	}

	//�I�u�W�F�N�g�̎��
	ObjectKind GetKind()const
	{
		return m_kind;
	}
	void SetKind(const ObjectKind kind)
	{
		m_kind = kind;
	}

	//���a
	float GetRadius()const
	{
		return m_radius;
	}
	void SetRadius(const float radius)
	{
		m_radius = radius;
	}

	//�`��ID
	int GetDrawID()const
	{
		return m_drawID;
	}
	void SetDrawID(const int id)
	{
		m_drawID = id;
	}

	//�n���h��
	int GetHandle()const
	{
		return m_handle;
	}
	void SetHandle(const int id)
	{
		m_handle = id;
	}
};
