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

//�N���A�w�i�̃p�X
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

static const char* ACTION_BUF1 = "�\���L�[(�{�^��)��3�����̖��O�����߂悤�I";
static const char* ACTION_BUF2 = "Enter�܂���B�{�^���Ō���I";

//�N���A������
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

//�N���A�ʏ폈��
void CClearScene::Step()
{
	(this->*StepPhaseFunc[m_phase])();
}

//�N���A�`�揈��
void CClearScene::Draw()
{
	(this->*DrawPhaseFunc[m_phase])();
}

//�N���A���[�v����
void CClearScene::Loop()
{
	Step();
	Draw();
}

//�N���A�㏈��
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

	//	�ڂ�����
	CSaveData::GetSaveData()->DeleteSaveData();

	//	�Z���N�g��ʂ�
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

	//Enter�L�[�����ꂽ�Ȃ�
	if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY) ||
		CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
	{
		//�㏈����
		CSceneManager::GetManager()->SetFuncID(SCENE_FUNC_ID_FIN);
	}
}


void CClearScene::DrawScore()
{
	DrawGraph(0, 0, m_handle[0], true);

	//	��ʂ�������������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 90);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

	DrawFormatStringToHandle(100, 100, GetColor(255, 255, 30), m_fontHandle[0], CLEAR_BUF[stage_kind]);

	int time = (CSceneManager::GetManager()->GetGameTime() / 1000);
	int minutes = time / 60;
	int seconds = time % 60;
	DrawFormatStringToHandle(300, 250, GetColor(255, 100, 100), m_fontHandle[1], "�N���A�^�C�� �F %02d�� %02d�b", minutes, seconds);


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
