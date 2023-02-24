#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "../Input.h"

class CMouse : public CInput
{
private:
	static CMouse* m_MouseInfo;

private:
	static const int MOUSE_BUF_LEN = 3;
	char m_currentBuf[MOUSE_BUF_LEN];	//現在フレームのマウスボタン情報
	char m_preBuf[MOUSE_BUF_LEN];		//前フレームのマウスボタン情報

	int m_posX, m_posY;
	int m_oldPosX, m_oldPosY;

public:
	static CMouse* CreateMouse();
	static CMouse* GetMouse() { return m_MouseInfo; }

	bool IsMoveMousePos();

	VECTOR GetDistance();

	//初期化
	void Init() override;
	//ステップ
	void Step() override;

	//今押された
	bool IsPush(int mouse_code) override;
	//押しているか
	bool IsKeep(int mouse_code) override;
	//離された
	bool IsRelease(int mouse_code) override;
	//単純に押されているか
	bool IsDown(int mouse_code) override;
};


#endif
