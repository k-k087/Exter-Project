#pragma once

class CSystem
{
public:
	static bool m_isEnd;

	static void FinGame();

	//������
	static bool Init();

	//���C���V�X�e��
	static void SystemMain();

	//���ʂ̏�����
	static void InitSystem();
	//���ʂ̃X�e�b�v
	static void StepSystem();

	static int AllFin();

	//�㏈��
	static void Fin();
};