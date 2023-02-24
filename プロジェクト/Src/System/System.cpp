#include "DxLib.h"

#include "System.h"

#include <crtdbg.h>

#include "../Common.h"

#include "../GameManager/GameManager.h"
#include "../Input/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../FPS/FPS.h"
#include "../Camera/CameraManager.h"
#include "../Character/CharacterManager.h"
#include "../Collision/CollisionManager.h"
#include "../Draw/DrawManager.h"
#include "../Field/FieldManager.h"
#include "../Memory/MemoryManager.h"
#include "../Sky/SkyManager.h"
#include "../Effekseer.h"
#include "../Beam/BeamManager.h"
#include "../Magic/MagicManager.h"
#include "../UI/UIManager.h"
#include "../Fade/Fade.h"

bool CSystem::m_isEnd = false;

void CSystem::FinGame()
{
	m_isEnd = true;
}


//������
bool CSystem::Init()
{
	SetMainWindowText("Exter'");


	//�E�B���h�E�̏�Ԃ�ݒ肷��
	ChangeWindowMode(true);
#ifdef DEBUG_MODE
	
#endif

	//�E�B���h�E�T�C�Y��ύX
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

	//DX���C�u�����̏�����
	if (DxLib_Init() == -1) { return false; }

	//�`�悷��X�N���[����ݒ�
	SetDrawScreen(DX_SCREEN_BACK);

	//�RD���f���ݒ�
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetTransColor(255, 0, 255);

	// Z�\�[�g����
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetTransColor(255, 0, 255);	// ���ߐF�w��

	return true;
}

//���C���V�X�e��
void CSystem::SystemMain()
{
	//�Q�[�����C�����[�v
	while (ProcessMessage() != -1)
	{
		Sleep(1);	//�V�X�e���ɏ�����Ԃ�

		//FPS����
		CFrameRate::Step();

		// ���݂̎��Ԃ��A�O��̃t���[�������
		// 1000/60�~���b�i1/60�b�j�ȏ�o�߂��Ă����珈�������s����
		if (CFrameRate::IsPassageFrameTime())
		{
			//�t���[�����[�g���X�V
			CFrameRate::Update();

			if (m_isEnd)
			{
				CSystem::AllFin();
				//�G�X�P�[�v�L�[�������ꂽ��I��
				break;
			}

			//��ʂɕ\�����ꂽ���̂��������i�P���[�v�̂P�x�����s���j
			ClearDrawScreen();

			//���ʂ̃V�X�e��
			StepSystem();

			/*
				===============���݂̃V�[���̌��݂̏�����======================
			*/
			CSceneManager::GetManager()->CurrrentSceneFunc();
			/*
				===========================================================
			*/

			CFadeManager::GetManager()->Draw();

			//FPS�v�Z
			CFrameRate::Calc();

#ifdef DEBUG_MODE
			//FPS�\��
			CFrameRate::Draw();
#endif 

			

			//�t���b�v�֐��i���[�v�̏I���ɂP�x�����s���j
			ScreenFlip();

		}//�t���[�����[�g��if���I���

	}//���C�����[�v�̏I���
}

//���ʂ̏�����
void CSystem::InitSystem()
{
	/*
		���I�N���X
	*/
	
	//�Q�[���}�l�[�W���[������
	CGameManager::CreateManager();
	//���͐��䏉����
	CInputManager::CreateManager();
	//�V�[���}�l�[�W���[����
	CSceneManager::CreateManager();
	//�t�F�[�h�}�l�[�W���[����
	CFadeManager::CreateManager();


	/*
		�ÓI�N���X
	*/

	//FPS������
	CFrameRate::Init();
}

//���ʂ̃X�e�b�v
void CSystem::StepSystem()
{
	//���͐���X�e�b�v
	CInputManager::GetManager()->Step();

	//�t�F�[�h�A�b�v�f�[�g
	CFadeManager::GetManager()->Update();
}

//�㏈��
void CSystem::Fin()
{
	AllFin();

	//�Ō�ɂP�񂾂���鏈���������ɏ���
	CSoundManager::Fin();

	CInputManager::GetManager()->DeleteManager();
	CSceneManager::GetManager()->DeleteManager();
	CGameManager::GetGameManager()->DeleteManager();
}

int CSystem::AllFin()
{
	CSoundManager::Fin();
	CInputManager::GetManager()->DeleteManager();
	CSceneManager::GetManager()->DeleteManager();
	CFadeManager::GetManager()->DeleteManager();;

#ifdef DEBUG_MODE
	//������Check������
	CMemoryManager::GetManager()->CheckComplete();

#endif

	CCameraManager::GetManager()->DeleteManager();
	CCharacterManager::GetManager()->DeleteAllManager();
	CDrawManager::GetManager()->DeleteManager();
	CFieldManager::GetManager()->DeleteManager();
	CInputManager::GetManager()->DeleteManager();
	CSceneManager::GetManager()->DeleteManager();
	CSkyManager::GetManager()->DeleteManager();
	CCollisionManager::GetManager()->DeleteManager();
	CSoundManager::Fin();
	CEffekseerCtrl::Exit();
	CBeamManager::GetManager()->DeleteManager();
	CMagicManager::GetManager()->DeleteManager();
	CUIManager::GetManager()->DeleteManager();
	CFadeManager::GetManager()->DeleteManager();

	CGameManager::GetGameManager()->DeleteManager();


	return -1;
}