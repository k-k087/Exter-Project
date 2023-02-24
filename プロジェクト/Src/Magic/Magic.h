#pragma once

#include "../Common.h"

#include "../Character/Character.h"

//���@�̎��
enum MagicKind
{
	MAGIC_KIND_NONE = -1,

	MAGIC_KIND_GOLEM_MAGIC,

	MAGIC_KIND_NUM,
};


struct MagicDetailInfo
{
	MagicKind	m_magicKind;	//���

	VECTOR		size;			//���f���T�C�Y
	VECTOR		scale;			//�g�嗦
	float		radius;			//���f�����a
	float		speed;			//�X�s�[�h
	float		maxLange;		//�ő勗��
};

struct MagicRequestInfo
{
	VECTOR startPos;
	VECTOR rot;
	VECTOR move;

	int			paramI;
	float		paramF;
};

class CMagic
{
private:

	MagicDetailInfo		m_detailInfo;				//�ڍ׏��i�G�t�F�N�g�ɂ���Ă������ς��j

	VECTOR				m_pos;					//���W
	VECTOR				m_rot;					//��]�l
	VECTOR				m_startPos;				//�������W
	VECTOR				m_oldPos;				//1�t���[���O�̍��W
	VECTOR				m_move;					//�ړ���

	int					m_effectHandleID;		//�摜�n���h��
	bool				m_isUse;				//�g�p���t���O

	CMagic() :
		m_detailInfo{},
		m_isUse(false),
		m_effectHandleID(-1)
	{}
	

public:

	~CMagic() {}

	void		Init();			
	void		Step();
	void		Update();
	void		Fin();

	static CMagic* Create()
	{
		CMagic* magic = _NEW(CMagic);	magic->Init();

		return magic;
	}

	void Request(MagicRequestInfo& request_info, MagicDetailInfo& detail_info, int handle_id);


	/*
		Getter&Setter
	*/

	MagicDetailInfo* GetDetailInfo() { return &m_detailInfo; }
	void SetDetailInfo(MagicDetailInfo& detail) { m_detailInfo = detail; }

	//�g�p�t���O
	bool GetIsUse()const { return m_isUse; }
	void SetIsUse(const bool is_use) { m_isUse = is_use; }

	//�n���h��
	int GetEffectHandleID()const { return m_effectHandleID; }
	void SetEffectHandleID(const int handle_id) { m_effectHandleID = handle_id; }

	// ���W
	VECTOR GetPos()const { return m_pos; }
	void SetPos(const VECTOR& pos) { m_pos = pos; }

	//1�t���[���O�̍��W
	VECTOR GetOldPos()const { return m_oldPos; }
	void SetOldPos(const VECTOR& old_pos) { m_oldPos = old_pos; }

	//�X�^�[�g���W
	VECTOR GetStartPos()const { return m_startPos; }
	void SetStartPos(const VECTOR& start_pos) { m_startPos = start_pos; }

	//�ړ���
	VECTOR GetMove()const { return m_move; }
	void SetMove(const VECTOR& move) { m_move = move; }

	//��]�l
	VECTOR GetRot()const { return m_rot; }
	void SetRot(const VECTOR& rot) { m_rot = rot; }

	//���a
	float GetRadius()const { return m_detailInfo.radius; }
	void SetRadius(const float radius) { m_detailInfo.radius = radius; }

};



