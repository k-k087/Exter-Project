#pragma once

#include "../GameManager/ManagerBase.h"

enum InputKind
{
	INPUT_KIND_NONE = -1,

	I_KEY,
	I_MOUSE,
	I_PAD,

	INPUT_KIND_NUM,
};

class CInput;

//�C���v�b�g�}�l�[�W���[�N���X
class CInputManager : public CManagerBase
{

private:
	CInput* m_input[INPUT_KIND_NUM];	//�C���X�^���X
	static CInputManager* m_inputManager;	//�C���v�b�g�}�l�[�W���[����

	//�R���X�g���N�^
	CInputManager() :
		m_input{ NULL }
	{}

public:

	//������
	void Init();
	//�X�e�b�v
	void Step();
	//�㏈��
	void Fin();

	//�}�l�[�W���[�����A�擾�A�폜
	static void CreateManager();
	static CInputManager* GetManager();
	void DeleteManager();

	static bool IsPush(int code, InputKind kind);
	static bool IsKeep(int code, InputKind kind);
	static bool IsRelease(int code, InputKind kind);
	static bool IsDown(int code, InputKind kind);
};