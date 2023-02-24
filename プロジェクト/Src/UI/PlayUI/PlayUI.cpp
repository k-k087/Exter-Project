#include "DxLib.h"

#include "PlayUI.h"

#include "../UIManager.h"

#include "../../Character/CharacterManager.h"
#include "../../Character/Player/PlayerManager.h"
#include "../../Character/Enemy/EnemyManager.h"
#include "../../Input/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Camera/CameraManager.h"
#include "../../MyMath/MyMath.h"
#include "../../System/System.h"
#include "../../Sound/SoundManager.h"

#include "../../Collision/Collision.h"

#include "../../Input/Mouse/Mouse.h"

//	パス
static const char* ENEMY_HP_PATH = "Data/Graph/HP/EnemyHP.png";
static const char* ENEMY_HP_BAR_PATH = "Data/Graph/HP/EnemyHPBar.png";
static const char* PLAYER_BAR_PATH = "Data/Graph/HP/PlayerHPBar.png";
static const char* PLAYER_HP_PATH = "Data/Graph/HP/PlayerHP2.png";
static const char* BOSS1_HP_PATH = "Data/Graph/HP/Boss1HP.png";
static const char* BOSS2_HP_PATH = "Data/Graph/HP/Boss2HP.png";
static const char* PLAYER_MP_PATH = "Data/Graph/HP/PlayerMP2.png";
static const char* ANNIHILATE_PATH = "Data/Graph/UI/Senmetu.png";


static const char* STAGE1_PATH = "Data/Graph/UI/Stage1.png";
static const char* STAGE2_PATH = "Data/Graph/UI/Stage2.png";
static const char* STAGE3_PATH = "Data/Graph/UI/Stage3.png";
static const char* STAGE_PATH[STAGE_KIND_NUM] = 
{
	" ",
	"Data/Graph/UI/Stage1.png",
	"Data/Graph/UI/Stage2.png",
	"Data/Graph/UI/Stage3.png",
	" ",
};


static const char* YES_NO_PATH = "Data/Graph/Text/YesNo.png";
static const char* SELECT_PATH = "Data/Graph/Text/GameOrTitle2.png";
static const char* RETURN_TITLE_PATH = "Data/Graph/Text/Title11.png";

static const char* PLAYER_BOOST_PATH = "Data/Graph/HP/PlayerBoostS.png";
static const char* PLAYER_BOOST_N_PATH = "Data/Graph/HP/PlayerBoostN2.png";

static const char* MINI_MAP_PATH = "Data/Graph/UI/Map1.png";
static const char* PLAYER_CUR_PATH = "Data/Graph/UI/PlayerCur.png";
static const char* AKATEN_PATH = "Data/Graph/UI/Akaten.png";

static const char* HOW_TO_PATH = "Data/Graph/UI/HowToPlay2.png";

static const char* GAME_FIN_PATH = "Data/Graph/Text/GameFin.png";

static const char* TIME_PATH = "Data/Graph/Time/Time.png";

static const char* PLATER_STATES_BAR_PATH = "Data/Graph/HP/PlayerStatesBar.png";

static const char* WARNING_PATH = "Data/Graph/Warning/Warning.png";

static const char* SIGN_FRAME_PATH = "Data/Graph/UI/SignFrame.png";

//	定義
static const float NORMAL_ENEMY_HP_LENGTH = 20.f;
static const float NORMAL_ENEMY_BARRIER_HP_LENGTH = 20.f;

static const float BOSS_HP_LENGTH = 700;

static const float PLAYER_HP_LENGTH = 622.f;
static const float PLAYER_MP_LENGTH = 516.f;
static const float PLAYER_BOOST_LENGTH = 449.f;

static const int BOOST_ANIME_TIME = 100;

static const int ANNI_MAX_TIME = 800;

static const int MINI_MAP_CENTER_X = WINDOW_WIDTH - 160;
static const int MINI_MAP_CENTER_Y = 160;

void CPlayUI::Init()
{
	m_playerHPHandle = 
	m_playerMPHandle = 
	m_enemyHPHandle = 
	m_enemyHPBarHandle = 
	m_annihilateHandle =
	m_reallyHandle =
	m_stateHandle  = 
	m_miniMapHandle =
	m_akaten = 
	m_playerCur = 
	m_howToHandle = 
	m_playerStatesBar = -1;

	memset(&m_YesNoHandle, -1, sizeof(m_YesNoHandle));
	memset(&m_SelectHandle, -1, sizeof(m_SelectHandle));
	memset(&m_playerBoostHandle, -1, sizeof(m_playerBoostHandle));
	memset(m_timeHandle, -1, sizeof(m_timeHandle));
	memset(m_warningHandle, -1, sizeof(m_warningHandle));

	m_annihilatePos.y = -500.f;
	m_annihilatePos.x = m_statePos.x = WINDOW_WIDTH / 2.f;
	m_statePos.y = m_annihilatePos.y - WINDOW_HEIGHT;

	m_count1 = 
	m_count2 = 
	m_selectCur = 
	m_yesNoCur = 
	m_currentBoostNum = 0;

	m_boostCount = BOOST_ANIME_TIME;

	m_isStay = 
	m_isThrough[0] = 
	m_isThrough[1] = 
	m_isReturnTitle = 
	m_isHowTo = false;
}
void CPlayUI::Load()
{
	if (m_playerStatesBar == -1)
		m_playerStatesBar = LoadGraph(PLATER_STATES_BAR_PATH);

	if (m_playerHPHandle == -1)
		m_playerHPHandle = LoadGraph(PLAYER_HP_PATH);

	if (m_playerMPHandle == -1)
		m_playerMPHandle = LoadGraph(PLAYER_MP_PATH);

	if (m_playerBar == -1)
		m_playerBar = LoadGraph(PLAYER_BAR_PATH);

	if (m_enemyHPHandle == -1)
		m_enemyHPHandle = LoadGraph(ENEMY_HP_PATH);

	if (m_enemyHPBarHandle == -1)
		m_enemyHPBarHandle = LoadGraph(ENEMY_HP_BAR_PATH);

	if (m_annihilateHandle == -1)
		m_annihilateHandle = LoadGraph(ANNIHILATE_PATH);

	if (m_playerBoostNHandle == -1)
		m_playerBoostNHandle = LoadGraph(PLAYER_BOOST_N_PATH);

	if (m_stateHandle == -1)
	{
		//	ステージ情報をとってくる
		StageKind stage = CSceneManager::GetManager()->GetCuurentStage();
		m_stateHandle = LoadGraph(STAGE_PATH[stage]);
	}

	if (m_bossHP1Handle == -1)
		m_bossHP1Handle = LoadGraph(BOSS1_HP_PATH);

	if (m_bossHP2Handle == -1)
		m_bossHP2Handle = LoadGraph(BOSS2_HP_PATH);

	if (m_reallyHandle == -1)
		m_reallyHandle = LoadGraph(RETURN_TITLE_PATH);

	if (m_miniMapHandle == -1)
		m_miniMapHandle = LoadGraph(MINI_MAP_PATH);

	if (m_playerCur == -1)
		m_playerCur = LoadGraph(PLAYER_CUR_PATH);

	if (m_howToHandle == -1)
		m_howToHandle = LoadGraph(HOW_TO_PATH);

	if (m_akaten == -1)
		m_akaten = LoadGraph(AKATEN_PATH);

	if (m_YesNoHandle[0] == -1)
		LoadDivGraph(YES_NO_PATH, 4, 2, 2, 267, 77, m_YesNoHandle);

	if (m_SelectHandle[0] == -1)
		LoadDivGraph(SELECT_PATH, 6, 2, 3, 267, 77, m_SelectHandle);

	if (m_playerBoostHandle[0] == -1)
		LoadDivGraph(PLAYER_BOOST_PATH, PLAYER_BOOST_HANDLE_NUM, 4, 4, 32, 32, m_playerBoostHandle);

	if (m_gameFinHandle[0] == -1)
		LoadDivGraph(GAME_FIN_PATH, 2, 1, 2, 267, 77, m_gameFinHandle);

	if (m_timeHandle[0] == -1)
		LoadDivGraph(TIME_PATH, 12, 12, 1, 32, 64, m_timeHandle);

	if (m_warningHandle[0] == -1)
		LoadDivGraph(WARNING_PATH, 4, 2, 2, 1280, 720, m_warningHandle);

	if (m_signFrameHandle == -1)
		m_signFrameHandle = LoadGraph(SIGN_FRAME_PATH);
	
	
	m_boostCount = BOOST_ANIME_TIME;
}
void CPlayUI::Step()
{
	if (m_bossFlg)
	{
		m_bossCountHP += 2;

		if (m_bossCountHP >= m_currentBossHP)
		{
			m_bossCountHP = m_currentBossHP;
		}

		for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
		{
			EnemyKind enemy_kind = enemy_list->GetEnemyKind();

			if (enemy_kind != ENEMY_KIND_BOSS1) { continue; }

			//生きていないならcontinue
			if (!enemy_list->GetIsAlive()) { continue; }
			if (!enemy_list->GetIsAppear()) { continue; }

			m_currentBossHP = enemy_list->GetHP();

		}
	}


	//プレイヤーのブースト画像Update処理
	PlayerBoostKind boost_kind = CPlayerManager::GetManager()->GetPlayer()->GetBoostKind();
	if (boost_kind == PLAYER_BOOST_GO ||
		boost_kind == PLAYER_BOOST_NOW)
	{
		m_boostCount -= FRAME_TIME;

		//	更新
		if (m_boostCount <= 0)
		{
			m_boostCount = BOOST_ANIME_TIME;
			m_currentBoostNum != PLAYER_BOOST_HANDLE_NUM - 1 ? m_currentBoostNum++ : m_currentBoostNum = 0;
		}
	}
	else
	{
		m_currentBoostNum = 0;
	}

	if (CEnemyManager::GetManager()->GetWarningFlg())
	{
		StepWarning();
	}
}
void CPlayUI::Update()
{
}
void CPlayUI::Draw()
{
	//敵のHPを計算して描画
	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//生きていないならcontinue
		if (!enemy_list->GetIsAlive()) { continue; }
		if (!enemy_list->GetIsAppear()) { continue; }

		int enemy_hp = enemy_list->GetHP();
		int enemy_max_hp = enemy_list->GetMaxHP();

		int barrier_hp = 0;
		int barrier_max_hp = 0;

		if (enemy_list->GetBarrierEffectID() != -1)
		{
			barrier_hp = enemy_list->GetBarrierHP();
			barrier_max_hp = enemy_list->GetBarrierMaxHP();
		}

		float hp_len = NORMAL_ENEMY_HP_LENGTH * static_cast<float>(enemy_hp) / static_cast<float>(enemy_max_hp);
		float barrier_len = NORMAL_ENEMY_BARRIER_HP_LENGTH * static_cast<float>(barrier_hp) / static_cast<float>(barrier_max_hp);

		VECTOR enemy_pos = enemy_list->GetPos();
		VECTOR barrier_pos = enemy_pos;

		//	すこし左上にずらす
		switch (enemy_list->GetEnemyKind())
		{
		case ENEMY_KIND_NORMAL:	enemy_pos.y += 45.f; barrier_pos.y += 55.f; break;
		case ENEMY_KIND_GOLEM:	enemy_pos.y += 80.f; barrier_pos.y += 90.f; break;
		case ENEMY_KIND_SONIC:	enemy_pos.y += 55.f; barrier_pos.y += 65.f;	break;
		
		case ENEMY_KIND_BOSS1:
			DrawBossHP(enemy_list); continue;
			break;
		}
		enemy_pos.x -= static_cast<float>(NORMAL_ENEMY_HP_LENGTH) / 2.f;
		barrier_pos.x -= static_cast<float>(NORMAL_ENEMY_BARRIER_HP_LENGTH) / 2.f;

		DrawModiBillboard3D(enemy_pos, 0.f, 0.f, NORMAL_ENEMY_HP_LENGTH, 0.f, NORMAL_ENEMY_HP_LENGTH, 2.f, 0.f, 2.f, m_enemyHPBarHandle, true);
		DrawModiBillboard3D(enemy_pos, 0.f, 0.f, hp_len, 0.f, hp_len, 2.f, 0.f, 2.f, m_enemyHPHandle, true);

		if (barrier_hp != 0)
		{
			DrawModiBillboard3D(barrier_pos, 0.f, 0.f, NORMAL_ENEMY_BARRIER_HP_LENGTH, 0.f, NORMAL_ENEMY_BARRIER_HP_LENGTH, 2.f, 0.f, 2.f, m_enemyHPBarHandle, true);
			DrawModiBillboard3D(barrier_pos, 0.f, 0.f, barrier_len, 0.f, barrier_len, 2.f, 0.f, 2.f, m_playerMPHandle, true);
		}
	}


	//プレイヤーのHP、MP,Boostを描画
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	int player_hp = player->GetHP();
	int player_max_hp = player->GetMaxHP();
	float player_mp = player->GetMP();
	float player_max_mp = player->GetMaxMP();

	float player_boost_max = CPlayer::BOOST_MAX_NUM;
	float player_boost_num = player->GetBoostNum();

	float hp_len = PLAYER_HP_LENGTH * static_cast<float>(player_hp) / static_cast<float>(player_max_hp);
	float mp_len = PLAYER_MP_LENGTH * player_mp / player_max_mp;
	float boost_len = PLAYER_BOOST_LENGTH * player_boost_num / player_boost_max;

	//boost_len = 100.f;

	DrawExtendGraph(232, 574, 854, 606, m_playerBar, true);
	DrawExtendGraph(232, 574, 232 + static_cast<int>(hp_len), 606, m_playerHPHandle, true);

	DrawExtendGraph(232, 615, 748, 636, m_playerBar, true);
	DrawExtendGraph(232, 615, 232 + static_cast<int>(mp_len), 636, m_playerMPHandle, true);

	int boost_handle = m_playerBoostNHandle;

	if (CPlayerManager::GetManager()->GetPlayer()->GetBoostKind() != PLAYER_BOOST_NOW)
		boost_handle = m_playerBoostHandle[m_currentBoostNum];

	DrawExtendGraph(172, 647, 621, 662, m_playerBar, true);
	DrawExtendGraph(172, 647, 172 + static_cast<int>(boost_len), 662, boost_handle, true);

	DrawRotaGraph(450, WINDOW_HEIGHT - 130, 1.0, 0.0, m_playerStatesBar, true);


	//	ミニマップ描画

	DrawRotaGraph(MINI_MAP_CENTER_X, MINI_MAP_CENTER_Y, 0.6, 0.0, m_miniMapHandle, true);

	VECTOR player_pos = player->GetPos();
	VECTOR player_rot = player->GetRot();

	VECTOR camera_rot = CCameraManager::GetManager()->GetPlayCamera()->GetRot();

	DrawRotaGraph(MINI_MAP_CENTER_X, MINI_MAP_CENTER_Y, 0.47, player_rot.y + DX_PI_F - camera_rot.y, m_playerCur, true);

	//	プレイヤーと敵の位置描画
	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//生きていないならcontinue
		if (!enemy_list->GetIsAlive()) { continue; }
		if (!enemy_list->GetIsAppear()) { continue; }

		VECTOR enemy_pos = enemy_list->GetPos();

		enemy_pos = CMyMath::VecCreate(player_pos, enemy_pos);

		if (CMyMath::VecLong(enemy_pos) >= 500.f) 
		{ 
			enemy_pos = CMyMath::VecNormalize(enemy_pos);
			enemy_pos = CMyMath::VecScale(enemy_pos, 500.f);
		}

		enemy_pos = CMyMath::VecScale(enemy_pos, 0.27f);

		enemy_pos.x += MINI_MAP_CENTER_X;
		enemy_pos.z = MINI_MAP_CENTER_Y - enemy_pos.z;

		float x = enemy_pos.x - MINI_MAP_CENTER_X;
		float y = enemy_pos.z - MINI_MAP_CENTER_Y;

		float enemy_distance = CMyMath::VecLong(VGet(x, y, 0));
		float enemy_rot = CMyMath::VecDir(x, y);

		enemy_pos.x = cosf(-camera_rot.y - enemy_rot + DX_PI_F / 2.f) * enemy_distance + MINI_MAP_CENTER_X;
		enemy_pos.z = sinf(-camera_rot.y - enemy_rot + DX_PI_F / 2.f) * enemy_distance + MINI_MAP_CENTER_Y;


		DrawRotaGraph(static_cast<int>(enemy_pos.x), static_cast<int>(enemy_pos.z),
			0.3, 0.0, m_akaten, true);
	}

	//	看板描画
	DrawRotaGraph(90, 50, 0.2, 0.0, m_signFrameHandle, true);

	//	整数の直す
	int game_time = (CSceneManager::GetManager()->GetGameTime() / 1000);
	int minutes = game_time / 60;
	int seconds = game_time % 60;

	int handle_index[5] =
	{
		minutes / 10,
		minutes % 10,
		10,
		seconds / 10,
		seconds % 10,
	};
	for (int time_index = 0; time_index < 5; time_index++)
	{
		DrawGraph(15 + time_index * 30, 50, m_timeHandle[handle_index[time_index]], true);
	}

	//	最後にWarning文
	if (CEnemyManager::GetManager()->GetWarningFlg())
	{
		DrawWarning();
	}
}

int CPlayUI::StepPause()
{
	//	ゲーム画面に戻るかタイトルに行くかの選択
	if (m_isHowTo)
	{
		if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
		{
			m_isHowTo = false;
		}

		return 0;
	}

	int mouse_x, mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);

	if (!m_isReturnTitle)
	{
		if (CInputManager::IsPush(KEY_INPUT_UP, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_UP, I_PAD))
		{
			m_selectCur--;
			if (m_selectCur < 0) { m_selectCur = 3; }
		}
		if (CInputManager::IsPush(KEY_INPUT_DOWN, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_DOWN, I_PAD))
		{
			m_selectCur++;
			if (m_selectCur > 3) { m_selectCur = 0; }
		}

		//	決定
		if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_SPACE, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
		{
			//	０ならゲームに戻る
			if (m_selectCur == 0)
			{
				return 1;
			}
			//	1ならゲーム操作画面へ
			else if (m_selectCur == 1)
			{
				m_isHowTo = true;
			}
			//	2ならほんとうにタイトルにもどるかの選択へ
			else if (m_selectCur == 2)
			{
				m_isReturnTitle = true;
			}
			else if (m_selectCur == 3)
			{
				//======================
				CSystem::FinGame();
				//======================
			}
		}

		if (CCollision::IsHitRect(mouse_x, mouse_y, 1, 1, WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 5, 150, 70 / 2))
		{
			m_selectCur = 0;
		}
		else if (CCollision::IsHitRect(mouse_x, mouse_y, 1, 1, WINDOW_WIDTH / 2, WINDOW_HEIGHT * 3 / 5, 150, 70 / 2))
		{
			m_selectCur = 1;
		}
		else if (CCollision::IsHitRect(mouse_x, mouse_y, 1, 1, WINDOW_WIDTH / 2, WINDOW_HEIGHT * 4 / 5, 150, 70 / 2))
		{
			m_selectCur = 2;
		}
		else if (CCollision::IsHitRect(mouse_x, mouse_y, 1, 1, 267 / 4, WINDOW_HEIGHT - 77 / 4, 60, 50 / 2))
		{
			m_selectCur = 3;
		}
	}
	//	ほんとうにタイトルにもどるかの選択
	else
		//////////////////////////////////////////////////aaaa///a/aaaaaaaaaaaaa//a/a/a/a/a/a1456345635231
	{
 		if (CInputManager::IsPush(KEY_INPUT_LEFT, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_LEFT, I_PAD))
		{
			m_yesNoCur--;
			if (m_yesNoCur < 0) { m_yesNoCur = 1; }
		}
		if (CInputManager::IsPush(KEY_INPUT_RIGHT, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_RIGHT, I_PAD))
		{
			m_yesNoCur++;
			if (m_yesNoCur > 1) { m_yesNoCur = 0; }
		}

		//	決定
		if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_SPACE, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
		{
			//	０ならもう一度選択画面へ
			if (m_yesNoCur == 0)
			{
				m_isReturnTitle = false;
			}
			//	１ならタイトルに戻る
			else if (m_yesNoCur == 1)
			{
				CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_TITLE);
				m_yesNoCur = 0;
				return 2;
			}
		}

		if (CCollision::IsHitRect(mouse_x, mouse_y, 1, 1, WINDOW_WIDTH / 4 + 70, 500, 130, 40 ))
		{
			m_yesNoCur = 0;
		}
		else if(CCollision::IsHitRect(mouse_x, mouse_y, 1, 1, WINDOW_WIDTH * 3 / 4 - 70, 500, 130, 40))
		{
			m_yesNoCur = 1;
		}
	}

	return 0;
}
void CPlayUI::DrawPause()
{
	//	まずは普通のDraw
	Draw();

	//	画面を少し暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (m_isHowTo)
	{
		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1.0, 0.0, m_howToHandle, true);

		return;
	}

	//	選択描画
	if (!m_isReturnTitle)
	{
		int return_game = m_selectCur == 0 ? 
			m_SelectHandle[RETURN_GAME_RIGHT] : m_SelectHandle[RETURN_GAME_DARK];

		int how_to = m_selectCur == 1 ?
			m_SelectHandle[HOW_TO_RIGHT] : m_SelectHandle[HOW_TO_DARK];

		int go_title = m_selectCur == 2 ?
			m_SelectHandle[GO_TITLE_RIGHT] : m_SelectHandle[GO_TITLE_DARK];

		int fin_handle = m_selectCur == 3 ?
			m_gameFinHandle[1] : m_gameFinHandle[0];

		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 5, 2.0, 0.0, return_game, true);
		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 3 / 5, 2.0, 0.0, how_to, true);
		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 4 / 5, 2.0, 0.0, go_title, true);
		DrawRotaGraph(267 / 4, WINDOW_HEIGHT - 77 / 4, 0.5, 0.0, fin_handle, true);
	}
	//	タイトルへ描画
	else
	{
		//	ほんとうにもどりますか描画
		DrawRotaGraph(WINDOW_WIDTH / 2, 220, 2.0, 0.0, m_reallyHandle, true);

		//	はい、いいえ描画
		int no_handle = m_yesNoCur == 0 ?
			m_YesNoHandle[NO_RIGHT] : m_YesNoHandle[NO_DARK];

		int yes_handle = m_yesNoCur == 1 ?
			m_YesNoHandle[YES_RIGHT] : m_YesNoHandle[YES_DARK];

		DrawRotaGraph(WINDOW_WIDTH / 4 + 70, 500, 2.0, 0.0, no_handle, true);
		DrawRotaGraph(WINDOW_WIDTH * 3 / 4 - 70, 500, 2.0, 0.0, yes_handle, true);
	}

	int x, y;
	GetMousePoint(&x, &y);

	DrawFormatString(10, 10, GetColor(255, 255, 255), "%d, %d", x, y);
}

void CPlayUI::DrawBossHP(CEnemy* boss)
{
	float hp1 = static_cast<float>(m_bossCountHP);
	float hp2 = 0;
	if (hp1 > m_bossAllHP / 2.f)
	{
		hp2 = hp1 - (m_bossAllHP / 2.f);
		hp1 = m_bossAllHP / 2.f;
	}

	float len1 = (BOSS_HP_LENGTH) * hp1 / (m_bossAllHP / 2.f);
	float len2 = (BOSS_HP_LENGTH) * hp2 / (m_bossAllHP / 2.f);


	DrawExtendGraph(210, 20, 210 + static_cast<int>(BOSS_HP_LENGTH), 45, m_enemyHPBarHandle, true);
	DrawExtendGraph(210, 20, 210 + static_cast<int>(len1), 45, m_bossHP1Handle, true);
	DrawExtendGraph(210, 20, 210 + static_cast<int>(len2), 45, m_bossHP2Handle, true);
	
}

bool CPlayUI::StepStartPhase()
{
	if (!m_isStay)
	{
		m_annihilatePos.y += 15.f;
		m_statePos.y += 15.f;
	}
	else
	{
		m_count1 += FRAME_TIME;

		if (m_count1 >= ANNI_MAX_TIME)
		{
			m_isStay = false;
			m_count1 = 0;
		}
	}

	//中心で止まる処理
	if (!m_isThrough[0])
	{
		if (m_annihilatePos.y >= WINDOW_HEIGHT / 2.f)
		{
			m_annihilatePos.y = WINDOW_HEIGHT / 2.f;
			m_isStay = m_isThrough[0] = true;
		}
	}
	if (!m_isThrough[1])
	{
		if (m_statePos.y >= WINDOW_HEIGHT / 2.f)
		{
			m_statePos.y = WINDOW_HEIGHT / 2.f;
			m_isStay = m_isThrough[1] = true;
		}
	}

	//画面外にでたら始まる
	if (m_statePos.y >= WINDOW_HEIGHT + 200)
	{
		m_count1 = 0;
		return true;
	}

	return false;
}
void CPlayUI::DrawStartPhase()
{
	int anni_pos_x = static_cast<int>(m_annihilatePos.x);
	int anni_pos_y = static_cast<int>(m_annihilatePos.y);
	DrawRotaGraph(anni_pos_x, anni_pos_y, 1.0, 0.0, m_annihilateHandle, true);

	int st_pos_x = static_cast<int>(m_statePos.x);
	int st_pos_y = static_cast<int>(m_statePos.y);
	DrawRotaGraph(st_pos_x, st_pos_y, 1.0, 0.0, m_stateHandle, true);
}

void CPlayUI::Fin()
{
	if (m_playerStatesBar != -1)
		DeleteGraph(m_playerStatesBar); m_playerStatesBar = -1;

	if (m_playerBoostNHandle != -1)
		DeleteGraph(m_playerBoostNHandle); m_playerBoostNHandle = -1;

	if (m_playerHPHandle != -1)
		DeleteGraph(m_playerHPHandle); m_playerHPHandle = -1;
	
	if (m_playerMPHandle != -1)
		DeleteGraph(m_playerMPHandle); m_playerMPHandle = -1;

	if (m_playerBar != -1)
		DeleteGraph(m_playerBar); m_playerBar = -1;

	if (m_enemyHPHandle != -1)
		DeleteGraph(m_enemyHPHandle); m_enemyHPHandle = -1;

	if (m_enemyHPBarHandle != -1)
		DeleteGraph(m_enemyHPBarHandle); m_enemyHPBarHandle = -1;

	if (m_annihilateHandle != -1)
		DeleteGraph(m_annihilateHandle); m_annihilateHandle = -1;

	if (m_stateHandle != -1)
		DeleteGraph(m_stateHandle); m_stateHandle = -1;

	if (m_bossHP1Handle != -1)
		DeleteGraph(m_bossHP1Handle); m_bossHP1Handle = -1;

	if (m_bossHP2Handle != -1)
		DeleteGraph(m_bossHP2Handle); m_bossHP2Handle = -1;

	if (m_reallyHandle != -1)
		DeleteGraph(m_reallyHandle); m_reallyHandle = -1;

	if (m_miniMapHandle != -1)
		DeleteGraph(m_miniMapHandle); m_miniMapHandle = -1;

	if (m_playerCur != -1)
		DeleteGraph(m_playerCur); m_playerCur = -1;

	if (m_akaten != -1)
		DeleteGraph(m_akaten); m_akaten = -1;

	if (m_howToHandle != -1)
		DeleteGraph(m_howToHandle); m_howToHandle = -1;

	if (m_signFrameHandle != -1)
		DeleteGraph(m_signFrameHandle); m_signFrameHandle = -1;

	for (int graph_index = 0; graph_index < 4; graph_index++)
	{
		if (m_YesNoHandle[graph_index] != -1)
			DeleteGraph(m_YesNoHandle[graph_index]); m_YesNoHandle[graph_index] = -1;

		if (m_SelectHandle[graph_index] != -1)
			DeleteGraph(m_SelectHandle[graph_index]); m_SelectHandle[graph_index] = -1;
	}

	for (int graph_index = 0; graph_index < PLAYER_BOOST_HANDLE_NUM; graph_index++)
	{
		if (m_playerBoostHandle[graph_index] != -1)
			DeleteGraph(m_playerBoostHandle[graph_index]); m_playerBoostHandle[graph_index] = -1;
	}

	for (int graph_index = 0; graph_index < 2; graph_index++)
	{
		if (m_gameFinHandle[graph_index] != -1)
			DeleteGraph(m_gameFinHandle[graph_index]); m_gameFinHandle[graph_index] = -1;
	}

	for (int graph_index = 0; graph_index < 12; graph_index++)
	{
		if (m_timeHandle[graph_index] != -1)
			DeleteGraph(m_timeHandle[graph_index]); m_timeHandle[graph_index] = -1;
	}

	for (int graph_index = 0; graph_index < WARNING_NUM; graph_index++)
	{
		if (m_warningHandle[graph_index] != -1)
			DeleteGraph(m_warningHandle[graph_index]); m_warningHandle[graph_index] = -1;
	}
}

CPlayUI* CPlayUI::CreatePlayUI()
{
	return _NEW(CPlayUI);
}

void CPlayUI::SetBossHP(int hp)
{
	m_bossAllHP = hp;
	m_currentBossHP = hp;
	m_bossFlg = true;
}

void CPlayUI::StepWarning()
{
	//	カウント加算
	m_warningCount += FRAME_TIME;

	m_warningBlackMileage += 20.f;

	if (static_cast<int>(m_warningBlackMileage) >= WINDOW_WIDTH)
	{
		m_warningBlackMileage = static_cast<float>(WINDOW_WIDTH);
		m_count2 += FRAME_TIME;

		m_alphaSin += ONE_DEGREE * 10;
	}

	//	終了
	if (m_count2 >= 3000)
	{
		m_count2 = 0;
		m_warningBlackMileage = 0.f;
		CEnemyManager::GetManager()->SetWarningFlg(false);
		CSceneManager::GetManager()->RestartGameTime();
		CSoundManager::StopSE(SE_KIND_WARNING);
	}
	
}

void CPlayUI::DrawWarning()
{
	//	黄色描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawGraph(0, 0, m_warningHandle[WARNING_BACK], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int black_up_x = static_cast<int>(m_warningBlackMileage) - WINDOW_WIDTH;
	int black_down_x = WINDOW_WIDTH - static_cast<int>(m_warningBlackMileage);

	DrawGraph(black_up_x, 0, m_warningHandle[WARNING_BLACK_UP], true);
	DrawGraph(black_down_x, 0, m_warningHandle[WARNING_BLACK_DOWN], true);

	//	文描画
	if (m_count2 > 0)
	{
		m_warningAlpha = sinf(m_alphaSin) * 255.f;
		if (m_warningAlpha < 0) { m_warningAlpha *= -1.f; }

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)m_warningAlpha);
		DrawGraph(0, 0, m_warningHandle[WARNING_LEN], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
