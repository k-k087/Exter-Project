#include "DxLib.h"

#include <cassert>

#include "SceneManager.h"
#include "Scene.h"

#include "PlayScene/PlayScene.h"
#include "TitleScene/TitleScene.h"
#include "SelectScene/SelectScene.h"
#include "ClearScene/ClearScene.h"
#include "GameOverScene/GameOverScene.h"

#include "../Input/InputManager.h"
#include "../Fade/Fade.h"
#include "../Debug/Debug.h"
#include "../Common.h"

//�V�[���}�l�[�W���[�錾
CSceneManager* CSceneManager::m_sceneManager = nullptr;

void CSceneManager::CreateManager()
{
	//���݂��Ȃ���ΐV�������
	if (!m_sceneManager)
	{
		m_sceneManager = _NEW(CSceneManager);
	}

	//�������^�C�g����
	m_sceneManager->ChangeScene(SCENE_ID_TITLE);
}

CSceneManager* CSceneManager::GetManager()
{
	if (!m_sceneManager) { return nullptr; }

	return m_sceneManager;
}

void CSceneManager::DeleteManager()
{
	if (!m_sceneManager) { return; }
	
	if (m_scene)
	{
		_DELETE(m_scene);

		m_scene = nullptr;
	}

	_DELETE(m_sceneManager);

	m_sceneManager = nullptr;
	
}

//������
void CSceneManager::Init()
{
	//���݂̃V�[���̏�����
	m_scene->Init();

	//���[�h��
	m_funcID = SCENE_FUNC_ID_LOAD;
}

//���[�h
void CSceneManager::Load()
{
	//���݂̃V�[���̃��[�h
	m_scene->Load();

	CFadeManager::GetManager()->Request(4.f, true);

	//���[�v��
	m_funcID = SCENE_FUNC_ID_LOOP;
}

//���[�v
void CSceneManager::Loop()
{
	//���݂̃��[�v����
	m_scene->Loop();
}

//�㏈��
void CSceneManager::Fin()
{

	(this->*m_fadeStep)();

	if (m_fadeStep != nullptr) { return; }

	//���݂̃V�[���̌㏈����
	m_scene->Fin();

	//���̏������ɍs��
	m_funcID = SCENE_FUNC_ID_INIT;
}

void CSceneManager::ChangeScene(SceneID scene_id)
{
	//����Ȃ����
	if (m_scene)
	{
		_DELETE(m_scene);

		m_scene = nullptr;
	}

	switch (scene_id)
	{
	case SCENE_ID_TITLE:	m_scene = _NEW(CTitleScene); break;

	case SCENE_ID_SELECT:	m_scene = _NEW(CSelectScene); break;

	case SCENE_ID_PLAY:		m_scene = _NEW(CPlayScene); break;

	case SCENE_ID_CLEAR:	m_scene = _NEW(CClearScene); break;

	case SCENE_ID_GAMEOVER: m_scene = _NEW(CGameOverScene); break;
	}

	//��������
	m_sceneID = scene_id;
	m_funcID = SCENE_FUNC_ID_INIT;
}

void CSceneManager::CurrrentSceneFunc()
{
	(this->*SceneFunc[m_funcID])();
}

void CSceneManager::SetFuncID(SceneFuncID current_func_id)
{
	m_funcID = current_func_id;

	if (current_func_id == SCENE_FUNC_ID_FIN)
	{
		CFadeManager::GetManager()->Request(4.f, false);
		m_fadeStep = &CSceneManager::StepFadeOut;
	}
}
void CSceneManager::StepFadeOut()
{
	//���̃V�[����Draw���������đҋ@
	m_scene->Draw();

	//fade�������΂���
	if (CFadeManager::GetManager()->IsEnd())
		m_fadeStep = nullptr;
}

void CSceneManager::StepGameTime()
{
	if (!m_isStopGameTime)
	{
		m_gameTime += FRAME_TIME;
	}

	if (m_gameTime >= 99999999)
	{
		m_gameTime = 99999999;
	}
}
void CSceneManager::DrawGameTime()
{
	//	�~���b�Ȃ̂ŕϊ����ĕ`��
	int time = m_gameTime / 1000;
	int decimal = (m_gameTime - time * 1000) / 10;

	DrawExtendFormatString(15, 50, 1.5, 1.5, GetColor(20, 20, 20), " %d�F%02d", time, decimal);
}
int CSceneManager::GetGameTime()const
{
	return m_gameTime;
}
void CSceneManager::SetGameTime(const int time)
{
	m_gameTime = time;
}

float CSceneManager::GetGameTimeT()
{
	return m_gameTime / 1000.f;
}