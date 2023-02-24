#pragma once

#include "../Common.h"

#include <vector>

#include "Beam.h"

#include "../GameManager/ManagerBase.h"
#include "../GameManager/GameManager.h"

using namespace std;

class CPlayer; class CEnemy;

static const int BEAM_MAX_NUM = 10;

//Beam管理クラス
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
	static CBeamManager* m_beamManager;	//インスタンス

	int				m_beamEffectID[BEAM_KIND_NUM];
	BeamRequestData m_requestData[BEAM_MAX_NUM];

	vector<CBeam*> m_playerBeam;	//プレイヤー
	vector<CBeam*> m_enemyBeam;		//敵ビーム

	CBeamManager() :
		m_beamEffectID{},
		m_requestData{}
	{
		m_kind = MG_KIND_BEAM;
		m_playerBeam.clear();
		m_enemyBeam.clear();
	}

public:
	void Init();			//初期化
	void Load();			//ロード
	void Step();			//ステップ
	void Update();			//アップデート
	void Fin();				//後処理


	//マネージャー生成
	static CBeamManager* CreateManager()
	{
		//ないならつくる
		if (!m_beamManager)
		{
			m_beamManager = _NEW(CBeamManager);

			//リストに追加
			CGameManager::AddMGList(m_beamManager);
		}

		return m_beamManager;
	}
	//マネージャー取得
	static CBeamManager* GetManager()
	{
		if (m_beamManager)
		{
			return m_beamManager;
		}

		return NULL;
	}
	//マネージャー削除
	void DeleteManager();


	//ビーム生成
	void CreatePlayerBeam();
	void CreateEnemyBeam();



	//すべて削除
	void DeleteAllBeam();

	//魔法発射処理
	static void RequestPlayerBeam(BeamRequestInfo& player, BeamKind kind);
	static void RequestEnemyBeam(BeamRequestInfo& enemy, BeamKind kind, VECTOR effect_pos);


	/*
		Getter&Setter
	*/

	//プレイヤー魔法
	vector<CBeam*>* GetPlayerBeam() { return &m_playerBeam; }

	//敵魔法
	vector<CBeam*>* GetEnemyBeam() { return &m_enemyBeam; }

	//エフェクトID
	int GetBeamEffectID(BeamKind kind)const { return m_beamEffectID[kind]; }

	//リクエストデータ
	BeamRequestData* GetRequestData() { return m_requestData; }
};
