#include "DxLib.h"
#include "GameOverScene.h"

#include "../Scene.h"
#include "../SceneManager.h"

#include "../../Common.h"

#include "../../Input/InputManager.h"
#include "../../Debug/Debug.h"

//�Q�[���I�[�o�[�w�i�̃p�X
static const char* GAMEOVER_PATH = "Data/Graph/GameOver/GameOver.png";


void CGameOverScene::Init()
{
	m_handle = -1;
}
void CGameOverScene::Load()
{
	m_handle = LoadGraph(GAMEOVER_PATH);
}
void CGameOverScene::Step()
{
	//Enter�L�[�����ꂽ�Ȃ�
	if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY) ||
		CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
	{
		//�㏈����
		CSceneManager::GetManager()->SetFuncID(SCENE_FUNC_ID_FIN);
	}
}
void CGameOverScene::Draw()
{
#if 0

	DrawExtendFormatString(100, 100, 10.f, 10.f, GetColor(255, 255, 255), "�Q�[���I�[�o�[����");

#endif

	DrawGraph(0, 0, m_handle, true);
}
void CGameOverScene::Loop()
{
	Step();
	Draw();
}
void CGameOverScene::Fin()
{
	DeleteGraph(m_handle);
	m_handle = -1;

	//�^�C�g����ʂ�
	CSceneManager::GetManager()->ChangeScene(SCENE_ID_TITLE);
}
