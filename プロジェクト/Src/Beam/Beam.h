#pragma once

#include "../Common.h"

#include "../Character/Character.h"

//�r�[���̎��
enum BeamKind
{
	BEAM_KIND_NONE = -1,

	BEAM_KIND_SONIC_BEAM,
	BEAM_KIND_BOSS1_THUNDER,

	BEAM_KIND_NUM,
};


struct BeamDetailInfo
{
	BeamKind			beamKind;		//���

	VECTOR				size;			//���f���T�C�Y
	VECTOR				scale;			//�g�嗦
	float				radius;			//�r�[�����a
	float				speed;			//�X�s�[�h
	float				distance;		//�ŏ��̊�
	float				maxLange;		//�ő勗��
	int					inteval;		//�Ԋu
	int					maxTime;		//�r�[���̎���
};

//�G�t�F�N�g���N�G�X�g���i�������W�A�G�t�F�N�g��]�l�A�ړ��ʁA�G�t�F�N�g�J�n���ԁA�G�t�F�N�g�����蔻��J�n���ԁA�G�t�F�N�g�Đ����x�AI�AF�j
struct BeamRequestInfo
{
	VECTOR				startPos;				//�������W
	VECTOR				rot;					//�G�t�F�N�g��]�l
	VECTOR				move;					//�ړ���

	int					startEffectTime;		//�G�t�F�N�g�J�n����
	int					startCollisionTime;		//�G�t�F�N�g�����蔻��J�n����

	float				effectSpeed;			//�G�t�F�N�g�Đ����x

	int					paramI;
	float				paramF;
};

class CBeam
{
private:

	BeamDetailInfo		m_detailInfo;			//�ڍ׏��i�G�t�F�N�g�ɂ���Ă������ς��j

	VECTOR				m_pos;					//���W
	VECTOR				m_rot;					//��]�l
	VECTOR				m_startPos;				//�������W
	VECTOR				m_oldPos;				//1�t���[���O�̍��W
	VECTOR				m_move;

	bool				m_isUse;				//�g�p���t���O

	int					m_requestIndex;			//���ˎw�W

	BeamKind			m_beamKind;

	CBeam() :
		m_detailInfo{},
		m_isUse(false),
		m_move{},
		m_pos{},
		m_rot{},
		m_startPos{},
		m_requestIndex(-1),
		m_oldPos{},
		m_beamKind(BEAM_KIND_NONE)
	{}
	

public:

	~CBeam() {}
	void		Init();
	void		Step();
	void		Update();
	void		Fin();

	static CBeam* Create()
	{
		CBeam* beam = _NEW(CBeam);	beam->Init();

		return beam;
	}

	void Request(BeamRequestInfo& request_info, BeamDetailInfo& detail_info, int request_index);


	/*
		Getter&Setter
	*/

	BeamDetailInfo* GetDetailInfo() { return &m_detailInfo; }
	void SetDetailInfo(BeamDetailInfo& detail) { m_detailInfo = detail; }

	//�g�p�t���O
	bool GetIsUse()const { return m_isUse; }
	void SetIsUse(const bool is_use) { m_isUse = is_use; }

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

	//�G�t�F�N�g���
	BeamKind GetBeamKind()const { return m_beamKind; }

	//���N�G�X�g�w�W
	int GetRequestIndex()const { return m_requestIndex; }
	void SetRequestIndex(const int request_index) { m_requestIndex = request_index; }

};



