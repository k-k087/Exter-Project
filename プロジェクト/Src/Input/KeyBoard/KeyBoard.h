#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "../Input.h"

class CKeyBoard : public CInput
{
private:
	static const int KEY_BUF_LEN = 256;
	char m_currentBuf[KEY_BUF_LEN];	//���݃t���[���̃L�[���
	char m_preBuf[KEY_BUF_LEN];		//�O�t���[���̃L�[���
	
public:
	//������
	void Init() override;
	//�X�e�b�v
	void Step() override;

	//�������ꂽ
	bool IsPush(int key_code) override;
	//�����Ă��邩
	bool IsKeep(int key_code) override;
	//�����ꂽ
	bool IsRelease(int key_code) override;
	//�P���ɉ�����Ă��邩
	bool IsDown(int key_code) override;

};


#endif
