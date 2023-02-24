#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "../Input.h"

class CMouse : public CInput
{
private:
	static CMouse* m_MouseInfo;

private:
	static const int MOUSE_BUF_LEN = 3;
	char m_currentBuf[MOUSE_BUF_LEN];	//���݃t���[���̃}�E�X�{�^�����
	char m_preBuf[MOUSE_BUF_LEN];		//�O�t���[���̃}�E�X�{�^�����

	int m_posX, m_posY;
	int m_oldPosX, m_oldPosY;

public:
	static CMouse* CreateMouse();
	static CMouse* GetMouse() { return m_MouseInfo; }

	bool IsMoveMousePos();

	VECTOR GetDistance();

	//������
	void Init() override;
	//�X�e�b�v
	void Step() override;

	//�������ꂽ
	bool IsPush(int mouse_code) override;
	//�����Ă��邩
	bool IsKeep(int mouse_code) override;
	//�����ꂽ
	bool IsRelease(int mouse_code) override;
	//�P���ɉ�����Ă��邩
	bool IsDown(int mouse_code) override;
};


#endif
