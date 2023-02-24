
#include "../Debug/Debug.h"

#include "Beam.h"
#include "BeamManager.h"
#include "../Common.h"

#include "../Effekseer/Effekseer.h"

#include "../MyMath/MyMath.h"


static const int PLAYER_BEAM_NUM = 100;
static const int ENEMY_BEAM_NUM = 500;

static const int MAX_BEAM_COLL = 20;


BeamDetailInfo BEAM_DETAIL_INFO[BEAM_KIND_NUM] =
{
	//		ビーム種類						サイズ							拡大率				半径		スピード	   間の距離		最大距離		発射間隔（ミリ秒）	  ビームの時間
	{	BEAM_KIND_SONIC_BEAM,		{   10.f,  10.f,  10.f   },		{   1.f,  1.f,  5.f   },	 9.f,	5.f,		30.f,		1000.f,		 100,				1500},
	{	BEAM_KIND_BOSS1_THUNDER,	{   10.f,  10.f,  10.f   },		{   2.f,  2.f,  2.f   },	20.f,	1.f,		25.f,		   3.f,		3000,				2000},

};

static const char* EFFECT_BEAM_PATH[BEAM_KIND_NUM] =
{
	"Data/Effect/Beam_Red.efk",
	"Data/Effect/Thunder.efk"
};


//インスタンス
CBeamManager* CBeamManager::m_beamManager = nullptr;

void CBeamManager::Init()
{
	CreatePlayerBeam();
	CreateEnemyBeam();

	for (auto& player_magic : m_playerBeam)
		player_magic->Init();

	for (auto& enemy_bullet : m_enemyBeam)
		enemy_bullet->Init();

	memset(&m_requestData, 0, sizeof(BeamRequestData));

}
void CBeamManager::Load()
{
	//エフェクトロード
	for (int effect_index = 0; effect_index < BEAM_KIND_NUM; ++effect_index)
	{
		m_beamEffectID[effect_index] = CEffekseerCtrl::LoadData(EFFECT_BEAM_PATH[effect_index]);
	}

	BeamRequestData* request_data = m_requestData;
	for (int beam_index = 0; beam_index < BEAM_MAX_NUM; ++beam_index, ++request_data)
	{
		request_data->isUse = false;
		request_data->requestIndex = -1;
	}
}
void CBeamManager::Step()
{
	BeamRequestData* request_data = m_requestData;
	for (int beam_index = 0; beam_index < BEAM_MAX_NUM; ++beam_index, ++request_data)
	{
		if (!request_data->isUse) { continue; }

		//スタートカウント加算し、Max値まできたら当たり判定玉を飛ばす
		if (!request_data->m_isStart)
		{
			request_data->startCount += FRAME_TIME;

			//たま
			if (request_data->startCount >= request_data->requestInfo.startCollisionTime)
			{
				request_data->startCount = 0;

				//種類をみて、詳細情報をとってくる
				BeamDetailInfo detail_info = BEAM_DETAIL_INFO[request_data->beamKind];

				int max_beam_coll = MAX_BEAM_COLL;
				int count = 0;

				BeamRequestInfo request_info = request_data->requestInfo;

				//空いてるところを探す
				for (auto& enemy_beam : *m_beamManager->GetEnemyBeam())
				{
					if (!enemy_beam->GetIsUse())
					{
						enemy_beam->Request(request_info, detail_info, request_data->requestIndex);
						count++;

						if (count >= max_beam_coll)
						{
							request_data->m_isStart = true;

							break;
						}
						
						VECTOR pos_add = CMyMath::VecScale(request_data->requestInfo.move, detail_info.distance);
						request_info.startPos = CMyMath::VecAdd(request_info.startPos, pos_add);
					}
				}
			}
		}
		else
		{

			request_data->intervalCount += FRAME_TIME;
			request_data->timeCount += FRAME_TIME;

			//間隔をあけて弾飛ばし
			if (request_data->intervalCount >= request_data->interval)
			{
				request_data->intervalCount = 0;

				//空いてるところを探す
				for (auto& enemy_beam : *m_beamManager->GetEnemyBeam())
				{

					if (!enemy_beam->GetIsUse())
					{
						//種類をみて、詳細情報をとってくる
						BeamDetailInfo detail_info = BEAM_DETAIL_INFO[request_data->beamKind];

						enemy_beam->Request(request_data->requestInfo, detail_info, request_data->requestIndex);

						break;
					}
				}
			}

			//最大時間飛ばしたら全部消す
			if (request_data->timeCount >= request_data->maxTime)
			{
				for (auto& enemy_beam : *m_beamManager->GetEnemyBeam())
				{
					if (!enemy_beam->GetIsUse())
					{
						continue;
					}

					if (request_data->requestIndex == enemy_beam->GetRequestIndex())
					{
						enemy_beam->SetIsUse(false);
						enemy_beam->SetRequestIndex(-1);
					}
				}
				request_data->isUse = false;
				request_data->timeCount = 0;

				//エフェクトも止める
				CEffekseerCtrl::Stop(request_data->effectHandleID);
			}
		}
	}

	//プレイヤーの魔法
	for (auto& player_beam : m_playerBeam)
	{
		player_beam->Step();
	}

	//次に敵の魔法
	for (auto& enemy_beam : m_enemyBeam)
	{
		enemy_beam->Step();
	}
}
void CBeamManager::Update()
{
	for (auto& player_beam : m_playerBeam)
		player_beam->Update();

	for (auto& enemy_beam : m_enemyBeam)
		enemy_beam->Update();
}
void CBeamManager::Fin()
{
	//ロード関連
	for (auto& player_beam : m_playerBeam)
	{
		player_beam->Fin();
	}

	for (auto& enemy_beam : m_enemyBeam)
	{
		enemy_beam->Fin();
	}
}


//マネージャー削除
void CBeamManager::DeleteManager()
{
	if (!m_beamManager) { return; }

	//まずは後処理
	Fin();

	//魔法破棄
	DeleteAllBeam();

	//最後に配列クリア
	m_playerBeam.clear();
	m_enemyBeam.clear();

	//マネージャーを消す
	CGameManager::DeleteMGList(m_beamManager);
	_DELETE(m_beamManager);

	m_beamManager = nullptr;
}


//生成
void CBeamManager::CreatePlayerBeam()
{
	//生成し、リストに追加
	for (int bullet_index = 0; bullet_index < PLAYER_BEAM_NUM; ++bullet_index)
	{
		m_playerBeam.push_back(CBeam::Create());
	}
}
void CBeamManager::CreateEnemyBeam()
{
	//生成し、リストに追加
	for (int bullet_index = 0; bullet_index < ENEMY_BEAM_NUM; ++bullet_index)
	{
		m_enemyBeam.push_back(CBeam::Create());
	}
}

//魔法発射処理
void CBeamManager::RequestPlayerBeam(BeamRequestInfo& request, BeamKind kind)
{
	//種類をみて、詳細情報をとってくる
	BeamDetailInfo detail_info = BEAM_DETAIL_INFO[kind];

	//まず、リクエストデータであいているところをみる
	BeamRequestData* request_data = m_beamManager->GetRequestData();
	int beam_index = 0;
	for (; beam_index < BEAM_MAX_NUM; ++beam_index, ++request_data)
	{
		if (request_data->isUse) { continue; }

		//必要情報をいれる
		request_data->beamKind = kind;
		request_data->timeCount = request_data->intervalCount = 0;
		request_data->interval = detail_info.inteval;
		request_data->maxTime = detail_info.maxTime;
		request_data->isUse = true;
		request_data->requestInfo = request;
		request_data->requestIndex = beam_index;

		//エフェクト要請
		request_data->effectHandleID = CEffekseerCtrl::Request(m_beamManager->GetBeamEffectID(kind),
			request_data->requestInfo.startPos, false);

		CEffekseerCtrl::SetRot(request_data->effectHandleID, request_data->requestInfo.rot);
		CEffekseerCtrl::SetScale(request_data->effectHandleID, detail_info.scale);


		break;
	}

	//あいてなければreturn
	if (beam_index >= BEAM_MAX_NUM) { return; }

	//空いてるところを探す
	for (auto & player_beam : *m_beamManager->GetPlayerBeam())
	{
		if (!player_beam->GetIsUse())
		{
			player_beam->Request(request, detail_info, request_data->requestIndex);
			break;
		}
	}
}
void CBeamManager::RequestEnemyBeam(BeamRequestInfo& enemy, BeamKind kind, VECTOR effect_pos)
{
	//種類をみて、詳細情報をとってくる
	BeamDetailInfo detail_info = BEAM_DETAIL_INFO[kind];

	//まず、リクエストデータであいているところをみる
	BeamRequestData* request_data = m_beamManager->GetRequestData();
	int beam_index = 0;
	for (; beam_index < BEAM_MAX_NUM; ++beam_index, ++request_data)
	{
		if (request_data->isUse) { continue; }

		//必要情報をいれる
		request_data->beamKind = kind;
		request_data->timeCount = request_data->intervalCount = 0;
		request_data->interval = detail_info.inteval;
		request_data->maxTime = detail_info.maxTime;
		request_data->isUse = true;
		request_data->requestInfo = enemy;
		request_data->requestIndex = beam_index;
		request_data->startCount = 0;
		request_data->m_isStart = false;

		//エフェクト要請
		request_data->effectHandleID = CEffekseerCtrl::Request2(m_beamManager->GetBeamEffectID(kind),
			effect_pos, enemy.startEffectTime, false);
		CEffekseerCtrl::SetSpeed(request_data->effectHandleID, enemy.effectSpeed);
		CEffekseerCtrl::SetScale(request_data->effectHandleID, detail_info.scale);
		CEffekseerCtrl::SetRot(request_data->effectHandleID, request_data->requestInfo.rot);

		break;
	}
}

//すべて削除
void CBeamManager::DeleteAllBeam()
{
	//中身も消す
	if (m_playerBeam.size() != 0)
	{
		std::vector<CBeam*>::iterator player_beam = m_playerBeam.begin();

		for (; player_beam != m_playerBeam.end(); ++player_beam)
		{
			//念のため後処理
			(*player_beam)->Fin();

			_DELETE(*player_beam);
		}
	}

	//なかみも消す
	if (m_enemyBeam.size() != 0)
	{
		std::vector<CBeam*>::iterator enemy_beam = m_enemyBeam.begin();

		for (; enemy_beam != m_enemyBeam.end(); ++enemy_beam)
		{
			//念のため後処理
			(*enemy_beam)->Fin();

			_DELETE(*enemy_beam);
		}
	}
}
