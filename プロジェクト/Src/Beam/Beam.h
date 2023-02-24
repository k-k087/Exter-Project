#pragma once

#include "../Common.h"

#include "../Character/Character.h"

//ビームの種類
enum BeamKind
{
	BEAM_KIND_NONE = -1,

	BEAM_KIND_SONIC_BEAM,
	BEAM_KIND_BOSS1_THUNDER,

	BEAM_KIND_NUM,
};


struct BeamDetailInfo
{
	BeamKind			beamKind;		//種類

	VECTOR				size;			//モデルサイズ
	VECTOR				scale;			//拡大率
	float				radius;			//ビーム半径
	float				speed;			//スピード
	float				distance;		//最初の間
	float				maxLange;		//最大距離
	int					inteval;		//間隔
	int					maxTime;		//ビームの時間
};

//エフェクトリクエスト情報（初期座標、エフェクト回転値、移動量、エフェクト開始時間、エフェクト当たり判定開始時間、エフェクト再生速度、I、F）
struct BeamRequestInfo
{
	VECTOR				startPos;				//初期座標
	VECTOR				rot;					//エフェクト回転値
	VECTOR				move;					//移動量

	int					startEffectTime;		//エフェクト開始時間
	int					startCollisionTime;		//エフェクト当たり判定開始時間

	float				effectSpeed;			//エフェクト再生速度

	int					paramI;
	float				paramF;
};

class CBeam
{
private:

	BeamDetailInfo		m_detailInfo;			//詳細情報（エフェクトによってここが変わる）

	VECTOR				m_pos;					//座標
	VECTOR				m_rot;					//回転値
	VECTOR				m_startPos;				//初期座標
	VECTOR				m_oldPos;				//1フレーム前の座標
	VECTOR				m_move;

	bool				m_isUse;				//使用中フラグ

	int					m_requestIndex;			//発射指標

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

	//使用フラグ
	bool GetIsUse()const { return m_isUse; }
	void SetIsUse(const bool is_use) { m_isUse = is_use; }

	// 座標
	VECTOR GetPos()const { return m_pos; }
	void SetPos(const VECTOR& pos) { m_pos = pos; }

	//1フレーム前の座標
	VECTOR GetOldPos()const { return m_oldPos; }
	void SetOldPos(const VECTOR& old_pos) { m_oldPos = old_pos; }

	//スタート座標
	VECTOR GetStartPos()const { return m_startPos; }
	void SetStartPos(const VECTOR& start_pos) { m_startPos = start_pos; }

	//移動量
	VECTOR GetMove()const { return m_move; }
	void SetMove(const VECTOR& move) { m_move = move; }

	//回転値
	VECTOR GetRot()const { return m_rot; }
	void SetRot(const VECTOR& rot) { m_rot = rot; }

	//半径
	float GetRadius()const { return m_detailInfo.radius; }
	void SetRadius(const float radius) { m_detailInfo.radius = radius; }

	//エフェクト種類
	BeamKind GetBeamKind()const { return m_beamKind; }

	//リクエスト指標
	int GetRequestIndex()const { return m_requestIndex; }
	void SetRequestIndex(const int request_index) { m_requestIndex = request_index; }

};



