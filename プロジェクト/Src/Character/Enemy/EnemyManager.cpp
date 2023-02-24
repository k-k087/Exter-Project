#include "DxLib.h"

#include <cassert>

#include "EnemyManager.h"
#include "Enemy.h"
#include "Boss/Boss.h"

#include "NormalEnemy/NormalEnemy.h"
#include "GolemEnemy/GolemEnemy.h"
#include "SonicEnemy/SonicEnemy.h"

#include "Boss/Boss1/Boss1.h"

#include "../CharacterManager.h"

#include "../../Debug/Debug.h"
#include "../../Common.h"

#include "../Effekseer.h"

#include "../../Scene/SceneManager.h"
#include "../../Scene/SelectScene/SelectScene.h"

#include "../Player/PlayerManager.h"

#include "../../Sound/SoundManager.h"




CEnemyManager* CEnemyManager::m_enemyManager = nullptr;

//生成数
static const int NORMAL_ENEMY_CREATE_NUM = 10;
static const int GOLEM_ENEMY_CREATE_NUM = 0;
static const int SONIC_ENEMY_CREATE_NUM = 0;

static const int NORMAL_ENEMY_CREATE_NUM2 = 10;
static const int GOLEM_ENEMY_CREATE_NUM2 = 0;
static const int SONIC_ENEMY_CREATE_NUM2 = 0;

static const int NORMAL_ENEMY_CREATE_NUM3 = 10;
static const int GOLEM_ENEMY_CREATE_NUM3 = 0;
static const int SONIC_ENEMY_CREATE_NUM3 = 0;


static const int NORMAL_ENEMY_CREATE_NUM4 = 10;
static const int GOLEM_ENEMY_CREATE_NUM4 = 0;
static const int SONIC_ENEMY_CREATE_NUM4 = 0;


static const int NORMAL_ENEMY_CREATE_NUM5 = 10;
static const int GOLEM_ENEMY_CREATE_NUM5 = 0;
static const int SONIC_ENEMY_CREATE_NUM5 = 0;

static const int BOSS1_CREATE_NUM = 1;

static const int WARNING_RANK[STAGE_KIND_NUM] =
{
	-1, 3, 3, 2
};



static const int CREATE_NUM[5][ENEMY_KIND_NUM] = {

	{NORMAL_ENEMY_CREATE_NUM, GOLEM_ENEMY_CREATE_NUM, SONIC_ENEMY_CREATE_NUM},
	{NORMAL_ENEMY_CREATE_NUM2, GOLEM_ENEMY_CREATE_NUM2, SONIC_ENEMY_CREATE_NUM2},
	{NORMAL_ENEMY_CREATE_NUM3, GOLEM_ENEMY_CREATE_NUM3, SONIC_ENEMY_CREATE_NUM3},
	{NORMAL_ENEMY_CREATE_NUM4, GOLEM_ENEMY_CREATE_NUM4, SONIC_ENEMY_CREATE_NUM4},
	{NORMAL_ENEMY_CREATE_NUM5, GOLEM_ENEMY_CREATE_NUM5, SONIC_ENEMY_CREATE_NUM5},
};

//敵モデルパス
static const char* NORMAL_ENEMY_PATH = "Data/Model/Character/Enemy/Enemy.x";
static const char* GOLEM_ENEMY_PATH = "Data/Model/Character/Enemy/GolemEnemy/GolemEnemy2.x";
static const char* SONIC_ENEMY_PATH = "Data/Model/Character/Enemy/Sonic/Sonic.x";
static const char* BOSS1_PATH = "Data/MOdel/Character/Enemy/Boss/Boss1/Boss1.x";

static const char* EFFECT_PATH[ENEMY_EFFECT_KIND_NUM] =
{
	"Data/Effect/Barrier.efk",
	"Data/Effect/Fire.efk",
	"Data/Effect/MagicArea.efk",
	"Data/Effect/Beam_Blue.efk",
	"Data/Effect/SonicBoom2.efk",
	"Data/Effect/BigFireBlueGreen.efk",
	"Data/Effect/Blow3.efk",
};

void CEnemyManager::Init()
{
	/*
		生成順に注意！！！！！！！！！！！！！！！！！！！！！
	*/

	m_currentRank = m_oldRank = 0;


	//通常敵生成
	for (int normal_enemy_index = 0; normal_enemy_index < 20; ++normal_enemy_index)
		m_enemyList.push_back(CNormalEnemy::Create(++m_createCount));

	//ゴーレム生成
	for (int golem_enemy_index = 0; golem_enemy_index < 10; ++golem_enemy_index)
		m_enemyList.push_back(CGolemEnemy::Create(++m_createCount));

	//ソニック生成
	for (int sonic_enemy_index = 0; sonic_enemy_index < 10; ++sonic_enemy_index)
		m_enemyList.push_back(CSonicEnemy::Create(++m_createCount));

	//ボス１生成
	for (int boss1_index = 0; boss1_index < 1; ++boss1_index)
		m_enemyList.push_back(CBoss1::Create(++m_createCount));


	//最後に初期化
	for (const auto& enemy_list : m_enemyList)
		enemy_list->Init();

	m_bossEnemyP[0] = m_bossEnemyP[1] = nullptr;
	memset(m_paramB, false, sizeof(m_paramB));
	m_rankTime = 0;
	m_warningFin = false;
}

void CEnemyManager::Load()
{
	//敵モデルのロード
	int model_handle[ENEMY_KIND_NUM] =
	{
		MV1LoadModel(NORMAL_ENEMY_PATH),
		MV1LoadModel(GOLEM_ENEMY_PATH),
		MV1LoadModel(SONIC_ENEMY_PATH),

		MV1LoadModel(BOSS1_PATH),
	};

	//エフェクトロード
	for (int effect_index = 0; effect_index < ENEMY_EFFECT_KIND_NUM; ++effect_index)
	{
		m_effectID[effect_index] = CEffekseerCtrl::LoadData(EFFECT_PATH[effect_index]);
	}

	//デュプリケートする
	for (const auto& enemy_list : m_enemyList)
		enemy_list->Load(model_handle[enemy_list->GetEnemyKind()]);

	//最後にDELETE
	for (int handle_index = 0; handle_index < ENEMY_KIND_NUM; ++handle_index)
		MV1DeleteModel(model_handle[handle_index]);
}

void CEnemyManager::Step()
{

	m_oldRank = m_currentRank;

	for (const auto& enemy_list : m_enemyList)
		enemy_list->Step();

	StepRank();

	if (CSceneManager::GetManager()->GetIsTutorial())return;
	if (m_warningFlg)return;

	StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

	int next_rank = m_currentRank + 1;

	if (next_rank == WARNING_RANK[stage_kind] && m_aliveNum <= 0 &&
		m_warningFin == false)
	{
		m_warningFlg = m_warningFin = true;
		CSceneManager::GetManager()->StopGameTime();
		CSoundManager::StopBGM(BGM_KIND_PLAY);
		CSoundManager::PlaySE(SE_KIND_WARNING);
		return;
	}

	if (m_aliveNum <= 0)
	{

		m_currentRank++;

		memset(m_paramB, false, sizeof(m_paramB));
		m_rankTime = 0;

		CPlayer* player = CPlayerManager::GetManager()->GetPlayer();
		VECTOR player_pos = player->GetPos();
		if (stage_kind == STAGE_KIND_1)
		{
			switch (m_currentRank)
			{
			case 0:
			{

			}
			break;

			case 1:
			{
				VECTOR normal_enemy[2] =
				{
					CCommon::GetPMRandV(player_pos, 170.f, 40.f),
					CCommon::GetPMRandV(player_pos, 170.f, 40.f),
				};
				for (int appear_index = 0; appear_index < 2; ++appear_index)
					AppearEnemy(ENEMY_KIND_NORMAL, normal_enemy[appear_index]);
			}
			break;

			case 2:
			{
				VECTOR normal_enemy[2] =
				{
					{player_pos.x + 120.f, 0.f, player_pos.z + 120.f },
					{player_pos.x - 120.f, 0.f, player_pos.z - 120.f },
				};
				for (int appear_index = 0; appear_index < 2; ++appear_index)
					AppearEnemy(ENEMY_KIND_NORMAL, normal_enemy[appear_index]);

				VECTOR normal_enemy2[2] =
				{
					{player_pos.x + 120.f, 0.f, player_pos.z - 120.f },
					{player_pos.x - 120.f, 0.f, player_pos.z + 120.f },
				};
				for (int appear_index = 0; appear_index < 2; ++appear_index)
					AppearEnemy(ENEMY_KIND_NORMAL, normal_enemy2[appear_index], true);
			}
			break;

			case 3:
			{
				AppearEnemy(ENEMY_KIND_GOLEM, CCommon::GetPMRandV(player_pos, 170.f, 40.f), false, true);
				CSoundManager::StopBGM(BGM_KIND_PLAY);
				CSoundManager::PlayBGM(BGM_KIND_BOSS);
			}
			break;

			case 4:
			{

			}
			break;

			}
		}
		//	Stage2
		else if (stage_kind == STAGE_KIND_2)
		{
			switch (m_currentRank)
			{
			case 0:
			{

			}
			break;
			case 1:
			{
				VECTOR enemy_pos[2] =
				{
					{player_pos.x + 120.f, 0.f, player_pos.z - 120.f },
					{player_pos.x - 120.f, 0.f, player_pos.z + 120.f },
				};
				for (int appear_index = 0; appear_index < 2; ++appear_index)
					AppearEnemy(ENEMY_KIND_NORMAL, enemy_pos[appear_index], true);

				AppearEnemy(ENEMY_KIND_GOLEM, VGet(player_pos.x + 170.f, 0.f, player_pos.z + 170.f));
			}
			break;
			case 2:
			{
				VECTOR enemy_pos[4] =
				{
					{player_pos.x + 120.f, 0.f, player_pos.z },
					{player_pos.x + 150.f, 0.f, player_pos.z },
					{player_pos.x + 180.f, 0.f, player_pos.z },
					{player_pos.x + 210.f, 0.f, player_pos.z },
				};
				for (int appear_index = 0; appear_index < 4; ++appear_index)
					AppearEnemy(ENEMY_KIND_NORMAL, enemy_pos[appear_index], false);

				/*AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));*/
			}
			break;
			case 3:
			{
				AppearEnemy(ENEMY_KIND_GOLEM, CCommon::GetPMRandV(player_pos, 150.f, 40.f), false, true);
				AppearEnemy(ENEMY_KIND_SONIC, CCommon::GetPMRandV(player_pos, 150.f, 40.f), false, true);

				CSoundManager::StopBGM(BGM_KIND_PLAY);
				CSoundManager::PlayBGM(BGM_KIND_BOSS);
			}
			break;
			case 4:
			{

			}

			break;
			}
		}
		//	Stage3
		else if (stage_kind == STAGE_KIND_3)
		{
			switch (m_currentRank)
			{
			case 0:
			{

			}
			break;
			case 1:
			{
				VECTOR normal_enemy_pos[3] =
				{
					CCommon::GetPMRandV(player_pos, 140.f, 40.f),
					CCommon::GetPMRandV(player_pos, 160.f, 40.f),
					CCommon::GetPMRandV(player_pos, 190.f, 40.f),
				};
				bool barrier_flg[5] =
				{
					false,true,false,
				};
				for (int enemy_index = 0; enemy_index < 3; ++enemy_index)
				{
					AppearEnemy(ENEMY_KIND_NORMAL, normal_enemy_pos[enemy_index], barrier_flg[enemy_index]);
				}

				AppearEnemy(ENEMY_KIND_SONIC, VGet(player_pos.x + 180, 0.f, player_pos.z));
				AppearEnemy(ENEMY_KIND_SONIC, VGet(player_pos.x - 180, 0.f, player_pos.z));

			}
			break;
			case 2:
			{
				AppearEnemy(ENEMY_KIND_BOSS1, CCommon::GetPMRandV(player_pos, 120.f, 40.f), true, true);
				CSoundManager::StopBGM(BGM_KIND_PLAY);
				CSoundManager::PlayBGM(BGM_KIND_BOSS);
			}
			break;
			case 3:
			{

			}
			break;
			case 4:
			{

			}

			break;
			}
		}
	}
}

void CEnemyManager::StepRank()
{
	if (CSceneManager::GetManager()->GetIsTutorial())return;
	if (m_warningFlg)return;

	StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

	m_rankTime += FRAME_TIME;

	//	Stage1
	if (stage_kind == STAGE_KIND_1)
	{
		switch (m_currentRank)
		{
		case 0:
		{

		}
		break;

		case 1:
		{
			if (!m_paramB[0])
			{
				if (m_aliveNum <= 1)
				{
					//	1体追加
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 170.f, 40.f), true);

					m_paramB[0] = true;
				}
			}
		}
		break;

		case 2:
		{

		}
		break;

		case 3:
		{
			//	秒数、またはHPの割合で敵を出現させる
			if (!m_paramB[0])
			{
				if (m_rankTime >= 1000 * 6 ||
					m_bossEnemyP[0]->GetHP() <= m_bossEnemyP[0]->GetMaxHP() * 3 / 4)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 170.f, 40.f));
					m_paramB[0] = true;
				}
			}
			else if (!m_paramB[1])
			{
				if (m_rankTime >= 1000 * 24 ||
					m_bossEnemyP[0]->GetHP() <= m_bossEnemyP[0]->GetMaxHP() * 2 / 5)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f), true);
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 170.f, 40.f), true);
					m_paramB[1] = true;
				}
			}

		}
		break;

		case 4:
		{
		}
		break;

		}
	}
	//	Stage2
	else if (stage_kind == STAGE_KIND_2)
	{
		switch (m_currentRank)
		{
		case 0:
			if (!m_paramB[0])
			{
				if (m_aliveNum <= 1)
				{
					//	1体追加
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 170.f, 40.f), true);

					m_paramB[0] = true;
				}
			}

			break;
		case 1:
			if (!m_paramB[0])
			{
				if (m_aliveNum <= 2 || m_rankTime >= 1000 * 10)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f), true);
					m_paramB[0] = true;
				}
			}
			else if (!m_paramB[1])
			{
				if (m_aliveNum <= 1 || m_rankTime >= 1000 * 22)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f), true);
					m_paramB[1] = true;
				}
			}

			break;
		case 2:
			if (!m_paramB[0])
			{
				if (m_rankTime >= 1000 * 2)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					m_paramB[0] = true;
				}
			}
			else if (!m_paramB[1])
			{
				if (m_aliveNum <= 1 || m_rankTime >= 1000 * 5)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					m_paramB[1] = true;
				}
			}
			else if (!m_paramB[2])
			{
				if (m_aliveNum <= 1 || m_rankTime >= 1000 * 9)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					m_paramB[2] = true;
				}
			}
			else if (!m_paramB[3])
			{
				if (m_aliveNum <= 1 || m_rankTime >= 1000 * 13)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					m_paramB[3] = true;
				}
			}

			break;
		case 3:
			if (!m_paramB[0])
			{
				if (m_rankTime >= 1000 * 2)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					m_paramB[0] = true;
				}
			}
			else if (!m_paramB[1])
			{
				if (m_aliveNum <= 1 || m_rankTime >= 1000 * 5)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					m_paramB[1] = true;
				}
			}
			else if (!m_paramB[2])
			{
				if (m_bossEnemyP[0]->GetHP() <= m_bossEnemyP[0]->GetMaxHP() * 2 / 5 ||
					m_bossEnemyP[1]->GetHP() <= m_bossEnemyP[0]->GetMaxHP() * 2 / 5 ||
					m_rankTime >= 1000 * 45)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f));
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 140.f, 40.f));
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 150.f, 40.f));
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 160.f, 40.f));
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 170.f, 40.f));
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 180.f, 40.f));
					m_paramB[2] = true;

					m_bossEnemyP[0]->SetBarrier(VGet(1.5f, 1.5f, 1.5f));
					m_bossEnemyP[1]->SetBarrier(VGet(1.5f, 1.5f, 1.5f));
				}
			}

			break;
		case 4:

			break;
		}
	}
	//	Stage3
	else if (stage_kind == STAGE_KIND_3)
	{
		switch (m_currentRank)
		{
		case 0:
			if (!m_paramB[0])
			{
				if (m_aliveNum <= 2 || m_rankTime >= 1000 * 10)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_GOLEM, CCommon::GetPMRandV(player_pos, 160.f, 40.f), true);
					m_paramB[0] = true;
				}
			}
			else if (!m_paramB[1])
			{
				if (m_aliveNum <= 1 || m_rankTime >= 1000 * 27)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 120.f, 40.f), true);
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 110.f, 40.f), false);
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 130.f, 40.f), true);
					AppearEnemy(ENEMY_KIND_NORMAL, CCommon::GetPMRandV(player_pos, 140.f, 40.f), false);
					m_paramB[1] = true;
				}
			}

			break;
		case 1:
			if (!m_paramB[0])
			{
				if (m_aliveNum <= 2 || m_rankTime >= 1000 * 30)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, VGet(player_pos.x + 150.f, 0.f, player_pos.z + 150));
					AppearEnemy(ENEMY_KIND_NORMAL, VGet(player_pos.x - 150.f, 0.f, player_pos.z - 150));
					AppearEnemy(ENEMY_KIND_NORMAL, VGet(player_pos.x + 150.f, 0.f, player_pos.z));
					AppearEnemy(ENEMY_KIND_NORMAL, VGet(player_pos.x - 150.f, 0.f, player_pos.z));
					m_paramB[0] = true;
				}
			}

			break;
		case 2:
			if (!m_paramB[0])
			{
				if (m_rankTime >= 1000 * 15)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					AppearEnemy(ENEMY_KIND_NORMAL, VGet(player_pos.x + 70.f, 0.f, player_pos.z));
					AppearEnemy(ENEMY_KIND_NORMAL, VGet(player_pos.x - 70.f, 0.f, player_pos.z));
					AppearEnemy(ENEMY_KIND_NORMAL, VGet(player_pos.x, 0.f, player_pos.z + 70.f));
					AppearEnemy(ENEMY_KIND_NORMAL, VGet(player_pos.x, 0.f, player_pos.z - 70.f));

					m_paramB[0] = true;
				}
			}
			else if (!m_paramB[2])
			{
				if (m_bossEnemyP[0]->GetHP() <= m_bossEnemyP[0]->GetMaxHP() / 2 ||
					m_rankTime >= 1000 * 60)
				{
					VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
					VECTOR boss_pos = m_bossEnemyP[0]->GetPos();
					AppearEnemy(ENEMY_KIND_GOLEM, VGet(boss_pos.x, 0.f, boss_pos.z - 70.f), true);
					AppearEnemy(ENEMY_KIND_SONIC, VGet(boss_pos.x, 0.f, boss_pos.z + 70.f), true);
					m_paramB[2] = true;

					m_bossEnemyP[0]->SetBarrier(VGet(1.6f, 1.6f, 1.6f));
				}
			}

			break;
		case 3:


			break;
		case 4:

			break;
		}
	}

}


void CEnemyManager::StartAppear(int stage_num)
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();
	VECTOR player_pos = player->GetPos();

	switch (stage_num)
	{
		//	Stage1
	case 1:
	{
		VECTOR normal_enemy_pos = CCommon::GetPMRandV(player_pos, 170.f, 40.f);
		AppearEnemy(ENEMY_KIND_NORMAL, normal_enemy_pos);
	}
	break;

	//	Stage2
	case 2:
	{
		VECTOR normal_enemy_pos[5] =
		{
			CCommon::GetPMRandV(player_pos, 140.f, 40.f),
			CCommon::GetPMRandV(player_pos, 160.f, 40.f),
			CCommon::GetPMRandV(player_pos, 190.f, 40.f),
			CCommon::GetPMRandV(player_pos, 120.f, 40.f),
			CCommon::GetPMRandV(player_pos, 140.f, 40.f),
		};
		bool barrier_flg[5] =
		{
			false,true,false,true,false,
		};
		for (int enemy_index = 0; enemy_index < 5; ++enemy_index)
		{
			AppearEnemy(ENEMY_KIND_NORMAL, normal_enemy_pos[enemy_index], barrier_flg[enemy_index]);
		}
	}
	break;

	//	Stage3
	case 3:
	{
		VECTOR normal_enemy_pos[3] =
		{
			CCommon::GetPMRandV(player_pos, 140.f, 40.f),
			CCommon::GetPMRandV(player_pos, 160.f, 40.f),
			CCommon::GetPMRandV(player_pos, 190.f, 40.f),
		};
		bool barrier_flg[5] =
		{
			false,true,false,
		};
		for (int enemy_index = 0; enemy_index < 3; ++enemy_index)
		{
			AppearEnemy(ENEMY_KIND_NORMAL, normal_enemy_pos[enemy_index], barrier_flg[enemy_index]);
		}

		AppearEnemy(ENEMY_KIND_GOLEM, CCommon::GetPMRandV(player_pos, 140.f, 40.f));
	}
	break;

	}
}

//マネージャー削除
void CEnemyManager::DeleteManager()
{
	if (!m_enemyManager) { return; }

	//まずは後処理
	Fin();

	//先に中身のプレイヤー削除
	DeleteEnemy();

	//マネージャー削除
	_DELETE(m_enemyManager);

	m_enemyManager = nullptr;
}

void CEnemyManager::Fin()
{
	//ロード関連削除
	const auto& itr_end = m_enemyList.end();
	for (auto& enemy_itr = m_enemyList.begin(); enemy_itr != itr_end; ++enemy_itr)
	{
		int handle = (*enemy_itr)->GetHandle();
		if (handle != -1)
		{
			MV1DeleteModel(handle);

			(*enemy_itr)->SetHandle(-1);
		}
	}
}

int CEnemyManager::RequestEffect(EnemyEffectKind effect_kind, VECTOR pos, bool isLoop, int start_frame)
{
	return CEffekseerCtrl::Request2(m_effectID[effect_kind], pos, start_frame, isLoop);
}

//出現させる
void CEnemyManager::AppearEnemy(EnemyKind enemy_kind, int appear_num)
{
	if (appear_num <= 0)return;

	int appear_count = 0;
	m_aliveNum += appear_num;
	for (const auto& enemy_list : m_enemyList)
	{
		if (enemy_list->GetEnemyKind() != enemy_kind) { continue; }
		if (enemy_list->GetIsAppear()) { continue; }

		if (m_currentRank == 0)
		{

		}
		enemy_list->Appear();
		enemy_list->SetIsAppear(true);
		enemy_list->SetAppearCount(1200);

		//エフェクト出す
		VECTOR enemy_pos = enemy_list->GetPos();
		int handle_id = RequestEffect(ENEMY_EFFECT_KIND_MAGIC_APPEAR, enemy_pos, false);
		CEffekseerCtrl::SetScale(handle_id, VGet(3.f, 3.f, 3.f));
		CEffekseerCtrl::SetSpeed(handle_id, 2.5f);

		++appear_count;

		if (appear_count >= appear_num)
			break;
	}
}
void CEnemyManager::AppearEnemy(EnemyKind enemy_kind, VECTOR start_pos, bool is_barrier, bool boss_flg)
{
	m_aliveNum++;
	for (const auto& enemy_list : m_enemyList)
	{
		if (enemy_list->GetEnemyKind() != enemy_kind) { continue; }
		if (enemy_list->GetIsAppear()) { continue; }

		if (m_currentRank == 0)
		{

		}
		enemy_list->Appear(start_pos, is_barrier);
		enemy_list->SetIsAppear(true);
		enemy_list->SetAppearCount(1200);

		//エフェクト出す
		VECTOR enemy_pos = enemy_list->GetPos();
		int handle_id = RequestEffect(ENEMY_EFFECT_KIND_MAGIC_APPEAR, enemy_pos, false);
		CEffekseerCtrl::SetScale(handle_id, VGet(3.f, 3.f, 3.f));
		CEffekseerCtrl::SetSpeed(handle_id, 2.5f);

		//	ボス情報
		if (boss_flg)
		{
			SetBossEnemyPointer(enemy_list);
		}

		break;
	}
}
void CEnemyManager::AppearAllEnemy()
{
	for (const auto& enemy_list : m_enemyList)
	{
		if (enemy_list->GetIsAppear()) { continue; }

		enemy_list->SetIsAppear(true);
		enemy_list->SetAppearCount(1200);

		//エフェクト出す
		VECTOR enemy_pos = enemy_list->GetPos();
		int handle_id = RequestEffect(ENEMY_EFFECT_KIND_MAGIC_APPEAR, enemy_pos, false);
		CEffekseerCtrl::SetScale(handle_id, VGet(3.f, 3.f, 3.f));
		CEffekseerCtrl::SetSpeed(handle_id, 2.5f);
	}
}

//敵生成
void CEnemyManager::CreateEnemy()
{
	//CPlayer* new_player = CPlayer::Create();

	////情報を反映
	//m_playerManager->SetPlayer(new_player);
}

//敵削除
void CEnemyManager::DeleteEnemy()
{
	const auto& itr_end = m_enemyList.end();
	for (auto& enemy_itr = m_enemyList.begin(); enemy_itr != itr_end;)
	{
		(*enemy_itr)->Fin();

		auto& del_itr = enemy_itr++;

		_DELETE(*del_itr);
	}
}

bool CEnemyManager::IsDownBoss()
{
	bool is_fin[2] = { false };

	if (m_bossEnemyP[0] == nullptr &&
		m_bossEnemyP[1] == nullptr)
	{
		return false;
	}

	for (int boss_index = 0; boss_index < 2; boss_index++)
	{
		if (!m_bossEnemyP[boss_index])
		{
			is_fin[boss_index] = true;
			continue;
		}

		if (m_bossEnemyP[boss_index]->GetHP() <= 0)
		{
			is_fin[boss_index] = true;
		}
	}

	if (is_fin[0] && is_fin[1])
		return true;

	return false;
}

void CEnemyManager::KillAllEnemy()
{
	for (const auto& enemy_list : m_enemyList)
	{
		if (!enemy_list->GetIsAlive())
		{
			continue;
		}

		enemy_list->Killing();
	}
}