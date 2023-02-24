#include "DxLib.h"

#include "../Camera.h"
#include "../CameraManager.h"
#include "PlayCamera.h"

#include <Math.h>

#include "../../Character/Player/PlayerManager.h"
#include "../../Character/Player/Player.h"
#include "../../Sound/SoundManager.h"
#include "../../MyMath/MyMath.h"
#include "../../Input/InputManager.h"

#include "../../Input/Pad/Pad.h"
#include "../../Input/Mouse/Mouse.h"

#include "../../Common.h"

static const float CAMERA_FRAME_ = 180.f;

static const float CAMERA_DISTANCE = 70.f;
static const float CAMERA_START_DISTANCE = 50.f;
static const float CAMERA_DISTANCE_BETWEEN = (CAMERA_DISTANCE - CAMERA_START_DISTANCE)/ CAMERA_FRAME_;
static const float CAMERA_DISTANCE_Y = 35.f;
static const float CAMERA_START_DISTANCE_Y = 0.f;
static const float CAMERA_DISTANCE_Y_BETWEEN = (CAMERA_DISTANCE_Y - CAMERA_START_DISTANCE_Y) / CAMERA_FRAME_;

static const float CAMERA_TARGET_POS_Y = 50.f;
static const float CAMERA_TARGET_START_POS_Y = 30.f;
static const float CAMERA_TARGET_POS_Y_BETWEEN = (CAMERA_TARGET_POS_Y - CAMERA_TARGET_START_POS_Y) / CAMERA_FRAME_;

static const float PLAYER_POS_Y_ADD = 50.f;
static const float PLAYER_POS_Y_ADD2 = 20.f;

static const float CAMERA_NEAR = 1.f;
static const float CAMERA_FAR = 100000.f;

static const int CAMERA_START_MAX_COUNT = 2000;
int a = 0;

static const VECTOR CAMERA_START_DISTANCE_POS = { 0.f,75.f,-97.f };

//初期化
void CPlayCamera::Init()
{
	m_pos	= VGet(0.f, 0.f, 0.f);
	m_rot = VGet(0.f, 0.f, 0.f);
	m_upVec = VGet(0.f, 1.f, 0.f);
	m_isMoveRot = true;

	//カメラセット
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);
}
//ロード
void CPlayCamera::Load()
{
	m_distance = CAMERA_START_DISTANCE_POS;

	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();
	VECTOR player_pos = player->GetPos();
	MATRIX all_mat[5] =
	{
		CMyMath::GetTranslateMatrix(m_distance),
		CMyMath::GetPitchMatrix(m_rot.x),
		CMyMath::GetYawMatrix(m_rot.y),
		CMyMath::GetRollMatrix(m_rot.z),
		CMyMath::GetTranslateMatrix(player_pos),
	};

	MATRIX camera_mat = all_mat[4];
	for (int mat_index = 3; mat_index >= 0; --mat_index)
		camera_mat = CMyMath::MatMult(camera_mat, all_mat[mat_index]);

	//座標反映
	m_pos.x = camera_mat.m[0][3];
	m_pos.y = camera_mat.m[1][3];
	m_pos.z = camera_mat.m[2][3];

	m_oldGoalPos = m_goalPos = m_pos;

	//ターゲットは常にプレイヤー
	player_pos.y += PLAYER_POS_Y_ADD;
	m_targetPos = player_pos;

	Update();
}

//生成
CPlayCamera* CPlayCamera::Create()
{
	return _NEW(CPlayCamera);
}

//ステップ
void CPlayCamera::Step()
{
	m_oldPos = m_pos;
	m_oldGoalPos = m_goalPos;

	//プレイヤーを見下ろす
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	VECTOR player_pos = player->GetPos();
	VECTOR player_rot = player->GetRot();

	if (m_isMoveRot)
	{
		if (CPad::GetPad()->IsRightStick())
		{

			//右スティック値で動く
			float left_stick = CPad::GetPad()->GetStickBuf(RIGHT_STICK_X);

			m_rot.y += 3.f * left_stick * (DX_PI_F / 180.f);
		}

		VECTOR distance = CMouse::GetMouse()->GetDistance();

		if (!CCommon::CheckZeroVector(distance))
		{
			m_rot.y += (distance.x * 0.001f);
			m_posYAdd += (distance.y * 0.01f);

			if (m_posYAdd < -PLAYER_POS_Y_ADD) { m_posYAdd = -PLAYER_POS_Y_ADD; }
			else if (m_posYAdd > PLAYER_POS_Y_ADD2) { m_posYAdd = PLAYER_POS_Y_ADD2; }
		}


	}

	if (m_approachTime > 0)
	{
		m_approachVec.y += 1.8f;
		m_approachVec.z -= 1.8f;

		if (m_approachVec.y > m_approachDistance.y) { m_approachVec.y = m_approachDistance.y; }
		if (m_approachVec.z < m_approachDistance.z) { m_approachVec.z = m_approachDistance.z; }


		m_approachTime -= FRAME_TIME;

		if (m_approachTime < 0)
		{
			m_approachTime = 0;
		}
	}
	else
	{
		//	差がゼロじゃない（戻ってないなら戻す）
		if (!CCommon::CheckZeroVector(m_approachVec))
		{
			m_approachVec.y -= 1.f;
			m_approachVec.z += 1.f;

			if (m_approachVec.y < 0.f) { m_approachVec.y = 0.f; }
			if (m_approachVec.z > 0.f) { m_approachVec.z = 0.f; }
		}
	}

	m_distance = CAMERA_START_DISTANCE_POS;
	m_distance.z += m_approachVec.z;
	m_distance.y += (m_posYAdd + m_approachVec.y);

#if 1
	MATRIX all_mat[5] =
	{
		CMyMath::GetTranslateMatrix(m_distance),
		CMyMath::GetPitchMatrix(m_rot.x),
		CMyMath::GetYawMatrix(m_rot.y),
		CMyMath::GetRollMatrix(m_rot.z),
		CMyMath::GetTranslateMatrix(player_pos),
	};

	MATRIX camera_mat = all_mat[4];
	for (int mat_index = 3; mat_index >= 0; --mat_index)
		camera_mat = CMyMath::MatMult(camera_mat, all_mat[mat_index]);

	m_pos.x = camera_mat.m[0][3];
	m_pos.y = camera_mat.m[1][3];
	m_pos.z = camera_mat.m[2][3];

	////	距離を出す
	//float goal_distance = CMyMath::VecLong(m_pos, m_goalPos);

	////	ゴールと今のPosの差があるなら
	//if (goal_distance >= 0.1f)
	//{
	//	if()
	//	VECTOR add = 
	//}
	////	差がないなら
	//else
	//{
	//}


#else
	MATRIX start_mat = CMyMath::GetTranslateMatrix(CAMERA_START_DISTANCE_POS);
	MATRIX rot_mat[3] =
	{
		CMyMath::GetPitchMatrix(m_rot.x),CMyMath::GetYawMatrix(m_rot.y),CMyMath::GetRollMatrix(m_rot.z),
	};
	//X,Y,Zの順で計算
	for (int shoot_index = 0; shoot_index < 3; ++shoot_index)
	{
		start_mat = CMyMath::MatMult(start_mat, rot_mat[shoot_index]);
	}
	m_pos = player_pos;
	m_pos.x += start_mat.m[0][3];
	m_pos.y += start_mat.m[1][3];
	m_pos.z += start_mat.m[2][3];

	//ターゲットは常にプレイヤー
	m_targetPos = player_pos;

#endif

	//ターゲットは常にプレイヤー
	player_pos.y += PLAYER_POS_Y_ADD;
	m_targetPos = player_pos;
}

void CPlayCamera::StepFin()
{
	m_oldPos = m_pos;
	m_oldGoalPos = m_goalPos;

	//プレイヤーを見下ろす
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	VECTOR player_pos = player->GetPos();
	VECTOR player_rot = player->GetRot();

	/*m_distance = CAMERA_START_DISTANCE_POS;
	m_distance.y -= 50PLAYER_POS_Y_ADDf;
	m_distance.z += 50PLAYER_POS_Y_ADDf;*/

	MATRIX all_mat[5] =
	{
		CMyMath::GetTranslateMatrix(m_distance),
		CMyMath::GetPitchMatrix(m_rot.x),
		CMyMath::GetYawMatrix(m_rot.y),
		CMyMath::GetRollMatrix(m_rot.z),
		CMyMath::GetTranslateMatrix(player_pos),
	};

	MATRIX camera_mat = all_mat[4];
	for (int mat_index = 3; mat_index >= 0; --mat_index)
		camera_mat = CMyMath::MatMult(camera_mat, all_mat[mat_index]);

	m_pos.x = camera_mat.m[0][3];
	m_pos.y = camera_mat.m[1][3];
	m_pos.z = camera_mat.m[2][3];

	
	//ターゲットは常にプレイヤー
	player_pos.y += PLAYER_POS_Y_ADD;
	m_targetPos = player_pos;

	Update();
}

//座標セット
void CPlayCamera::Update()
{
	VECTOR camera_pos = m_pos;
	VECTOR target_pos = m_targetPos;
	VECTOR up_vec = m_upVec;

	SetCameraPositionAndTargetAndUpVec(camera_pos, target_pos, up_vec);
}

//カメラが近づく処理
void CPlayCamera::ApproachCamera(float approach, int time)
{
	m_approachLen = approach;
	m_approachTime = time;

	m_approachDistance.y = m_approachLen;
	m_approachDistance.z = -m_approachLen;
}
