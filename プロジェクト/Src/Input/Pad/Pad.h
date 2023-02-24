#ifndef __PAD_H__
#define __PAD_H__

#include "DxLib.h"

#include "../Input.h"

enum StickKind
{
	LEFT_STICK_X,		//���X�e�B�b�N��X
	LEFT_STICK_Y,		//���X�e�B�b�N��Y
	RIGHT_STICK_X,		//�E�X�e�B�b�N��X
	RIGHT_STICK_Y,		//�E�X�e�B�b�N��Y

	STICK_MAX_NUM,
};

enum TriggerKind
{
	TRRIGER_LEFT,		//�g���K�[�̍�
	TRRIGER_RIGHT,		//�g���K�[�̉E

	TRIGGER_MAX_NUM,
};

class CPad : public CInput
{
private:
	static CPad* m_padInfo;

public:
	static const int PAD_BUF_LEN = 16;

private:
	XINPUT_STATE m_pad;							//�p�b�h�����󂯎��\����
	unsigned char m_currentBuf[PAD_BUF_LEN];	//���݂̃t���[���̃p�b�h�̃{�^�����
	unsigned char m_preBuf[PAD_BUF_LEN];		//�O�t���[���̃p�b�h�̃{�^�����
	float m_stickBuf[STICK_MAX_NUM];			//�p�b�h�̃X�e�B�b�N���
	float m_triggerBuf[TRIGGER_MAX_NUM];		//�R���g���[���[�̃g���K�[���i�[�z��

public:
	static CPad* CreatePad();
	static CPad* GetPad() { return m_padInfo; }

	//������
	void Init() override;
	//�X�e�b�v
	void Step() override;

	//�������ꂽ
	bool IsPush(int button_code) override;
	//�����Ă��邩
	bool IsKeep(int button_code) override;
	//�����ꂽ
	bool IsRelease(int button_code) override;
	//�P���ɉ�����Ă��邩
	bool IsDown(int button_code) override;

	//�X�e�B�b�N�l�Q�b�g
	float GetStickBuf(StickKind kind)const { return m_stickBuf[kind]; }
	//�X�e�B�b�N�����͂���Ă��邩
	bool IsLeftStick();
	bool IsRightStick();
	//�g���K�[�l�Q�b�g
	float GetTriggerBuf(TriggerKind kind)const { return m_triggerBuf[kind]; }
};


#endif