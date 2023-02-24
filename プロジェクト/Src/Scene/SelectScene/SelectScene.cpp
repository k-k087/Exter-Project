#include "DxLib.h"

#include "SelectScene.h"

#include "../Scene.h"
#include "../SceneManager.h"

#include "../../Common.h"

#include "../../Input/InputManager.h"
#include "../../Debug/Debug.h"
#include "../../Sound/SoundManager.h"

#include "../../Input/Mouse/Mouse.h"
#include "../../Collision/Collision.h"

#include "../../Sound/SoundManager.h"

static const char* SELECT_PATH = "Data/Graph/Select/SelectBack.png";
static const char* SELECT_KIND_PATH = "Data/Graph/Select/SelectUI.png";

static const int TUTORIAL_POS_X = WINDOW_WIDTH / 2 - 300;
static const int TUTORIAL_POS_Y = WINDOW_HEIGHT / 2 - 200;

static const int STAGE1_POS_X = WINDOW_WIDTH / 2 - 300;
static const int STAGE1_POS_Y = WINDOW_HEIGHT / 2 - 120;
static const int STAGE2_POS_X = WINDOW_WIDTH / 2 - 300;
static const int STAGE2_POS_Y = WINDOW_HEIGHT / 2 - 40;
static const int STAGE3_POS_X = WINDOW_WIDTH / 2 - 300;
static const int STAGE3_POS_Y = WINDOW_HEIGHT / 2 + 40;

static const int STAGEEX_POS_X = WINDOW_WIDTH / 2 - 300;
static const int STAGEEX_POS_Y = WINDOW_HEIGHT / 2 + 120;

static const int TITLE_POS_X = WINDOW_WIDTH / 2 - 300;
static const int TITLE_POS_Y = WINDOW_HEIGHT / 2 + 200;

static const VECTOR_I SELECT_UI_POS[CSelectScene::SELECT_KIND_NUM] =
{
	{ TUTORIAL_POS_X, TUTORIAL_POS_Y, 0},
	{ STAGE1_POS_X, STAGE1_POS_Y, 0},
	{ STAGE2_POS_X, STAGE2_POS_Y, 0},
	{ STAGE3_POS_X, STAGE3_POS_Y, 0},
	{ STAGEEX_POS_X, STAGEEX_POS_Y, 0},
	{ TITLE_POS_X, TITLE_POS_Y, 0},
};


//初期化
void CSelectScene::Init()
{
	m_handle = -1;
	m_sceneCount = 0;
	m_cur = 0;
	m_isSE = false;

	InitEffect();
}

//ロード
void CSelectScene::Load()
{
	//セレクト画面ロード
	m_handle = LoadGraph(SELECT_PATH);

	LoadDivGraph(SELECT_KIND_PATH, SELECT_KIND_NUM, 1, SELECT_KIND_NUM, 600, 100, m_selectHandle);

	CSoundManager::LoadSE(SE_KIND_SELECT1);
	CSoundManager::LoadSE(SE_KIND_SELECT2);

	LoadEffect();
}

//通常処理
void CSelectScene::Step()
{
#ifdef DEBUG_MODE


#endif


	if (CInputManager::IsPush(KEY_INPUT_UP, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_UP, I_PAD))
	{
		m_cur--;
		if (m_cur < 0) { m_cur = 5; }

		if (!CSceneManager::GetManager()->GetIsOpenEX() && m_cur == 4)
		{
			m_cur = 3;
		}

		m_isCur = false;

		CSoundManager::PlaySE(SE_KIND_SELECT1);
	}
	else if (CInputManager::IsPush(KEY_INPUT_DOWN, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_DOWN, I_PAD))
	{
		m_cur++;
		if (m_cur > 5) { m_cur = 0; }

		if (!CSceneManager::GetManager()->GetIsOpenEX() && m_cur == 4)
		{
			m_cur = 5;
		}

		m_isCur = false;

		CSoundManager::PlaySE(SE_KIND_SELECT1);
	}

	//	決定
	if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY) || 
		CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
	{
		CSceneManager* scene_manager = CSceneManager::GetManager();

		//	０ならチュートリアル
		if (m_cur == 0)
		{
			scene_manager->SetNextSceneID(SCENE_ID_PLAY);
			scene_manager->SetIsTutorial(true);

			m_stageKind = STAGE_KIND_TUTORIAL;

			CSoundManager::PlaySE(SE_KIND_SELECT2);
		}
		//	１ならステージ１
		else if (m_cur == 1)
		{
			scene_manager->SetNextSceneID(SCENE_ID_PLAY);
			scene_manager->SetIsTutorial(false);

			m_stageKind = STAGE_KIND_1;

			CSoundManager::PlaySE(SE_KIND_SELECT2);
		}
		//	2ならステージ2
		else if (m_cur == 2)
		{
			scene_manager->SetNextSceneID(SCENE_ID_PLAY);
			scene_manager->SetIsTutorial(false);

			m_stageKind = STAGE_KIND_2;

			CSoundManager::PlaySE(SE_KIND_SELECT2);
		}
		//	3ならステージ3
		else if (m_cur == 3)
		{
			scene_manager->SetNextSceneID(SCENE_ID_PLAY);
			scene_manager->SetIsTutorial(false);

			m_stageKind = STAGE_KIND_3;

			CSoundManager::PlaySE(SE_KIND_SELECT2);
		}
		//	4ならEXへ
		else if (m_cur == 4)
		{
			scene_manager->SetNextSceneID(SCENE_ID_PLAY);
			scene_manager->SetIsTutorial(false);

			m_stageKind = STAGE_KIND_EX;

			CSoundManager::PlaySE(SE_KIND_SELECT2);
		}
		//	5ならタイトルへ
		else if (m_cur == 5)
		{
			scene_manager->SetNextSceneID(SCENE_ID_TITLE);

			m_stageKind = STAGE_KIND_NONE;

			CSoundManager::PlaySE(SE_KIND_SELECT2);
		}

		if (m_cur != -1)
		{
			//後処理へ
			scene_manager->SetFuncID(SCENE_FUNC_ID_FIN);
		}
	}


	//	マウスカーソルがあるならCollision
	if (m_isCur)
	{
		int mouse_x, mouse_y;
		GetMousePoint(&mouse_x, &mouse_y);

		if (CCollision::IsHitRect(TUTORIAL_POS_X + 130, TUTORIAL_POS_Y + 30, 130, 30, mouse_x, mouse_y, 1, 1))
		{
			m_cur = 0;

			if (!m_isSE)
			{
				CSoundManager::PlaySE(SE_KIND_SELECT1);
				m_isSE = true;
			}
		}
		else if (CCollision::IsHitRect(STAGE1_POS_X + 110, STAGE1_POS_Y + 30, 110, 30, mouse_x, mouse_y, 1, 1))
		{
			m_cur = 1;

			if (!m_isSE)
			{
				CSoundManager::PlaySE(SE_KIND_SELECT1);
				m_isSE = true;
			}
		}
		else if (CCollision::IsHitRect(STAGE2_POS_X + 110, STAGE2_POS_Y + 30, 110, 30, mouse_x, mouse_y, 1, 1))
		{
			m_cur = 2;

			if (!m_isSE)
			{
				CSoundManager::PlaySE(SE_KIND_SELECT1);
				m_isSE = true;
			}
		}
		else if (CCollision::IsHitRect(STAGE3_POS_X + 110, STAGE3_POS_Y + 30, 110, 30, mouse_x, mouse_y, 1, 1))
		{
			m_cur = 3;

			if (!m_isSE)
			{
				CSoundManager::PlaySE(SE_KIND_SELECT1);
				m_isSE = true;
			}
		}
		else if (CCollision::IsHitRect(STAGEEX_POS_X + 120, STAGEEX_POS_Y + 30, 120, 30, mouse_x, mouse_y, 1, 1))
		{
			if (CSceneManager::GetManager()->GetIsOpenEX())
			{
				m_cur = 4;

				if (!m_isSE)
				{
					CSoundManager::PlaySE(SE_KIND_SELECT1);
					m_isSE = true;
				}
			}
		}
		else if (CCollision::IsHitRect(TITLE_POS_X + 190, TITLE_POS_Y + 30, 190, 30, mouse_x, mouse_y, 1, 1))
		{
			m_cur = 5;

			if (!m_isSE)
			{
				CSoundManager::PlaySE(SE_KIND_SELECT1);
				m_isSE = true;
			}
		}
		//	何もないなら-1
		else
		{
			m_cur = -1;
			m_isSE = false;
		}
	}

	StepEffect();
}

//描画処理
void CSelectScene::Draw()
{
#ifdef DEBUG_MODE


#endif

	DrawGraph(0, 0, m_handle, true);


	for (int graph_index = 0; graph_index < SELECT_KIND_NUM; graph_index++)
	{
		if (!CSceneManager::GetManager()->GetIsOpenEX() && graph_index == 4)
		{
			continue;
		}
		int alpha = 60;
		if(m_cur == graph_index)
		{
			alpha = 255;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(SELECT_UI_POS[graph_index].x, SELECT_UI_POS[graph_index].y, m_selectHandle[graph_index], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (GetMouseDispFlag())
	{
		int x, y;
		GetMousePoint(&x, &y);

		DrawFormatString(10, 10, GetColor(255, 255, 255), "%d, %d", x, y);
	}

	DrawEffect();

}


//ループ処理
void CSelectScene::Loop()
{
	Step();
	Draw();
}

//後処理
void CSelectScene::Fin()
{
	CSceneManager::GetManager()->SetCurrentStage(m_stageKind);


	for (int graph_index = 0; graph_index < SELECT_KIND_NUM; graph_index++)
	{
		DeleteGraph(m_selectHandle[graph_index]);
		m_selectHandle[graph_index] = -1;
	}

	DeleteGraph(m_handle);
	m_handle = -1;
	

	CSoundManager::Fin();

	//プレイ画面へ
	CSceneManager::GetManager()->ChangeScene(CSceneManager::GetManager()->GetNextSceneID());
}