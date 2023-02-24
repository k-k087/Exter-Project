#include "DxLib.h"

#include "PlayScene.h"

#include "../Scene.h"

#include "../../Character/CharacterManager.h"
#include "../../Character/Player/PlayerManager.h"
#include "../../Character/Player/Player.h"
#include "../../Character/Enemy/EnemyManager.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../Collision/CollisionManager.h"
#include "../../Input/Input.h"
#include "../../Common.h"
#include "../../Camera/CameraManager.h"
#include "../../Input/Input.h"
#include "../../Debug/Debug.h"
#include "../../Field/Field.h"
#include "../../Character/Player/Player.h"
#include "../../Scene/SceneManager.h"
#include "../../Sky/SkyManager.h"
#include "../../Field/FieldManager.h"
#include "../../GameManager/GameManager.h"
#include "../../Draw/DrawManager.h"
#include "../../Sound/SoundManager.h"
#include "../../Score/Score.h"
#include "../../SaveData/SaveData.h"
#include "../../UI/UIManager.h"
#include "../../Magic/MagicManager.h"
#include "../../Effekseer/Effekseer.h"
#include "../../Beam/BeamManager.h"
#include "../../MyMath/MyMath.h"

#include "../../Text/TextData.h"

#include "../../Input/InputManager.h"

static const int TUTORIAL_TEXT_NUM[TUTORIAL_NUM] =
{
	TUTORIAL_TEXT_START_NUM,
	TUTORIAL_TEXT_AVOID_NUM,
	TUTORIAL_TEXT_MOVE_NUM,
	TUTORIAL_TEXT_ENEMY_NUM,
	TUTORIAL_TEXT_ATTACK_NUM,
	TUTORIAL_TEXT_ATTACK2_NUM,
	TUTORIAL_TEXT_STRONG_ATTACK_NUM,
	TUTORIAL_TEXT_BOOST_GO_NUM,
	TUTORIAL_TEXT_BOOST_ATTACK_NUM,
	TUTORIAL_TEXT_END_NUM,
};

static const char** TUTORIAL_TEXT[TUTORIAL_NUM] =
{
	TUTORIAL_TEXT_START,
	TUTORIAL_TEXT_AVOID,
	TUTORIAL_TEXT_MOVE,
	TUTORIAL_TEXT_ENEMY,
	TUTORIAL_TEXT_ATTACK,
	TUTORIAL_TEXT_ATTACK2,
	TUTORIAL_TEXT_STRONG_ATTACK,
	TUTORIAL_TEXT_BOOST_GO,
	TUTORIAL_TEXT_BOOST_ATTACK,
	TUTORIAL_TEXT_END,
};

CPlayScene::CPlayScene()
{

}
CPlayScene::~CPlayScene()
{
	m_textInfo.~CText();
}

//�v���C������
void CPlayScene::Init()
{
	//Draw�}�l�[�W���[����
	CDrawManager::CreateManager()->Init();

	//�L�����}�l�[�W���[����
	CCharacterManager::CreateManager()->Init();
	CPlayerManager::CreateManager()->Init();
	CEnemyManager::CreateManager()->Init();
	//�v���C���[����
	CPlayerManager::CreatePlayer();


	//�J�����}�l�[�W���[����
	CCameraManager::CreateManager()->Init();

	//�t�B�[���h
	CFieldManager::CreateManager()->Init();


	//�X�J�C�}�l�[�W���[����
	CSkyManager::CreateManager()->Init();

	//���@�}�l�[�W���[
	CMagicManager::CreateManager()->Init();

	//�r�[���}�l�[�W���[����
	CBeamManager::CreateManager()->Init();

	//�X�R�A
	CScore::Init();

	////�Z�[�u�f�[�^
	//CSaveData::Init();

	//�����蔻��Ǘ��N���X
	CCollisionManager::CreateManager()->Init();

	CSceneManager::GetManager()->SetGameTime(0);



	//���[�U�C���^�t�F�[�X
	CUIManager::CreateManager()->CreateUI(UI_KIND_PLAY);
	CUIManager::GetManager()->Init();


	//�G�t�F�N�g
	CEffekseerCtrl::Init(100, 5000);


	//�t���O�Z�b�g
	CSceneManager* scene_manager = CSceneManager::GetManager();

	scene_manager->SetClearFlg(false);
	scene_manager->SetFinFlg(false);

	m_startcount = 0;
	m_finCount = 0;
	m_textCount = 0;
	m_isStart = false;
	m_isText = false;
	m_isTutorial = false;
	m_finCountFlg = false;

	m_fontHandle = CreateFontToHandle(nullptr, 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE);


	if (scene_manager->GetIsTutorial())
	{
		m_tutorialPhase = TUTORIAL_START;
		m_tutorialState = TUTORIAL_STATE_STOP;
		m_isTutorial = true;
		m_stopCount = 1000;
		m_isCurrentTutorialEnd = false;
		m_playCount = 1000;
		memset(m_checkTutorial, false, sizeof(m_checkTutorial));
	}

	if (CSceneManager::GetManager()->GetCuurentStage() == STAGE_KIND_NONE) { CSceneManager::GetManager()->SetCurrentStage(STAGE_KIND_1); }
}

//�v���C���[�h
void CPlayScene::Load()
{
	//�X�J�C���[�h
	CSkyManager::GetManager()->Load();
	CFieldManager::GetManager()->Load();

	//�v���C���[���[�h
	CPlayerManager::GetManager()->Load();

	//�G���[�h
	CEnemyManager::GetManager()->Load();

	//�T�E���h���[�h

	//���@���[�h
	CMagicManager::GetManager()->Load();

	//�r�[�����[�h
	CBeamManager::CreateManager()->Load();
	

	//�J�������[�h
	CCameraManager::GetManager()->Load();

	//�X�R�A�I�[�h
	//CScore::Load();


	CUIManager::GetManager()->Load();

	//�t�F�[�Y�����[�v��
	ChangePhase(PLAY_PHASE_START);

	m_textInfo.Load();
	m_TextBackHandle = LoadGraph("Data/Graph/Text/BoxBlue.png");
	CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_CLEAR);

	SetMouseDispFlag(false);

	//========================================
	CSoundManager::LoadSE(SE_KIND_ATTACK1);
	CSoundManager::LoadSE(SE_KIND_ATTACK2);
	CSoundManager::LoadSE(SE_KIND_ATTACK3);
	CSoundManager::LoadSE(SE_KIND_ATTACK4);
	CSoundManager::LoadSE(SE_KIND_BEAM1);
	CSoundManager::LoadSE(SE_KIND_DAMAGE1);
	CSoundManager::LoadSE(SE_KIND_DAMAGE2);
	CSoundManager::LoadSE(SE_KIND_HIT1);
	CSoundManager::LoadSE(SE_KIND_HIT2);
	CSoundManager::LoadSE(SE_KIND_HIT3);
	CSoundManager::LoadSE(SE_KIND_THUNDER);
	CSoundManager::LoadSE(SE_KIND_KO);
	CSoundManager::LoadSE(SE_KIND_SELECT1);
	CSoundManager::LoadSE(SE_KIND_SELECT2);
	CSoundManager::LoadSE(SE_KIND_SHOOT1);
	CSoundManager::LoadSE(SE_KIND_SWORD1);
	CSoundManager::LoadSE(SE_KIND_SWORD_SUPER);
	CSoundManager::LoadSE(SE_KIND_WARNING);

	CSoundManager::LoadBGM(BGM_KIND_PLAY);
	CSoundManager::LoadBGM(BGM_KIND_BOSS);

	CSoundManager::PlayBGM(BGM_KIND_PLAY);

	//========================================

	//========================================
	CSceneManager* scene_manager = CSceneManager::GetManager();
	if (scene_manager->GetIsTutorial())
	{
		GoLoop();
	}
	//========================================
}

//�v���C�ʏ폈��
void CPlayScene::Step()
{
	if (m_phase != PLAY_PHASE_PAUSE)
	{
		SetMousePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}

	//�|�[�Y���
	if (CInputManager::IsPush(KEY_INPUT_ESCAPE, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_START, I_PAD))
	{
		ChangePhase(PLAY_PHASE_PAUSE);
	}

	if (m_isTutorial)
	{
		if (m_phase == PLAY_PHASE_PAUSE)
			StepPause();
		else if (m_phase == PLAY_PHASE_FIN)
			StepFin();
		else
		{
			StepTutorial();
		}
		return;
	}


	//���݂̃t�F�[�Y�̃X�e�b�v��
	(this->*PlayPhaseFunc[m_phase])();


	CUIManager::GetManager()->Step();

	CAnimeInfo::AllStopAnimeStep();

	m_textInfo.Update();

#ifdef DEBUG_MODE


#endif
}
void CPlayScene::StepTutorial()
{
	switch (m_tutorialState)
	{
	case TUTORIAL_STATE_STOP:

		//��莞�ԓ����Ȃ�
		m_stopCount -= FRAME_TIME;

		if (m_stopCount <= 0)
		{
			m_textInfo.Request(TUTORIAL_TEXT[m_tutorialPhase][m_textCount++], 5);
			m_isText = true;
			m_tutorialState = TUTORIAL_STATE_TEXT;
			m_stopCount = 500;
		}

		CPlayerManager::GetManager()->GetPlayer()->StepStop();
		break;

	case TUTORIAL_STATE_TEXT:

		if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY) || 
			CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
		{
			if (m_textCount >= TUTORIAL_TEXT_NUM[m_tutorialPhase] &&
				m_textInfo.IsEnd())
			{
				m_isText = false;
				m_textCount = 0;
				m_tutorialState = TUTORIAL_STATE_ACTION;
			}
			else
			{
				if (m_textInfo.IsEnd())
				{
					m_textInfo.Request(TUTORIAL_TEXT[m_tutorialPhase][m_textCount++], 5);
				}
				else
				{
					m_textInfo.SetAllText();
				}
			}

		}

		break;

	case TUTORIAL_STATE_ACTION:
		CCameraManager::GetManager()->Step();
		CPlayerManager::GetManager()->GetPlayer()->StepTutorial();

		if(!m_isCurrentTutorialEnd)
			if (CheckTutorial())
				m_isCurrentTutorialEnd = true;

			

		CEnemyManager::GetManager()->Step();
		CGameManager::StepAllManagerN(CManagerBase::MG_KIND_CHARACTER, CManagerBase::MG_KIND_COLLISION);
		CCollisionManager::GetManager()->CheckTutorialCollision(static_cast<TutorialPhase>(m_tutorialPhase));
		CUIManager::GetManager()->Step();

		//	�V�т���������
		if (m_isCurrentTutorialEnd)
		{
			m_playCount -= FRAME_TIME;
			
			if (m_playCount <= 0)
			{
				m_isCurrentTutorialEnd = false;

				if (m_tutorialPhase == TUTORIAL_ATTACK2)
				{
					CPlayerManager::GetManager()->GetPlayer()->HealingMP(500.f);
				}
				else if (m_tutorialPhase == TUTORIAL_STRONG_ATTACK)
				{
					CPlayerManager::GetManager()->GetPlayer()->ChargeBoost(500.f);
				}
				

				m_tutorialPhase++;
				m_playCount = 2000;
				m_tutorialState = TUTORIAL_STATE_STOP;

				//�I��
				if (m_tutorialPhase == TUTORIAL_NUM)
				{
					CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_SELECT);
					//�t�F�[�Y���I����
					ChangePhase(PLAY_PHASE_FIN);
				}
			}
		}
		//	T�L�[�A�܂��͏\����L�[�ł�����x�ǂ�
		else if (CInputManager::IsPush(KEY_INPUT_T, I_KEY) ||
			CInputManager::IsPush(XINPUT_BUTTON_DPAD_UP, I_PAD))
		{
			m_textInfo.Request(TUTORIAL_TEXT[m_tutorialPhase][m_textCount++], 5);
			m_isText = true;
			m_tutorialState = TUTORIAL_STATE_TEXT;
		}
	}

	CAnimeInfo::AllStopAnimeStep();
	m_textInfo.Update();

}

bool CPlayScene::CheckTutorial()
{
	switch (m_tutorialPhase)
	{
		//	�ړ�
	case TUTORIAL_START:
		if (CPlayerManager::GetManager()->GetPlayer()->IsTutorialMove())
		{
			return true;
		}

		break;

	case TUTORIAL_AVOID:
		if(CPlayerManager::GetManager()->GetPlayer()->IsTutorialAvoid())
		{
			return true;
		}

		break;

	case TUTORIAL_MOVE:
		return true;
		break;
		

	case TUTORIAL_ENEMY:
		//	�G�̐������I�������True
		VECTOR start_pos = CPlayerManager::GetManager()
			->
			GetPlayer()
			->
			GetPos();
		start_pos = CMyMath::VecAdd(start_pos, VGet(170, 0, 100));
		CEnemyManager::GetManager()
			->
			AppearEnemy(ENEMY_KIND_NORMAL, start_pos);
		return true; break;

	case TUTORIAL_ATTACK:
		if (!m_checkTutorial[0])
			m_checkTutorial[0] = 
			CCollisionManager::GetManager()
			->
			IsCheckPlayerAttackHitEnemy();

		if (!m_checkTutorial[1])
			m_checkTutorial[1] = 
			CPlayerManager::GetManager()
			->
			GetPlayer()
			->
			IsTutorialNormalAttack();

		if (m_checkTutorial[0] && m_checkTutorial[1])
		{
			memset(m_checkTutorial, false, sizeof(m_checkTutorial));

			return true;
		}

		break;

	case TUTORIAL_ATTACK2:
		if (!m_checkTutorial[0])
			m_checkTutorial[0] =
			CCollisionManager::GetManager()
			->
			IsCheckPlayerAttackHitEnemy();

		if (!m_checkTutorial[1])
			m_checkTutorial[1] =
			CPlayerManager::GetManager()
			->
			GetPlayer()
			->
			IsTutorialNormalAttack2();

		if (m_checkTutorial[0] && m_checkTutorial[1])
		{
			memset(m_checkTutorial, false, sizeof(m_checkTutorial));

			return true;
		}

		break;

	case TUTORIAL_STRONG_ATTACK:

		if (!m_checkTutorial[0])
			m_checkTutorial[0] =
			CPlayerManager::GetManager()
			->
			GetPlayer()
			->
			IsTutorialStrongAttack();

		if (m_checkTutorial[0])
		{
			memset(m_checkTutorial, false, sizeof(m_checkTutorial));

			return true;
		}

		break;

	case TUTORIAL_BOOST_GO:
		if (CPlayerManager::GetManager()->GetPlayer()->IsTutorialBoostGo())
			return true;

		break;

	case TUTORIAL_BOOST_ATTACK:
		if (CPlayerManager::GetManager()->GetPlayer()->IsTutorialBoostAttack())
			return true;

		break;

	case TUTORIAL_END:

		return true;

		break;
	}

	return false;
}

void CPlayScene::StepText()
{
	if (m_textCount == -1) { return; }

	
}


//�v���C�`�揈��
void CPlayScene::Draw()
{
	if (m_phase != PLAY_PHASE_PAUSE || 
		m_phase != PLAY_PHASE_FIN)
	{
		//�ŋ�
		CEffekseerCtrl::UpdateAutoCamera();
	}

	//���ׂẴI�u�W�F�N�g�`��
	CDrawManager::DrawAllObject();


	//effectDraw
	CMagicManager::GetManager()->Update();
	CBeamManager::GetManager()->Update();
	CEffekseerCtrl::Draw();

	//���݂̃t�F�[�Y��draw��
	(this->*DrawPhaseFunc[m_phase])();


	//�Ō�Ƀe�L�X�g�`��
	if (m_phase != PLAY_PHASE_PAUSE)
	{
		if (m_isTutorial)
		{
			if (m_tutorialState == TUTORIAL_STATE_STOP ||
				m_tutorialState == TUTORIAL_STATE_TEXT)
			{
				//	��ʂ������Â�����
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
				DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(0, 0, 0), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			DrawTextData();
		}
	}

	//	�`���[�g���A�����̕`��
	if (m_isTutorial)
	{
		switch (m_tutorialState)
		{
		case TUTORIAL_STATE_STOP:
		{

		}
			break;
		case TUTORIAL_STATE_TEXT:
		{
			//	Enter�A���N���AB�{�^���őS���`��A�i�E���X
			DrawExtendFormatStringF(WINDOW_WIDTH - 300, 430, 1.f, 1.f, GetColor(255, 255, 255), "Enter�A���N���AB�{�^���őS���`��");
		}
			break;
		case TUTORIAL_STATE_ACTION:
		{
			if (!m_isCurrentTutorialEnd)
			{
				//	T�L�[�A�\����ł�����x�ǂ�
				DrawFormatStringToHandle(200, 30, GetColor(255, 100, 100), m_fontHandle, "T�L�[�A�\����{�^���ł�����x�ǂ�");
			}
		}
			break;
		}
	}
}

void CPlayScene::DrawTextData()
{
	if (!m_isText) {
		return;
	}

	DrawExtendGraph(0, 450, WINDOW_WIDTH, WINDOW_HEIGHT, m_TextBackHandle, true);
	m_textInfo.Draw();
}

void CPlayScene::StepStart()
{
	CPlayerManager::GetManager()->GetPlayer()->Step();

	CCameraManager::GetManager()->Step();

	if(CUIManager::GetManager()->GetUI()->StepStartPhase())
		m_isStart = true;

	if(m_isStart)
		GoLoop();
}

void CPlayScene::GoLoop()
{
	//�t�F�[�Y�����[�v��
	ChangePhase(PLAY_PHASE_LOOP);

	int stage_num = static_cast<int>(CSceneManager::GetManager()->GetCuurentStage());

	CEnemyManager::GetManager()->StartAppear(stage_num);

	//�J�E���g��߂�
	m_startcount = 0;
}

void CPlayScene::StepLoop()
{


	//�V�[���ȊO�̂��ׂẴ}�l�[�W���[�̃X�e�b�v����
	CGameManager::StepAllManager();


	//�v���C���[�����񂾂�StepFin��
	if (CPlayerManager::GetManager()->GetPlayer()->GetHP() <= 0)
	{
		//�t�F�[�Y���I����
		ChangePhase(PLAY_PHASE_FIN);
		CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_GAMEOVER);
	}

	//	�{�X�����ׂĎ��񂾂�Fin��
	if (CEnemyManager::GetManager()->IsDownBoss())
	{
		//�t�F�[�Y���I����
		ChangePhase(PLAY_PHASE_FIN);
		CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_CLEAR);
		CEnemyManager::GetManager()->KillAllEnemy();
		CSoundManager::StopBGM(BGM_KIND_PLAY);
		CSoundManager::StopBGM(BGM_KIND_BOSS);
		CSoundManager::PlaySE(SE_KIND_KO);
	}

	//if (CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY))
	//{
	//	//�t�F�[�Y���I����
	//	ChangePhase(PLAY_PHASE_FIN);
	//	CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_CLEAR);
	//	CEnemyManager::GetManager()->KillAllEnemy();
	//	CSoundManager::StopBGM(BGM_KIND_PLAY);
	//	CSoundManager::StopBGM(BGM_KIND_BOSS);
	//	CSoundManager::PlaySE(SE_KIND_KO);
	//}

	//========================================================================================
	CSceneManager::GetManager()->StepGameTime();
	//========================================================================================

}

void CPlayScene::StepFin()
{
	//	�Q�[���I�[�o�[�Ȃ�
	if (CSceneManager::GetManager()->GetNextSceneID() == SCENE_ID_GAMEOVER)
	{
		CPlayerManager::GetManager()->GetPlayer()->StepFinPhase();
		CSkyManager::GetManager()->Step();
		//	�ŋ�
		CEffekseerCtrl::UpdateAutoCamera();

		m_startcount += FRAME_TIME;

		if (m_startcount > 2000)
		{
			//	�㏈����
			CSceneManager::GetManager()->SetFuncID(SCENE_FUNC_ID_FIN);
		}
	}
	//	�N���A�Ȃ�
	else
	{
		if (!m_finCountFlg)
		{
			m_finCount += FRAME_TIME;

			if (m_finCount >= 120)
			{
				CPlayerManager::GetManager()->GetPlayer()->StepFinPhase();
				CSkyManager::GetManager()->Step();
				CEnemyManager::GetManager()->Step();
				CCameraManager::GetManager()->GetPlayCamera()->StepFin();

				//�ŋ�
				CEffekseerCtrl::UpdateAutoCamera();

				m_finCount = 0;

				m_startcount++;

				if (m_startcount >= 10)
				{
					m_finCountFlg = true;
					m_startcount = 0;
				}
			}
		}
		else
		{
			m_startcount += FRAME_TIME;

			CPlayerManager::GetManager()->GetPlayer()->StepFinPhase();
			CSkyManager::GetManager()->Step();
			CEnemyManager::GetManager()->Step();
			CCameraManager::GetManager()->GetPlayCamera()->StepFin();

			if (m_startcount >= 2000)
			{
				//�㏈����
				CSceneManager::GetManager()->SetFuncID(SCENE_FUNC_ID_FIN);
			}
		}

	}

}

void CPlayScene::StepPause()
{
	////����
	//if (CInputManager::IsPush(KEY_INPUT_1, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_START, I_PAD))
	//{
	//	//�L�����Ă����t�F�[�Y�ɂ���
	//	PlayPhase m = m_phase;
	//	m_phase = m_oldPhase;
	//	m_oldPhase = m;
	//
	//}

	int next = CUIManager::GetManager()->GetUI()->StepPause();

	if(next == 1)
	{
		//�L�����Ă����t�F�[�Y�ɂ���
		PlayPhase m = m_phase;
		m_phase = m_oldPhase;
		m_oldPhase = m;

		SetMouseDispFlag(false);
	}
	if (next == 2)
	{
		//�t�F�[�Y���I����
		ChangePhase(PLAY_PHASE_FIN);
		SetMouseDispFlag(false);
	}
}

void CPlayScene::DrawStart()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 90);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(255, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	CUIManager::GetManager()->GetUI()->DrawStartPhase();

	
	/*CScore::Update();
	CScore::DrawScorePlay();*/
}


void CPlayScene::DrawLoop()
{
#if 0

	CPlayerManager* player_manager = CPlayerManager::GetManager();
	CPlayer* player = player_manager->GetPlayer();
	player->Draw();

	CFieldManager* field_manager = CFieldManager::GetManager();
	field_manager->Draw();

	CSkyManager* sky_manager = CSkyManager::GetManager();
	sky_manager->Draw();

#endif
	////�v���C���[���W�`��
	//VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	//DrawFormatString(10, 10, GetColor(255, 80, 80), "�v���C���[���W[ %.1f, %.1f, %.1f ]", player_pos.x, player_pos.y, player_pos.z);


#if 0

	//�v���C���[���W�`��
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	DrawFormatString(10, 10, GetColor(255, 80, 80), "�v���C���[���W[ %.1f, %.1f, %.1f ]", player_pos.x, player_pos.y, player_pos.z);

	//�v���C���[HP�`��
	int player_hp = CPlayerManager::GetManager()->GetPlayer()->GetHP();
	DrawFormatString(10, 30, GetColor(10, 10, 10), "�v���C���[HP[ %d ]", player_hp);

	//�v���C���[�̏��
	char a[PLAYER_STATE_NUM][256] =
	{
		"���������Ă邺",
		"���邢�Ă��邾��",
		"���[���I�I�I�I",
		"������Ă���������",
		"�U���I�I",
		"���������",
		"�̂����΂����Ȃ�",
		"�����Ă��܂�"
		"����������"
	};
	DrawExtendFormatString(900, 10, 1.0, 1.0, GetColor(255, 20, 20), "�v���C���[�̏�ԁF%s", a[CPlayerManager::GetManager()->GetPlayer()->GetState()]);

	DrawExtendFormatString(10, 70, 1.0, 1.0, GetColor(255, 20, 20), "!!�R���g���[���[�����ł�");
	DrawExtendFormatString(10, 100, 0.7, 1.0, GetColor(20, 20, 20), "Z�L�[orX�{�^���@�@�@�@�@�F�@�����ʏ�U���i�R�i�K�j");
	DrawExtendFormatString(10, 120, 0.7, 1.0, GetColor(20, 20, 20), "C�L�[orB�{�^���@�@�@�@�@�F�@�x���ʏ�U���i�R�i�K�j");
	DrawExtendFormatString(10, 140, 0.7, 1.0, GetColor(20, 20, 20), "�\���L�[or���X�e�B�b�N�@�F�@�ړ�");
	DrawExtendFormatString(10, 160, 0.7, 1.0, GetColor(20, 20, 20), "���V�t�gorA�{�^���@�@�@ �F�@���");
	DrawExtendFormatString(10, 180, 0.7, 1.0, GetColor(20, 20, 20), "X�L�[orY�{�^���@�@�@�@�@�F�@���U���i����MP�̉񕜂������̂ő��p�ł��Ă��܂��j");

	DrawExtendFormatString(10, 220, 1.0, 1.0, GetColor(255, 20, 20), "�p�b�h��p�s���I�I�I�I");
	DrawExtendFormatString(10, 260, 0.7, 1.0, GetColor(20, 20, 20), "LB�����������ƉE�X�e�b�N�ŕ��������߁ARB�������ƃJ�b�R���������U�����ł���");
	DrawExtendFormatString(10, 280, 0.7, 1.0, GetColor(20, 20, 20), "�E�X�e�b�N�ŃJ�����ړ�");

	DrawExtendFormatString(10, 320, 1.0, 1.0, GetColor(10, 10, 10), "memo:�o���A�͋��U���P���ŉ󂹂܂�");





#endif

	//	�{�X�G�|���`��
	CEnemy** boss_p = CEnemyManager::GetManager()->GetBossEnemyPointer();
	if (boss_p[0] != nullptr)
	{
		StageKind stage = CSceneManager::GetManager()->GetCuurentStage();
		if (stage == STAGE_KIND_1)
		{
			//	T�L�[�A�\����ł�����x�ǂ�
			DrawFormatStringToHandle(800, 650, GetColor(255, 100, 100), m_fontHandle, "���S�[���� ��|��");
		}
		else if (stage == STAGE_KIND_2)
		{
			DrawFormatStringToHandle(900, 600, GetColor(255, 100, 100), m_fontHandle, "���S�[���� ��");
			DrawFormatStringToHandle(780, 655, GetColor(255, 100, 100), m_fontHandle, "���F�A�V�J ��|��");
		}
		else if (stage == STAGE_KIND_3)
		{
			DrawFormatStringToHandle(800, 650, GetColor(255, 100, 100), m_fontHandle, "�{�X�S�[���� ��|��");
		}
	}

	//========================================================================================
	if (!m_isTutorial) { CSceneManager::GetManager()->DrawGameTime(); }
	//========================================================================================

	CUIManager::GetManager()->Draw();
}
void CPlayScene::DrawFin()
{
	CUIManager::GetManager()->Draw();
}

void CPlayScene::DrawPause()
{
	CUIManager::GetManager()->GetUI()->DrawPause();
}

//�v���C���[�v����
void CPlayScene::Loop()
{
	Step();
	Draw();
}

//�v���C�㏈��
void CPlayScene::Fin()
{
	CEffekseerCtrl::Exit();

	//Delete����

	//BEAM�}�l�[�W���[�폜
	CBeamManager::GetManager()->DeleteManager();

	////���@�}�l�[�W���[�폜
	CMagicManager::GetManager()->DeleteManager();

	//Draw�}�l�[�W���[�폜
	CDrawManager::GetManager()->DeleteManager();

	//�L�����}�l�[�W���[�폜
	CCharacterManager::GetManager()->DeleteAllManager();

	//�J�����}�l�[�W���[�폜
	CCameraManager::GetManager()->DeleteManager();

	//�X�J�C�}�l�[�W���[�폜
	CSkyManager::GetManager()->DeleteManager();

	//�t�B�[���h�}�l�[�W���[�폜
	CFieldManager::GetManager()->DeleteManager();

	//�����蔻��Ǘ��N���X�폜
	CCollisionManager::GetManager()->DeleteManager();

	//�T�E���h
	CSoundManager::Fin();

	DeleteGraph(m_TextBackHandle);

	DeleteFontToHandle(m_fontHandle);

	//���[�U�C���^�t�F�[�X
	CUIManager::GetManager()->DeleteManager();

	//CGameManager::DeleteAllList();

	m_textInfo.~CText();
	
	CSceneManager::GetManager()->ChangeScene(CSceneManager::GetManager()->GetNextSceneID());
	
	CSceneManager::GetManager()->SetIsTutorial(false);

	SetMouseDispFlag(true);
}


void CPlayScene::JudgementClear()
{
	//�Q�[���I������
	CSceneManager::GetManager()->SetFinFlg(true);

	//�N���A�t���O��true��
	CSceneManager::GetManager()->SetClearFlg(true);
}

void CPlayScene::ChangePhase(PlayPhase phase)
{
	if (phase == m_phase) { return; }

	if (phase == PLAY_PHASE_PAUSE)
	{
		SetMouseDispFlag(true);
	}

	//���̃t�F�[�Y���L������
	m_oldPhase = m_phase;

	//�Z�b�g
	m_phase = phase;
}