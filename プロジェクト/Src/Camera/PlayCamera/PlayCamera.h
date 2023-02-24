#pragma once

#include "DxLib.h"

//カメラクラス
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

	//コンストラクタ
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
	//初期化
	void Init();
	//ロード
	void Load();
	//ステップ
	void Step();
	void StepFin();
	//座標セット
	void Update();

	//生成
	static CPlayCamera* Create();

	//カメラが近づく処理
	void ApproachCamera(float approach, int time);

	/*
		Getter&Setter
	*/

	//座標
	VECTOR GetPos()const { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//ターゲット座標
	VECTOR GetTargetPos()const { return m_targetPos; }
	void SetTargetPos(VECTOR pos) { m_targetPos = pos; }

	//方向
	VECTOR GetUpVec()const { return m_upVec; }
	void SetUpVec(VECTOR up_vec) { m_upVec = up_vec; }

	VECTOR GetRot()const { return m_rot; }

	bool GetIsMoveRot()const { return m_isMoveRot; }
	void SetIsMoveRot(const bool is_rot) { m_isMoveRot = is_rot; }

};

////初期化
//void InitCameraInfo();
////生成
//void CreateCamera();
////後処理
//void FinCamera();
////箱情報取得
//CCamera* GetCameraInfo();
