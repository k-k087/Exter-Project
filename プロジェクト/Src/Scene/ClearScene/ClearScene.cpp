#include "DxLib.h"

#include "ClearScene.h"

#include "../Scene.h"
#include "../SceneManager.h"

#include "../../Common.h"

#include "../../Input/InputManager.h"
#include "../../Debug/Debug.h"
#include "../../Score/Score.h"
#include "../../SaveData/SaveData.h"
#include "../../Sound/SoundManager.h"

//クリア背景のパス
static const char* CLEAR_PATH[2] = 
{ 
	"Data/Graph/Clear/Clear.png",
	"Data/Graph/Clear/Clear.png",
};

static const char* CLEAR_BUF[STAGE_KIND_NUM] =
{
	" ",
	"Stage1 Clear!",
	"Stage2 Clear!!",
	"Stage3 Clear!!!",
	"StageEX Clear!!!!",
};

static const char* ACTION_BUF1 = "十字キー(ボタン)で3文字の名前を決めよう！";
static const char* ACTION_BUF2 = "EnterまたはBボタンで決定！";

//クリア初期化
void CClearScene::Init()
{
	m_handle[0] = -1;
	m_handle[1] = -1;
	m_isEndScore = false;

	m_fontHandle[0] = CreateFontToHandle(nullptr, 100, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	m_fontHandle[1] = CreateFontToHandle(nullptr, 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

	CSaveData::CreateSaveData()->Init();

	m_isEndScore = true;
}

void CClearScene::Load()
{
	for (int graph_index = 0; graph_index < 2; ++graph_index)
	{
		m_handle[graph_index] = LoadGraph(CLEAR_PATH[graph_index]);
	}

	m_phase = CLEAR_PHASE_SCORE_AND_NAME;

	CSoundManager::LoadSE(SE_KIND_FANFARE);
	CSoundManager::PlaySE(SE_KIND_FANFARE);

}

//クリア通常処理
void CClearScene::Step()
{
	(this->*StepPhaseFunc[m_phase])();
}

//クリア描画処理
void CClearScene::Draw()
{
	(this->*DrawPhaseFunc[m_phase])();
}

//クリアループ処理
void CClearScene::Loop()
{
	Step();
	Draw();
}

//クリア後処理
void CClearScene::Fin()
{
	DeleteGraph(m_handle[0]);
	m_handle[0] = -1;
	DeleteGraph(m_handle[1]);
	m_handle[1] = -1;

	DeleteFontToHandle(m_fontHandle[0]);
	DeleteFontToHandle(m_fontHandle[1]);

	//	CScore::Fin();
	CSoundManager::Fin();

	//	詳しそう
	CSaveData::GetSaveData()->DeleteSaveData();

	//	セレクト画面へ
	CSceneManager::GetManager()->ChangeScene(SCENE_ID_SELECT);
}

void CClearScene::StepScore()
{
#ifdef DEBUG_MODE


#endif

}
void CClearScene::StepRinking()
{
#ifdef DEBUG_MODE


#endif

	//Enterキー押されたなら
	if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY) ||
		CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
	{
		//後処理へ
		CSceneManager::GetManager()->SetFuncID(SCENE_FUNC_ID_FIN);
	}
}


void CClearScene::DrawScore()
{
	DrawGraph(0, 0, m_handle[0], true);

	//	画面を少し白くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 90);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

	DrawFormatStringToHandle(100, 100, GetColor(255, 255, 30), m_fontHandle[0], CLEAR_BUF[stage_kind]);

	int time = (CSceneManager::GetManager()->GetGameTime() / 1000);
	int minutes = time / 60;
	int seconds = time % 60;
	DrawFormatStringToHandle(300, 250, GetColor(255, 100, 100), m_fontHandle[1], "クリアタイム ： %02d分 %02d秒", minutes, seconds);


	DrawFormatStringToHandle(50, 410, GetColor(255, 255, 255), m_fontHandle[1], ACTION_BUF1);
	DrawFormatStringToHandle(50, 530, GetColor(255, 255, 255), m_fontHandle[1], ACTION_BUF2);

	if (CSaveData::GetSaveData()->WriteData())
	{
		m_phase = CLEAR_PHASE_RANKING;
	}

	/*if (CScore::ResultUpdate())
	{
		m_isEndScore = true;
	}
	CScore::DrawScoreResult();*/

	
}


void CClearScene::DrawRinking()
{
	DrawGraph(0, 0, m_handle[1], true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 90);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	CSaveData::GetSaveData()->DrawData();
}
