#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "../Input.h"

class CKeyBoard : public CInput
{
private:
	static const int KEY_BUF_LEN = 256;
	char m_currentBuf[KEY_BUF_LEN];	//現在フレームのキー情報
	char m_preBuf[KEY_BUF_LEN];		//前フレームのキー情報
	
public:
	//初期化
	void Init() override;
	//ステップ
	void Step() override;

	//今押された
	bool IsPush(int key_code) override;
	//押しているか
	bool IsKeep(int key_code) override;
	//離された
	bool IsRelease(int key_code) override;
	//単純に押されているか
	bool IsDown(int key_code) override;

};


#endif
