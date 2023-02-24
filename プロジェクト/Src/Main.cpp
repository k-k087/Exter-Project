#include "DxLib.h"

#include <conio.h>

#include "System/System.h"
#include "Debug/Debug.h"
#include "Memory/MemoryManager.h"

#include "Sound/SoundManager.h"

#include "Common.h"

#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//���������[�N�`�F�b�N�p�f�o�b�O�t���O�쐬
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifdef DEBUG_MODE

	//�������Ǘ��N���X������
	CMemoryManager::CreateManager()->Init();

#endif

	

	//�V�X�e��������
	if (!CSystem::Init()) { return -1; }

	//���ʂ̏�����
	CSystem::InitSystem();



	/*
		�Q�[�����C���V�X�e��
	*/
	CSystem::SystemMain();



	//�Q�[���㏈��
	CSystem::Fin();

#ifdef DEBUG_MODE

	//�������Ǘ��N���XDelete
	CMemoryManager::GetManager()->DeleteManager();
#endif

	//DX���C�u�����̌㏈��
	DxLib_End();

	return 0;
}