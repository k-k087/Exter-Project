#include "DxLib.h"

#include "TitleScene.h"

#include "../Scene.h"
#include "../SceneManager.h"

#include "../../Common.h"

#include "../../System/System.h"

#include "../../Input/InputManager.h"
#include "../../Debug/Debug.h"
#include "../../Sound/SoundManager.h"

#include "../../Collision/Collision.h"

#include "../../Input/Mouse/Mouse.h"

static const char* TITLE_PATH[TITLE_PATH_NUM] =
{
	"Data/Graph/Title/TitleBack.png",
	"Data/Graph/Title/TitleBack.png",
	"Data/Graph/Title/TitleLen2.png",
	"Data/Graph/Title/TitleS1.png",
	"Data/Graph/Title/TitleSBack.png",
	"Data/Graph/Title/Play2.png",
	"Data/Graph/Title/Play.png",
	"Data/Graph/Title/ShutDown2.png",
	"Data/Graph/Title/ShutDown.png",
	"Data/Graph/Cur/Cur.png",
};

static const int PLAY_POS_X = 777;
static const int PLAY_POS_Y = 640;

static const int FIN_POS_X = 1060;
static const int FIN_POS_Y = 640;


//初期化
void CTitleScene::Init()
{
	m_handle[0] = -1;
	m_handle[1] = -1;
	m_sceneCount = m_cur = 0;
	m_handleRectX = WINDOW_WIDTH;
	m_handleAlpha2 = 100.f;
	m_isSE = false;
	
	InitEffect();
}

//ロード
void CTitleScene::Load()
{
	//タイトル画面ロード
	for (int title_index = 0; title_index < TITLE_PATH_NUM; title_index++)
	{
		m_handle[title_index] = LoadGraph(TITLE_PATH[title_index]);
	}

	CSoundManager::LoadBGM(BGM_KIND_TITLE);
	CSoundManager::LoadSE(SE_KIND_SELECT1);
	CSoundManager::LoadSE(SE_KIND_SELECT2);

	m_handleRectX = WINDOW_WIDTH;

	CSoundManager::PlayBGM(BGM_KIND_TITLE);

	m_isCur = true;

	SetMouseDispFlag(false);

	LoadEffect();
}

//通常処理
void CTitleScene::Step()
{
#ifdef DEBUG_MODE


#endif

	switch (m_alphaFlg)
	{
		//	加算
	case 0:
	{
		m_handleAlpha += 0.25f;
		if (m_handleAlpha > 120.f)
		{
			m_alphaFlg = 1;
			m_handleAlpha = 110.f;
		}
	}
	break;

	//	減算	
	case 1:
	{
		m_handleAlpha -= 0.7f;

		if (m_handleAlpha < 60.f)
		{
			m_alphaFlg = 2;
		}
	}
	break;

	//	切り取り
	case 2:
	{
		m_handleRectX -= 30.f;
		m_handleAlpha -= 0.3f;

		if (m_handleRectX < 0.f)
		{
			m_handleAlpha = 0.f;
			m_alphaFlg = 3;
		}
	}
	break;

	case 3:
	{
		m_handleAlpha2 -= 0.8f;

		if (m_handleAlpha2 < 0.f)
		{
			m_handleRectX = WINDOW_WIDTH;
			m_handleAlpha2 = 100.f;
			m_alphaFlg = 0;
		}
	}
	break;

	}

	//	エフェクトステップ
	StepEffect();

	int mouse_x, mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);


	if (CInputManager::IsPush(KEY_INPUT_RIGHT, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_RIGHT, I_PAD))
	{
		m_cur++;

		if (m_cur > 1) { m_cur = 0; }

		m_isCur = false;

		CSoundManager::PlaySE(SE_KIND_SELECT1);
	}
	else if (CInputManager::IsPush(KEY_INPUT_LEFT, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_LEFT, I_PAD))
	{
		m_cur--;

		if (m_cur < 0) { m_cur = 1; }

		m_isCur = false;

		CSoundManager::PlaySE(SE_KIND_SELECT1);
	}

	//Enterキー押されたなら
	if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
	{
		if (m_cur == 0)
		{
			//後処理へ
			CSceneManager::GetManager()->SetFuncID(SCENE_FUNC_ID_FIN);

			CSoundManager::PlaySE(SE_KIND_SELECT2);
			CSoundManager::StopBGM(BGM_KIND_TITLE);
		}
		else if (m_cur == 1)
		{
			//ゲーム終了
			//======================
			CSystem::FinGame();
			//======================

			CSoundManager::PlaySE(SE_KIND_SELECT2);
		}
	}

	//	マウスカーソルがあるならCollision
	if (m_isCur)
	{

		if (CCollision::IsHitRect(PLAY_POS_X, PLAY_POS_Y, 70, 40, mouse_x, mouse_y, 1, 1))
		{
			m_cur = 0;

			if (!m_isSE)
			{
				CSoundManager::PlaySE(SE_KIND_SELECT1);
				m_isSE = true;
			}
		}
		else if (CCollision::IsHitRect(FIN_POS_X, FIN_POS_Y, 110, 40, mouse_x, mouse_y, 1, 1))
		{
			m_cur = 1;

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
}

//描画処理
void CTitleScene::Draw()
{
#ifdef DEBUG_MODE


#endif

	DrawGraph(0, 0, m_handle[TITLE_PATH_BACK1], true);
	DrawRectGraph(0, 0, 0, 0, static_cast<int>(m_handleRectX), WINDOW_HEIGHT, m_handle[TITLE_PATH_BACK2], true);

	if (m_alphaFlg == 2 || m_alphaFlg == 3)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)m_handleAlpha2);
		DrawRectGraph(static_cast<int>(m_handleRectX), 0, static_cast<int>(m_handleRectX),
			0, WINDOW_WIDTH, WINDOW_HEIGHT,
			m_handle[TITLE_PATH_SWORD_BACK], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawGraph(0, 0, m_handle[TITLE_PATH_LEN], true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(m_handleAlpha));
	DrawRectGraph(0, 0, 0, 0, static_cast<int>(m_handleRectX), WINDOW_HEIGHT, m_handle[TITLE_PATH_SWORD], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int play_handle = m_handle[TITLE_PATH_PLAY_D];
	int shutdown_handle = m_handle[TITLE_PATH_SHUTDOWN_D];
	float play_scale = 1.f;
	float fin_scale = 1.f;
	if (m_cur == 0)
	{
		play_handle = m_handle[TITLE_PATH_PLAY_R];
		play_scale = 1.f;
	}
	else if (m_cur == 1)
	{
		shutdown_handle = m_handle[TITLE_PATH_SHUTDOWN_R];
		fin_scale = 1.f;
	}

	
	DrawRotaGraph(PLAY_POS_X, PLAY_POS_Y, play_scale, 0.0, play_handle, true);
	DrawRotaGraph(FIN_POS_X, FIN_POS_Y, fin_scale, 0.0, shutdown_handle, true);

	
	int mouse_x, mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);

	DrawFormatString(10, 10, GetColor(255, 255, 255), "%d, %d", mouse_x, mouse_y);
	

	DrawEffect();
}

//ループ処理
void CTitleScene::Loop()
{
	Step();
	Draw();
}

//後処理
void CTitleScene::Fin()
{
	for (int title_index = 0; title_index < TITLE_PATH_NUM; title_index++)
	{
		DeleteGraph(m_handle[title_index]);
		m_handle[title_index] = -1;
	}

	CSoundManager::Fin();

	FinEffect();

	//セレクト画面へ
	CSceneManager::GetManager()->ChangeScene(SCENE_ID_SELECT);
}

