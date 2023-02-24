#pragma once

#include "../Common.h"

#include <vector>

#include "Beam.h"

#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"

using namespace std;

class CPlayer; class CEnemy;

static const int BEAM_MAX_NUM = 10;

//Beam�Ǘ��N���X
class CBeamManager : public CManagerBase
{
public:
	typedef void (CBeamManager::*BMFunc)();

public:

	struct BeamRequestData
	{
		BeamKind beamKind;

		int interval;
		int intervalCount;
		int timeCount;
		int maxTime;
		int requestIndex;
		int	effectHandleID;
		int startCount;

		bool m_isStart;

		BeamRequestInfo requestInfo;

		bool isUse;
	};

private:
	static CBeamManager* m_beamManager;	//�C���X�^���X

	int				m_beamEffectID[BEAM_KIND_NUM];
	BeamRequestData m_requestData[BEAM_MAX_NUM];

	vector<CBeam*> m_playerBeam;	//�v���C���[
	vector<CBeam*> m_enemyBeam;		//�G�r�[��

	CBeamManager() :
		m_beamEffectID{},
		m_requestData{}
	{
		m_kind = MG_KIND_BEAM;
		m_playerBeam.clear();
		m_enemyBeam.clear();
	}

public:
	void Init();			//������
	void Load();			//���[�h
	void Step();			//�X�e�b�v
	void Update();			//�A�b�v�f�[�g
	void Fin();				//�㏈��


	//�}�l�[�W���[����
	static CBeamManager* CreateManager()
	{
		//�Ȃ��Ȃ����
		if (!m_beamManager)
		{
			m_beamManager = _NEW(CBeamManager);

			//���X�g�ɒǉ�
			CGameManager::AddMGList(m_beamManager);
		}

		return m_beamManager;
	}
	//�}�l�[�W���[�擾
	static CBeamManager* GetManager()
	{
		if (m_beamManager)
		{
			return m_beamManager;
		}

		return NULL;
	}
	//�}�l�[�W���[�폜
	void DeleteManager();


	//�r�[������
	void CreatePlayerBeam();
	void CreateEnemyBeam();



	//���ׂč폜
	void DeleteAllBeam();

	//���@���ˏ���
	static void RequestPlayerBeam(BeamRequestInfo& player, BeamKind kind);
	static void RequestEnemyBeam(BeamRequestInfo& enemy, BeamKind kind, VECTOR effect_pos);


	/*
		Getter&Setter
	*/

	//�v���C���[���@
	vector<CBeam*>* GetPlayerBeam() { return &m_playerBeam; }

	//�G���@
	vector<CBeam*>* GetEnemyBeam() { return &m_enemyBeam; }

	//�G�t�F�N�gID
	int GetBeamEffectID(BeamKind kind)const { return m_beamEffectID[kind]; }

	//���N�G�X�g�f�[�^
	BeamRequestData* GetRequestData() { return m_requestData; }
};
